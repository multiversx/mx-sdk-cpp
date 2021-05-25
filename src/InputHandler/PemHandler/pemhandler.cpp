#include "pemhandler.h"
#include "../Utils/ext.h"
#include <fstream>
#include <iostream>
#include <sodium.h>

namespace ih
{
  PemFileHandler::PemFileHandler(wrapper::PemHandlerInputWrapper const inputWrapper) :
    IFileHandler(inputWrapper.getPemFilePath()),
    m_inputData(inputWrapper)
  {
    m_fileContent = getFileContent();
  }

  void PemFileHandler::getSeed(unsigned char* seed) const
  {
    std::vector<char> keyBytes = getKeyBytesFromFile();

    std::copy(keyBytes.begin(), keyBytes.begin() + 32, seed);
  }

  void PemFileHandler::getPublicKey(unsigned char* pk) const
  {
    std::vector<char> keyBytes = getKeyBytesFromFile();

    std::copy(keyBytes.begin() + 32, keyBytes.end(), pk);
  }

  void PemFileHandler::getPrivateKey(unsigned char* sk) const
  {
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char seed[crypto_sign_SEEDBYTES];

    getSeed(seed);
    getPublicKey(pk);

    crypto_sign_seed_keypair(pk, sk, seed);
  }

  std::string PemFileHandler::getSegwitAddress() const
  {
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    getPublicKey(pk);

    return util::bech32::encode(hrp, util::convertBits(pk, crypto_sign_PUBLICKEYBYTES,  8, 5, true));
  }

  bool PemFileHandler::isFileValid() const
  {
    return IFileHandler::fileExists() &&
           IFileHandler::isFileExtensionValid("pem") &&
           (m_fileContent != "");
  }

  void PemFileHandler::printFileContent() const
  {
    if (isFileValid())
    {
      std::cerr << "Segwit address: " << getSegwitAddress() << "\n";
    }
  }

  std::vector<char> PemFileHandler::getKeyBytesFromFile() const
  {
    std::string keyHex = util::base64_decode(m_fileContent);

    return util::hexToBytes(keyHex);
  }

  std::string PemFileHandler::getFileContent() const
  {
    std::string line;
    std::string keyLines = "";
    std::ifstream inFile(IFileHandler::getFilePath());

    if (inFile.is_open())
    {
      while (std::getline(inFile, line))
      {
        if (line.substr(0, 5) == "-----") continue;
        keyLines += line;
      }
      inFile.close();
    }

    return keyLines;
  }
}