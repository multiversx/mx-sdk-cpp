#include <string>
#include <iostream>
#include <sodium.h>
#include <vector>
#include "InputHandler/ext.h"

void reportError(errorCode const err);

int main(int argc, char* argv[])
{
  if (sodium_init() < 0)
  {
    reportError(ERROR_SODIUM_INIT);
    return 0;
  }

  ih::ArgHandler argHandler(argc, argv);

  ih::RequestedCmd const reqestedCmd = argHandler.getRequestedCmd();
  errorCode const errCode = reqestedCmd.getErrorCode();

  if (errCode != ERROR_NONE)
  {
    reportError(errCode);
    return 0;
  }

  ih::RequestType const reqType = reqestedCmd.getRequestType();

  switch (reqType)
  {
  case ih::help:
  {
    argHandler.showInfo();
  }
  case ih::loadPemFile:
  {
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(reqestedCmd.getUserInputs());
    ih::PemFileHandler pemHandler(pemInputWrapper);

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
    ih::wrapper::PemHandlerInputWrapper const pemInputWrapper(reqestedCmd.getUserInputs());
    ih::wrapper::JsonHandlerInputWrapper const jsonInputWrapper(reqestedCmd.getUserInputs());

    ih::JsonFileHandler jsonFileHandler(jsonInputWrapper);
    ih::PemFileHandler pemFileHandler(pemInputWrapper);

    if (jsonFileHandler.isFileValid())
    {
      Transaction transaction
      (jsonInputWrapper.getNonce(), jsonInputWrapper.getValue(),jsonInputWrapper.getReceiver(),
       pemFileHandler.getAddress(), jsonInputWrapper.getGasPrice(), jsonInputWrapper.getGasLimit(),
       jsonInputWrapper.getData(), jsonInputWrapper.getChainId(), jsonInputWrapper.getVersion());

      Signer signer(pemFileHandler.getPrivateKey());

      transaction.applySignature(signer);

      jsonFileHandler.writeOutputFile(transaction);
    }
    else
    {
      reportError(ERROR_JSON_OUT_FILE);
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

