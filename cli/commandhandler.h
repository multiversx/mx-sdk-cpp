#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <sodium.h>
#include <iostream>

#include "inputhandler/ext.h"
#include "erdsdk.h"
#include "config/cliconfig.h"

class SecretKeyProvider
{
public:
    explicit SecretKeyProvider(std::shared_ptr<ISecretKey> &&keyFile) :
        m_keyFile(std::move(keyFile)) {};

    bytes getSeed()
    {
        return m_keyFile->getSeed();
    }
    Address getAddress()
    {
        return m_keyFile->getAddress();
    }

private:
    std::shared_ptr<ISecretKey> m_keyFile;
};

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

void signTransaction(Transaction &transaction, SecretKeyProvider keyFile)
{
    Signer signer(keyFile.getSeed());
    transaction.sign(signer);
}

SecretKeyProvider getSecretKeyFileFromInput(cxxopts::ParseResult const &input)
{
    auto const keyPath = input["key"].as<std::string>();
    auto const fileExtension = IFile::getFileExtension(keyPath);

    std::shared_ptr<ISecretKey> keyFileType;

    if (fileExtension == "pem")
    {
        keyFileType = std::make_shared<PemFileReader>(keyPath);
    }
    else if (fileExtension == "json")
    {
        auto const password = input["password"].as<std::string>();

        if (password.empty())
        {
            throw std::invalid_argument(ERROR_MSG_MISSING_PASSWORD);
        }
        keyFileType = std::make_shared<KeyFileReader>(keyPath, password);
    }
    else
    {
        throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);
    }

    return SecretKeyProvider(std::move(keyFileType));
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

void handleCreateSignedTransaction(cxxopts::ParseResult const &result)
{
    ih::wrapper::TransactionInputWrapper const transactionInputWrapper(result);

    ih::JsonFile jsonFile(transactionInputWrapper.getOutputFile());
    auto secretKeyFile = internal::getSecretKeyFileFromInput(result);

    Transaction transaction = internal::createTransaction(transactionInputWrapper, secretKeyFile.getAddress());
    internal::signTransaction(transaction, secretKeyFile);

    jsonFile.writeDataToFile(transaction.serialize());
    std::cerr<<"here"<<"\n";
}

void handleIssueESDT(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

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

    auto secretKeyFile = internal::getSecretKeyFileFromInput(result);

    Address const sender = secretKeyFile.getAddress();
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
    internal::signTransaction(tx, secretKeyFile);

    ProxyProvider proxy(config.proxyUrl);
    auto const txHash =  proxy.send(tx).hash;
    std::cerr<< "Transaction hash: " << txHash << "\n";
}

void handleTransferESDT(cxxopts::ParseResult const &result)
{
    auto const config = CLIConfig().config();

    auto const nonce = result["nonce"].as<uint64_t>();
    auto const gasPrice = result["gas-price"].as<uint64_t>();
    auto const gasLimit = result["gas-limit"].as<uint64_t>();
    auto const receiverAdr = result["receiver"].as<std::string>();
    auto const token = result["token"].as<std::string>();
    auto const value = result["value"].as<std::string>();
    auto const function = result["function"].as<std::string>();
    auto const args = result["args"].as<std::vector<std::string>>();

    auto secretKeyFile = internal::getSecretKeyFileFromInput(result);

    Address const sender = secretKeyFile.getAddress();
    Address const receiver = Address(receiverAdr);

    Transaction tx;
    tx.m_nonce = nonce;
    tx.m_value = value;
    tx.m_sender = std::make_shared<Address>(sender);
    tx.m_receiver = std::make_shared<Address>(receiver);
    tx.m_gasPrice = gasPrice;
    tx.m_gasLimit = gasLimit;
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

    internal::signTransaction(tx, secretKeyFile);

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

#endif
