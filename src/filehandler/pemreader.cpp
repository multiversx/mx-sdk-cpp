#include "filehandler/pemreader.h"
#include "params.h"
#include "base64.h"
#include "hex.h"

#include <fstream>
#include <iostream>
#include <sodium.h>
#include <stdexcept>
#include <iomanip>

namespace ih
{
PemFileReader::PemFileReader(std::string const &filePath) :
        IFile(filePath)
{
    if (PemFileReader::isFileValid())
    {
        std::string const fileContent = getFileContent();
        if (fileContent.empty())
            throw std::invalid_argument(ERROR_MSG_FILE_EMPTY);

        m_fileKeyBytes = getKeyBytesFromContent(fileContent);
        if(m_fileKeyBytes.size() != (crypto_sign_PUBLICKEYBYTES + crypto_sign_SEEDBYTES))
            throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
    }
}

bool PemFileReader::isFileValid() const
{
    bool const fileExists = IFile::fileExists();
    bool const fileExtensionValid = IFile::isFileExtension("pem");

    if (!fileExists) throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST);
    if (!fileExtensionValid) throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);

    return true;
}

Address PemFileReader::getAddress() const
{
    return Address(bytes(m_fileKeyBytes.begin() + crypto_sign_PUBLICKEYBYTES,m_fileKeyBytes.end()));
}

bytes PemFileReader::getSeed() const
{
    return bytes(m_fileKeyBytes.begin(),m_fileKeyBytes.begin() + crypto_sign_PUBLICKEYBYTES);
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

bytes PemFileReader::getKeyBytesFromContent(std::string const &content) const
{
    std::string const keyHex = util::base64::decode(content);
    return util::hexToBytes(keyHex);
}

std::string PemFileReader::getFileContent() const
{
    std::string line;
    std::string keyLines;
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
