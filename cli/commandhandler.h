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

void showSubGroupAvailableCmds(std::string cmdGroup)
{
    std::vector<std::string> cmd = cmdGroupMap.at(cmdGroup);

    if (cmd.size() == 0) std::cerr << "-";

    else
    {
        for (std::string subCmd : cmd)
        {
            std::cerr << subCmd << " ";
        }
    }
}

void showInfo()
{
    std::cerr << "----\nInfo\n----\n\nCommand groups: Avaiable arguments\n";

    for (auto it = cmdGroupMap.begin(); it != cmdGroupMap.end(); ++it)
    {
        std::cerr << it->first << ": ";
        showSubGroupAvailableCmds(it->first);
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

    if (pemReader.isFileValid())
    {
        std::cerr << "Bech32 address: " << pemReader.getAddress().getBech32Address() << "\n";
    }
    else
    {
        reportError(ERROR_PEM_INPUT_FILE);
    }
}

void handleCreateSignedTransactionWithPemFile(const std::map<uint32_t, std::string> &userInputs)
{
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(userInputs);
    ih::wrapper::TransactionInputWrapper const transactionInputWrapper(userInputs);

    ih::JsonFileHandler jsonHandler(transactionInputWrapper.getOutputFile());
    ih::PemFileReader pemFileHandler(pemInputWrapper.getPemFilePath());

    if (!pemFileHandler.isFileValid())
    {
        reportError(ERROR_PEM_INPUT_FILE);
    }
    else if (!jsonHandler.isFileValid())
    {
        reportError(ERROR_JSON_OUT_FILE);
    }
    else
    {
        Transaction transaction
                (transactionInputWrapper.getNonce(), transactionInputWrapper.getValue(), transactionInputWrapper.getReceiver(),
                 pemFileHandler.getAddress(), transactionInputWrapper.getGasPrice(), transactionInputWrapper.getGasLimit(),
                 transactionInputWrapper.getData(), transactionInputWrapper.getChainId(), transactionInputWrapper.getVersion());

        Signer signer(pemFileHandler.getPrivateKey());
        transaction.applySignature(signer);
        jsonHandler.writeDataToFile(transaction.getSerializedTransaction());
    }
}

void handleRequest(ih::RequestedCmd const requestedCmd)
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
