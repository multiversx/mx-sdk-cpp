#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <sodium.h>
#include <iostream>

#include "inputhandler/ext.h"
#include "erdsdk.h"
#include "toml/cpptoml.h"

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

void signTransaction(Transaction &transaction, bytes const& seed)
{
    Signer signer(seed);
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
    internal::signTransaction(transaction,pemReader.getSeed());

    jsonFile.writeDataToFile(transaction.serialize());
}

void handleIssueESDT(cxxopts::ParseResult const &result)
{
    auto data = cpptoml::parse_file("config.toml");
    auto config = data->get_table("Config");
    std::string const networkConfig = *config->get_as<std::string>("NetworkConfig");

    auto userConfig = data->get_table("Testnet");
    std::string const chainID = *userConfig->get_as<std::string>("ChainID");
    std::string const issueESDTSCAddr = *userConfig->get_as<std::string>("IssueESDTSCAddress");

    std::string const token = result["token-id"].as<std::string>();
    std::string const ticker = result["ticker"].as<std::string>();
    std::string const supply = result["supply"].as<std::string>();
    std::string const decimals = result["dec"].as<std::string>();
    uint64_t const gasPrice = result["gas-price"].as<uint64_t>();
    uint64_t const nonce = result["nonce"].as<uint64_t>();

    std::string const pem = result["pem"].as<std::string>();

    PemFileReader pemFileReader(pem);

    Transaction tx;
    tx.m_gasPrice = gasPrice;
    tx.m_nonce = nonce;
    tx.m_sender = std::make_shared<Address>(Address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u"));
    tx.m_receiver = std::make_shared<Address>(Address(issueESDTSCAddr));
    tx.m_chainID = chainID;

    prepareTransactionForESDTIssuance(tx, token, ticker, supply, decimals);

    internal::signTransaction(tx, pemFileReader.getSeed());
    std::cerr<<tx.serialize();
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
        default:
        {
            break;
        }
    }
}

}

#endif
