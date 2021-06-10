#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <sodium.h>
#include <iostream>
#include "inputhandler/ext.h"
#include "erdsdk.h"

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

void handleLoadPemFile(const std::map<uint32_t, std::string> &userInputs)
{
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(userInputs);
    ih::PemFileReader pemReader(pemInputWrapper.getPemFilePath());
    std::cerr << "File loaded successfully! Bech32 address: " << pemReader.getAddress().getBech32Address() << "\n";
}

void handleCreateSignedTransactionWithPemFile(const std::map<uint32_t, std::string> &userInputs)
{
    ih::wrapper::TransactionInputWrapper const transactionInputWrapper(userInputs);
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(userInputs);

    ih::JsonFile jsonFile(transactionInputWrapper.getOutputFile());
    ih::PemFileReader pemReader(pemInputWrapper.getPemFilePath());

    //TODO: Maybe in the future add a transaction builder here
    Transaction transaction
        (transactionInputWrapper.getNonce(), transactionInputWrapper.getValue(),
         transactionInputWrapper.getReceiver(), pemReader.getAddress(),
         transactionInputWrapper.getGasPrice(), transactionInputWrapper.getGasLimit(),
         transactionInputWrapper.getData(), transactionInputWrapper.getChainId(),
         transactionInputWrapper.getVersion());

    Signer signer(pemReader.getSecretKey());
    transaction.applySignature(signer);
    jsonFile.writeDataToFile(transaction.getSerializedTransaction());
}

void handleRequest(ih::RequestedCmd const &requestedCmd)
{
    switch (requestedCmd.getRequestType())
    {
        case ih::help:
        {
            showInfo();
            break;
        }
        case ih::loadPemFile:
        {
            handleLoadPemFile(requestedCmd.getUserInputs());
            break;
        }
        case ih::createSignedTransactionWithPemFile:
        {
            handleCreateSignedTransactionWithPemFile(requestedCmd.getUserInputs());
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
