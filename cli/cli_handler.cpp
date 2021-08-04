#include "cli_handler.h"
#include "cli_utility.h"
#include "erdsdk.h"
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
    auto const decimals = result["dec"].as<std::string>();

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

    Address const receiver = Address(config.issueESDTSCAddress);
    tx.m_receiver = std::make_shared<Address>(receiver);

    ESDTProperties const esdtProperties
            {canFreeze,
             canWipe,
             canPause,
             canMint,
             canBurn,
             canChangeOwner,
             canUpgrade,
             canAddSpecialRoles};

    prepareTransactionForESDTIssuance(tx, token, ticker, supply, decimals, esdtProperties);
    utility::signTransaction(tx, keyFile);

    ProxyProvider proxy(config.proxyUrl);
    auto const txHash =  proxy.send(tx).hash;
    std::cerr<< "Transaction hash: " << txHash << "\n";
}

void handleTransferESDT(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

    auto const token = result["token"].as<std::string>();
    auto const function = result["function"].as<std::string>();
    auto const args = result["args"].as<std::vector<std::string>>();

    auto keyFile = std::make_shared<SecretKeyProvider>(result);
    auto tx = utility::createTransaction(result, config);

    prepareTransactionForESDTTransfer(tx, token, function);

    if (!args.empty())
    {
        std::string txData (tx.m_data->begin(), tx.m_data->end());
        for (auto const &arg : args)
        {
            txData += "@" + arg;
        }
        *tx.m_data = bytes(txData.begin(), txData.end());
    }

    utility::signTransaction(tx, keyFile);

    ProxyProvider proxy(config.proxyUrl);
    auto const txHash =  proxy.send(tx).hash;
    std::cerr<< "Transaction hash: " << txHash << "\n";
}

void handleRequest(ih::ArgParsedResult const &parsedResult)
{
    switch (parsedResult.requestType)
    {
        case ih::help:
        {
            std::cerr<< parsedResult.help;
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
        default:
        {
            break;
        }
    }
}

}

