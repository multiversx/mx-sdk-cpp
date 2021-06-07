#include "filehandler/pemreader.h"
#include "cryptosignwrapper.h"
#include "errors.h"
#include "base64.h"
#include "hex.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace ih
{
PemFileReader::PemFileReader(std::string const &filePath) :
        IFile(filePath)
{
    try
    {
        PemFileReader::checkFile();

        std::string const fileContent = getFileContent();
        if (fileContent.empty())
            throw std::invalid_argument(ERROR_MSG_FILE_EMPTY);

        m_fileKeyBytes = getKeyBytesFromContent(fileContent);
        if(m_fileKeyBytes.size() != (PUBLIC_KEY_BYTES_LENGTH + SEED_BYTES_LENGTH))
            throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

bool PemFileReader::checkFile() const
{
    if (!IFile::fileExists()) throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST);
    if (!IFile::isFileExtension("pem")) throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);

    return true;
}

Address PemFileReader::getAddress() const
{
    return Address(bytes(m_fileKeyBytes.begin() + PUBLIC_KEY_BYTES_LENGTH, m_fileKeyBytes.end()));
}

bytes PemFileReader::getSeed() const
{
    return bytes(m_fileKeyBytes.begin(), m_fileKeyBytes.begin() + PUBLIC_KEY_BYTES_LENGTH);
}

bytes PemFileReader::getPrivateKey() const
{
    bytes const seedBytes = getSeed();
    bytes const pkBytes = getAddress().getPublicKey();

    return wrapper::crypto::getSecretKey(pkBytes,seedBytes);
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
