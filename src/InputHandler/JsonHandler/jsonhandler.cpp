#include "jsonhandler.h"
#include <iostream>
#include <fstream>

namespace ih
{
  JsonHandler::JsonHandler(wrapper::PemHandlerInputWrapper const pemInputWrapper,
                           wrapper::JsonHandlerInputWrapper const jsonInputWrapper) :
    IFileHandler(jsonInputWrapper.getOutputFile()),
    m_inputData(jsonInputWrapper),
    m_pemHandler(pemInputWrapper) {}

  bool JsonHandler::isFileValid() const
  {
    return IFileHandler::fileExists() && IFileHandler::isFileExtensionValid("json");
  }

  void JsonHandler::writeOutputFile()
  {
    std::ofstream myfile(m_inputData.getOutputFile());

    if (isFileValid())
    {
      nlohmann::ordered_json transaction = createUnsignedTransaction();
      signTransaction(transaction);
      myfile << transaction.dump();
      myfile.close();
      std::cerr << "Succesfully created transaction!\n";
    }
    else std::cerr << "Unable to open json file\n";
  }

  nlohmann::ordered_json JsonHandler::createUnsignedTransaction()
  {
    nlohmann::ordered_json json;

    json[STR_JSON_NONCE] = m_inputData.getNonce();
    json[STR_JSON_VALUE] = m_inputData.getValue();
    json[STR_JSON_RECEIVER] = m_inputData.getReceiver();
    json[STR_JSON_SENDER] = getSender();
    json[STR_JSON_GAS_PRICE] = m_inputData.getGasPrice();
    json[STR_JSON_GAS_LIMIT] = m_inputData.getGasLimit();
    if (!m_inputData.isDataEmpty())
    {
      json[STR_JSON_DATA] = util::base64_encode(m_inputData.getData());
    }
    json[STR_JSON_CHAIN_ID] = m_inputData.getChainId();
    json[STR_JSON_VERSION] = m_inputData.getVersion();

    return json;
  }

  void JsonHandler::signTransaction(nlohmann::ordered_json& transaction)
  {
    transaction[STR_JSON_SIGNATURE] = getSignature(transaction);
  }

  std::string JsonHandler::getSignature(nlohmann::ordered_json unsignedTransaction)
  {
    std::string const msgString = unsignedTransaction.dump();
    unsigned long long const msgLength = msgString.length();

    unsigned char *msg = new unsigned char[msgLength + 1ULL];
    std::strcpy((char*)msg, msgString.c_str());

    unsigned char sk[crypto_sign_SECRETKEYBYTES];
    unsigned char sig[crypto_sign_BYTES];

    m_pemHandler.getPrivateKey(sk);

    unsigned long long signLength;
    crypto_sign_detached(sig, &signLength, msg, msgLength, sk);
    std::string ret = util::uCharToStr(sig, signLength);

    delete msg;
    return util::stringToHex(ret);
  }

  std::string JsonHandler::getSender()
  {
    return (m_pemHandler.isFileValid()) ? (m_pemHandler.getSegwitAddress()) : ("");
  }
}

