#include "filehandler/pemreader.h"
#include "cryptosignwrapper.h"
#include "errors.h"
#include "base64.h"
#include "hex.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

PemFileReader::PemFileReader(std::string const &filePath) :
        IFile(filePath, "pem"), ISecretKeyProvider()
{
    try
    {
        std::string const fileContent = getFileContent();
        if (fileContent.empty())
            throw std::invalid_argument(ERROR_MSG_FILE_EMPTY);

        m_fileKeyBytes = getKeyBytesFromContent(fileContent);
        if(m_fileKeyBytes.size() != (SEED_LENGTH + PUBLIC_KEY_LENGTH))
            throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

Address PemFileReader::getAddress() const
{
    bytes const pubKey(m_fileKeyBytes.begin() + SEED_LENGTH, m_fileKeyBytes.end());
    return Address(pubKey);
}

bytes PemFileReader::getSeed() const
{
    return bytes(m_fileKeyBytes.begin(), m_fileKeyBytes.begin() + SEED_LENGTH);
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

