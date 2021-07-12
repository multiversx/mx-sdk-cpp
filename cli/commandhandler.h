#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <sodium.h>
#include <iostream>

#include "inputhandler/ext.h"
#include "erdsdk.h"
#include "config/networkconfig.h"

namespace internal
{

Transaction createTransaction(ih::wrapper::TransactionInputWrapper const &txWrapper, Address const& senderAddress)
{
    //TODO: Maybe in the future add a transaction builder here
    return Transaction
            (txWrapper.getNonce(), txWrapper.getValue(),
             txWrapper.getReceiver(), senderAddress,
             txWrapper.getReceiverName(), txWrapper.getSenderName(),
             txWrapper.getGasPrice(), txWrapper.getGasLimit(),
             txWrapper.getData(), txWrapper.getSignature(),
             txWrapper.getChainId(), txWrapper.getVersion(),
             txWrapper.getOptions());
}

// TODO: replace keyFile param with template param once
// we have key store or other key files implemented
void signTransaction(Transaction &transaction, PemFileReader const &keyFile)
{
    Signer signer(keyFile.getSeed());
    transaction.sign(signer);
}

}

namespace cli
{

void init()
{
    if (sodium_init() < 0)
    {
        throw std::runtime_error(ERROR_MSG_SODIUM_INIT);
    }
}

void handleLoadPemFile(cxxopts::ParseResult const &result)
{
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(result);
    PemFileReader pemReader(pemInputWrapper.getPemFilePath());
    std::cerr << "File loaded successfully!\nBech32 address: " << pemReader.getAddress().getBech32Address() << "\n";
}

void handleCreateSignedTransactionWithPemFile(cxxopts::ParseResult const &result)
{
    ih::wrapper::TransactionInputWrapper const transactionInputWrapper(result);

    ih::JsonFile jsonFile(transactionInputWrapper.getOutputFile());
    PemFileReader pemReader(transactionInputWrapper.getInputFile());

    Transaction transaction = internal::createTransaction(transactionInputWrapper, pemReader.getAddress());
    internal::signTransaction(transaction, pemReader);

    jsonFile.writeDataToFile(transaction.serialize());
}

void handleIssueESDT(cxxopts::ParseResult const &result)
{
    auto const config = NetworkConfig().config();

    auto const token = result["token"].as<std::string>();
    auto const ticker = result["ticker"].as<std::string>();
    auto const supply = result["supply"].as<std::string>();
    auto const decimals = result["dec"].as<std::string>();
    auto const gasPrice = result["gas-price"].as<uint64_t>();
    auto const nonce = result["nonce"].as<uint64_t>();

    auto const canFreeze = result["can-freeze"].as<bool>();
    auto const canWipe = result["can-wipe"].as<bool>();
    auto const canPause = result["can-pause"].as<bool>();
    auto const canMint = result["can-mint"].as<bool>();
    auto const canBurn = result["can-burn"].as<bool>();
    auto const canChangeOwner = result["can-change-owner"].as<bool>();
    auto const canUpgrade = result["can-upgrade"].as<bool>();
    auto const canAddSpecialRoles = result["can-add-roles"].as<bool>();

    auto const pemPath = result["pem"].as<std::string>();

    PemFileReader const pemFileReader(pemPath);
    Address const sender = pemFileReader.getAddress();
    Address const receiver = Address(config.issueESDTSCAddress);

    Transaction tx;
    tx.m_nonce = nonce;
    tx.m_sender = std::make_shared<Address>(sender);
    tx.m_receiver = std::make_shared<Address>(receiver);
    tx.m_gasPrice = gasPrice;
    tx.m_chainID = config.chainID;

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
    internal::signTransaction(tx, pemFileReader);

    ProxyProvider proxy(config.proxyUrl);
    auto const txHash =  proxy.send(tx).hash;
    std::cerr<< "Transaction hash: " << txHash;
}

void handleTransferESDT(cxxopts::ParseResult const &result)
{
    auto const config = NetworkConfig().config();

    auto const nonce = result["nonce"].as<uint64_t>();
    auto const gasPrice = result["gas-price"].as<uint64_t>();
    auto const token = result["token"].as<std::string>();
    auto const value = result["value"].as<std::string>();
    auto const function = result["function"].as<std::string>();
    auto const args = result["args"].as<std::vector<std::string>>();

    auto const pemPath = result["pem"].as<std::string>();

    PemFileReader const pemFileReader(pemPath);
    Address const sender = pemFileReader.getAddress();
    Address const receiver = Address(config.transferESDTSCAddress);

    Transaction tx;
    tx.m_nonce = nonce;
    tx.m_sender = std::make_shared<Address>(sender);
    tx.m_receiver = std::make_shared<Address>(receiver);
    tx.m_gasPrice = gasPrice;
    tx.m_chainID = config.chainID;

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

    internal::signTransaction(tx, pemFileReader);

    ProxyProvider proxy(config.proxyUrl);
    auto const txHash =  proxy.send(tx).hash;
    std::cerr<< "Transaction hash: " << txHash;
}

void handleRequest(ih::ArgParsedResult const &parsedRes)
{
    switch (parsedRes.requestType)
    {
        case ih::help:
        {
            std::cerr<< parsedRes.help;
            break;
        }
        case ih::loadPemFile:
        {
            handleLoadPemFile(parsedRes.result);
            break;
        }
        case ih::createSignedTransactionWithPemFile:
        {
            handleCreateSignedTransactionWithPemFile(parsedRes.result);
            break;
        }
        case ih::issueESDT:
        {
            handleIssueESDT(parsedRes.result);
            break;
        }
        case ih::transferESDT:
        {
            handleTransferESDT(parsedRes.result);
            break;
        }
        default:
        {
            break;
        }
    }
}

}

#endif
