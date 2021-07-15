#include "filehandler/keyfilereader.h"
#include "cryptosignwrapper.h"

#include "errors.h"

#include "hex.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include "json/json.hpp"

namespace internal
{
bytes deriveSecretKey(EncryptedData const &data, std::string const& password)
{
    bytes derivedKey = wrapper::crypto::scryptsy(password, data.kdfparams);
    unsigned int const derivedKeyLength = derivedKey.size();

    bytes derivedKeyFirstHalf(derivedKey.begin(), derivedKey.begin() + derivedKeyLength/2);
    bytes derivedKeySecondHalf(derivedKey.begin() + derivedKeyLength/2, derivedKey.end());

    std::string computedMac = wrapper::crypto::hmacsha256(derivedKeySecondHalf, data.cipherText);

    if (computedMac != data.mac)
    {
        throw std::runtime_error("MAC mismatch, possibly wrong password");
    }

   return wrapper::crypto::aes128ctrDecrypt(derivedKeyFirstHalf, data.cipherText, data.iv);
}
}

KeyFileReader::KeyFileReader(std::string const &filePath, std::string const &password) :
        IFile(filePath)
{
    try
    {
        KeyFileReader::checkFile();

        auto const data = getFileContent();
        m_secretKey = internal::deriveSecretKey(data, password);
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

void KeyFileReader::checkFile() const
{
    if (!IFile::fileExists())
    {
        throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST + getFilePath());
    }
    if (!IFile::isFileExtension("json"))
    {
        throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);
    }
}

Address KeyFileReader::getAddress() const
{
    bytes const publicKey = wrapper::crypto::getPublicKey(m_secretKey);

    return Address(publicKey);
}

bytes KeyFileReader::getSeed() const
{
    return wrapper::crypto::getSeed(m_secretKey);
}

EncryptedData KeyFileReader::getFileContent() const
{
    EncryptedData data;

    try
    {
        std::ifstream stream(IFile::getFilePath());
        auto json = nlohmann::json::parse(stream);

        data.kdfparams.dklen = json["crypto"]["kdfparams"]["dklen"];
        data.kdfparams.n = json["crypto"]["kdfparams"]["n"];
        data.kdfparams.r = json["crypto"]["kdfparams"]["r"];
        data.kdfparams.p = json["crypto"]["kdfparams"]["p"];
        data.kdfparams.salt = util::hexToString(json["crypto"]["kdfparams"]["salt"]);
        data.iv = util::hexToString(json["crypto"]["cipherparams"]["iv"]);
        data.cipherText = util::hexToString(json["crypto"]["ciphertext"]);
        data.mac = util::hexToString(json["crypto"]["mac"]);
    }
    catch(...)
    {
        throw std::invalid_argument(ERROR_MSG_KEY_FILE);
    }

    return data;
}

