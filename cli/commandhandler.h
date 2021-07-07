#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <sodium.h>
#include <iostream>
#include "inputhandler/ext.h"
#include "erdsdk.h"

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
    std::cerr << "File loaded successfully! Bech32 address: " << pemReader.getAddress().getBech32Address() << "\n";
}

void handleCreateSignedTransactionWithPemFile(cxxopts::ParseResult const &result)
{
    ih::wrapper::TransactionInputWrapper const transactionInputWrapper(result);
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(result);

    ih::JsonFile jsonFile(transactionInputWrapper.getOutputFile());
    PemFileReader pemReader(pemInputWrapper.getPemFilePath());

    Transaction transaction = internal::createTransaction(transactionInputWrapper, pemReader.getAddress());
    internal::signTransaction(transaction,pemReader.getSeed());

    jsonFile.writeDataToFile(transaction.serialize());
}

void handleRequest(ih::ParseResult const &parsedRes)
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
        default:
        {
            break;
        }
    }
}

}

#endif
