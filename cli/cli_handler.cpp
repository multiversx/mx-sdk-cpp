#include "cli_handler.h"
#include "cli_utility.h"
#include "mxsdk.h"
#include "config/cliconfig.h"
#include "secretkeyprovider.h"

#include <sodium.h>
#include <iostream>
#include <fstream>

namespace cli
{

void init()
{
    if (sodium_init() < 0)
    {
        throw std::runtime_error(ERROR_MSG_SODIUM_INIT);
    }
}

void handleCreateSignedTransaction(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

    auto const path = result["outfile"].as<std::string>();
    auto const keyFile = std::make_shared<SecretKeyProvider>(result);

    Transaction transaction = utility::createTransaction(result, config);
    utility::signTransaction(transaction, keyFile);

    IFile file(path, "json");
    std::ofstream outFile(file.getFilePath());
    outFile << transaction.serialize();
    outFile.close();

    std::cerr << "Transaction created and written successfully!\n";
}

void handleIssueESDT(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

    auto const token = result["token"].as<std::string>();
    auto const ticker = result["ticker"].as<std::string>();
    auto const supply = result["supply"].as<std::string>();
    auto const decimals = result["dec"].as<uint32_t>();

    auto const canFreeze = result["can-freeze"].as<bool>();
    auto const canWipe = result["can-wipe"].as<bool>();
    auto const canPause = result["can-pause"].as<bool>();
    auto const canMint = result["can-mint"].as<bool>();
    auto const canBurn = result["can-burn"].as<bool>();
    auto const canChangeOwner = result["can-change-owner"].as<bool>();
    auto const canUpgrade = result["can-upgrade"].as<bool>();
    auto const canAddSpecialRoles = result["can-add-roles"].as<bool>();

    auto keyFile = std::make_shared<SecretKeyProvider>(result);
    auto tx = utility::createTransaction(result, config);

    ESDTProperties const esdtProperties
            {canFreeze,
             canWipe,
             canPause,
             canMint,
             canBurn,
             canChangeOwner,
             canUpgrade,
             canAddSpecialRoles};

    ProxyProvider proxy(config.proxyUrl);
    NetworkConfig cfg = proxy.getNetworkConfig();

    TransactionFactory txFactory(cfg);
    tx = txFactory.createESDTIssue(
            tx.m_nonce,
            keyFile->getAddress(),
            tx.m_gasPrice,
            token,
            ticker,
            BigUInt(supply),
            decimals,
            esdtProperties
    )->buildSigned(keyFile->getSeed());

    auto const txHash = proxy.send(tx);
    std::cerr << "Transaction hash: " << txHash << "\n";
}

void handleTransferESDT(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

    auto const token = result["token"].as<std::string>();
    auto const function = result["function"].as<std::string>();
    auto const args = result["args"].as<std::vector<std::string>>();

    auto keyFile = std::make_shared<SecretKeyProvider>(result);
    auto tx = utility::createTransaction(result, config);

    ProxyProvider proxy(config.proxyUrl);
    NetworkConfig cfg = proxy.getNetworkConfig();

    TokenPayment tokenPayment = TokenPayment::fungibleFromBigUInt(token, BigUInt(tx.m_value));
    TransactionFactory txFactory(cfg);
    tx = txFactory.createESDTTransfer(
                    tokenPayment,
                    tx.m_nonce,
                    keyFile->getAddress(),
                    *tx.m_receiver,
                    tx.m_gasPrice)
            ->build();

    if (!args.empty())
    {
        std::string txData(tx.m_data->begin(), tx.m_data->end());
        for (auto const &arg: args)
        {
            txData += "@" + arg;
        }
        *tx.m_data = bytes(txData.begin(), txData.end());
    }
    tx.m_gasLimit = GasEstimator(cfg).forESDTTransfer(tx.m_data->size());
    utility::signTransaction(tx, keyFile);

    auto const txHash = proxy.send(tx);
    std::cerr << "Transaction hash: " << txHash << "\n";
}

void handleSetNetworkConfig(cxxopts::ParseResult const &result)
{
    std::string network = result["config"].as<std::string>();
    std::transform(network.begin(), network.end(), network.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    auto const cliConfig = CLIConfig();
    network[0] = char(toupper(network[0]));
    if (network == NETWORK_MAINNET)
    {
        cliConfig.setNetwork(Mainnet);
    }
    else if (network == NETWORK_DEVNET)
    {
        cliConfig.setNetwork(Devnet);
    }
    else if (network == NETWORK_TESTNET)
    {
        cliConfig.setNetwork(Testnet);
    }
    else if (network == NETWORK_LOCAL)
    {
        cliConfig.setNetwork(Local);
    }
    else
    {
        throw std::invalid_argument("Invalid network: " + network);
    }

    auto cfg = cliConfig.config();
    std::cerr << "Config changed to: " << network << "\n";
    std::cerr << "ChainID = " << cfg.chainID << "\n";
    std::cerr << "ProxyUrl = " << cfg.proxyUrl << "\n";
}

void handleRequest(ih::ArgParsedResult const &parsedResult)
{
    switch (parsedResult.requestType)
    {
        case ih::help:
        {
            std::cerr << parsedResult.help;
            break;
        }
        case ih::createSignedTransaction:
        {
            handleCreateSignedTransaction(parsedResult.result);
            break;
        }
        case ih::issueESDT:
        {
            handleIssueESDT(parsedResult.result);
            break;
        }
        case ih::transferESDT:
        {
            handleTransferESDT(parsedResult.result);
            break;
        }
        case ih::setNetworkConfig:
        {
            handleSetNetworkConfig(parsedResult.result);
            break;
        }
        default:
        {
            break;
        }
    }
}

}

