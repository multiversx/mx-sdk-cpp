#include "pemhandler.h"
#include "utils/base64.h"
#include "utils/hex.h"

#include <fstream>
#include <iostream>
#include <sodium.h>

namespace ih
{
PemInputHandler::PemInputHandler(wrapper::PemHandlerInputWrapper const inputWrapper) :
        IFileHandler(inputWrapper.getPemFilePath()),
        m_inputData(inputWrapper)
{
    m_fileContent = getFileContent();
}

bool PemInputHandler::isFileValid() const
{
    return IFileHandler::fileExists() &&
           IFileHandler::isFileExtensionValid("pem") &&
           (m_fileContent != "");
}

Address PemInputHandler::getAddress() const
{
    bytes keyBytes = getKeyBytesFromFile();
    keyBytes.erase(keyBytes.begin(), keyBytes.begin() + 32);

    return Address(keyBytes);
}

bytes PemInputHandler::getSeed() const
{
    bytes keyBytes = getKeyBytesFromFile();
    keyBytes.erase(keyBytes.begin() + 32, keyBytes.end());

    return keyBytes;
}

bytes PemInputHandler::getPrivateKey() const
{
    unsigned char sk[crypto_sign_SECRETKEYBYTES];
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char seed[crypto_sign_SEEDBYTES];

    bytes seedBytes = getSeed();
    bytes pkBytes = getAddress().getPublicKey();

    std::copy(seedBytes.begin(), seedBytes.end(), seed);
    std::copy(pkBytes.begin(), pkBytes.end(), pk);

    crypto_sign_seed_keypair(pk, sk, seed);

    return bytes(sk, sk + crypto_sign_SECRETKEYBYTES);
}

void PemInputHandler::printFileContent() const
{
    if (isFileValid())
    {
        std::cerr << "Segwit address: " << getAddress().getBech32Address() << "\n";
    }
}

bytes PemInputHandler::getKeyBytesFromFile() const
{
    std::string keyHex = util::base64::decode(m_fileContent);

    return util::hexToBytes(keyHex);
}

std::string PemInputHandler::getFileContent() const
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
