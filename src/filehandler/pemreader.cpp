#include "filehandler/pemreader.h"
#include "base64.h"
#include "hex.h"

#include <fstream>
#include <iostream>
#include <sodium.h>

namespace ih
{
PemFileReader::PemFileReader(std::string const filePath) :
        IFile(filePath)
{
    m_fileContent = getFileContent();
}

bool PemFileReader::isFileValid() const
{
    return IFile::fileExists() &&
           IFile::isFileExtension("pem") &&
           (m_fileContent != "");
}

Address PemFileReader::getAddress() const
{
    bytes keyBytes = getKeyBytesFromFile();
    keyBytes.erase(keyBytes.begin(), keyBytes.begin() + crypto_sign_PUBLICKEYBYTES);

    return Address(keyBytes);
}

bytes PemFileReader::getSeed() const
{
    bytes keyBytes = getKeyBytesFromFile();
    keyBytes.erase(keyBytes.begin() + crypto_sign_PUBLICKEYBYTES, keyBytes.end());

    return keyBytes;
}

bytes PemFileReader::getPrivateKey() const
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

bytes PemFileReader::getKeyBytesFromFile() const
{
    std::string keyHex = util::base64::decode(m_fileContent);

    return util::hexToBytes(keyHex);
}

std::string PemFileReader::getFileContent() const
{
    std::string line;
    std::string keyLines = "";
    std::ifstream inFile(IFile::getFilePath());

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
