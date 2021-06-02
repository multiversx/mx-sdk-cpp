#include <string>
#include <iostream>
#include <sodium.h>

#include "inputhandler/ext.h"
#include "erdsdk.h"

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
            try
            {
                ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(requestedCmd.getUserInputs());
                ih::PemFileReader pemHandler(pemInputWrapper.getPemFilePath());
                std::cerr << "Bech32 address: " << pemHandler.getAddress().getBech32Address() << "\n";
            }
            catch (std::exception exception)
            {
                std::cerr<< exception.what();
            }
            break;
        }
        case ih::createSignedTransactionWithPemFile:
        {
            ih::wrapper::TransactionInputWrapper const transactionInputWrapper(requestedCmd.getUserInputs());
            ih::TransactionInputHandler transactionHandler(transactionInputWrapper);

            if (!transactionHandler.isFileValid())
            {
                reportError(ERROR_JSON_OUT_FILE);
            }
            else
            {
                try
                {
                    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(requestedCmd.getUserInputs());
                    ih::PemFileReader pemFileHandler(pemInputWrapper.getPemFilePath());

                    Transaction transaction
                            (transactionInputWrapper.getNonce(), transactionInputWrapper.getValue(), transactionInputWrapper.getReceiver(),
                             pemFileHandler.getAddress(), transactionInputWrapper.getGasPrice(), transactionInputWrapper.getGasLimit(),
                             transactionInputWrapper.getData(), transactionInputWrapper.getChainId(), transactionInputWrapper.getVersion());

                    Signer signer(pemFileHandler.getPrivateKey());
                    transaction.applySignature(signer);
                    transactionHandler.writeTransactionToJsonFile(transaction);
                }
                catch (std::exception exception)
                {
                    std::cerr<<exception.what();
                }

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

