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

  Address PemFileHandler::getAddress() const
  {
    std::vector<char> keyBytes = getKeyBytesFromFile();

    keyBytes.erase(keyBytes.begin(),keyBytes.begin()+32);

    return Address(keyBytes);
  }

  void PemFileHandler::getSeed(unsigned char* seed) const
  {
    std::vector<char> keyBytes = getKeyBytesFromFile();

    std::copy(keyBytes.begin(), keyBytes.begin() + 32, seed);
  }

  void PemFileHandler::getPrivateKey(unsigned char* sk) const
  {
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char seed[crypto_sign_SEEDBYTES];

    getSeed(seed);
    getAddress().getPublicKey(pk);

    crypto_sign_seed_keypair(pk, sk, seed);
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
      std::cerr << "Segwit address: " << getAddress().getSegwitAddress() << "\n";
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