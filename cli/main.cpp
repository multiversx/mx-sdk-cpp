#include <string>
#include <iostream>
#include <sodium.h>

#include "inputhandler/ext.h"

void reportError(errorCode const err);

int main(int argc, char *argv[])
{
    if (sodium_init() < 0)
    {
        reportError(ERROR_SODIUM_INIT);
        return 0;
    }

    ih::ArgHandler argHandler(argc, argv);

    ih::RequestedCmd const requestedCmd = argHandler.getRequestedCmd();
    errorCode const errCode = requestedCmd.getErrorCode();

    if (errCode != ERROR_NONE)
    {
        reportError(errCode);
        return 0;
    }

    ih::RequestType const reqType = requestedCmd.getRequestType();

    switch (reqType)
    {
        case ih::help:
        {
            argHandler.showInfo();
        }
        case ih::loadPemFile:
        {
            ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(requestedCmd.getUserInputs());
            ih::PemInputHandler pemHandler(pemInputWrapper);

            if (pemHandler.isFileValid())
            {
                pemHandler.printFileContent();
            }
            else
            {
                reportError(ERROR_PEM_INPUT_FILE);
            }
            break;
        }
        case ih::createSignedTransactionWithPemFile:
        {
            ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(requestedCmd.getUserInputs());
            ih::wrapper::TransactionInputWrapper const transactionInputWrapper(requestedCmd.getUserInputs());

            ih::JsonFileHandler jsonHandler(transactionInputWrapper.getOutputFile());
            ih::PemInputHandler pemFileHandler(pemInputWrapper);

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
            break;
        }
        default:
        {
            argHandler.showInfo();
        }
    }

    return 0;
}

void reportError(errorCode const err)
{
    std::cerr << "Error. ";

    if (errors.find(err) != errors.end()) std::cerr << errors.at(err) << "\n";
}

