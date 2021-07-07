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
typedef std::map<std::string, std::vector<std::string>> commandGroupMap;

static commandGroupMap const cmdGroupMap =
        {
                {"transaction", {"new"}},
                {"pem",         {"load"}},
                {"help",        {}}
        };

void showSubGroupAvailableCmds(std::string const &cmdGroup)
{
    std::vector<std::string> cmd = cmdGroupMap.at(cmdGroup);

    if (cmd.empty()) std::cerr << "-";

    else
    {
        for (std::string const &subCmd : cmd)
        {
            std::cerr << subCmd << " ";
        }
    }
}

void showInfo()
{
    std::cerr << "----\nInfo\n----\n\nCommand groups: Avaiable arguments\n";

    for (const auto & it : cmdGroupMap)
    {
        std::cerr << it.first << ": ";
        showSubGroupAvailableCmds(it.first);
        std::cerr << "\n";
    }
}

void reportError(errorCode const err)
{
    std::cerr << "Error. ";

    if (errors.find(err) != errors.end()) std::cerr << errors.at(err) << "\n";
}

bool init()
{
    bool ret = true;
    if (sodium_init() < 0)
    {
        ret = false;
        reportError(ERROR_SODIUM_INIT);
    }
    return ret;
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
            showInfo();
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
            showInfo();
            break;
        }
    }
}

}

#endif
