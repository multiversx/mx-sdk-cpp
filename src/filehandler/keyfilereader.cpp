#include "filehandler/keyfilereader.h"
#include "cryptosignwrapper.h"
#include "json/json.hpp"
#include "errors.h"
#include "hex.h"

#include <fstream>
#include <stdexcept>


namespace internal
{
bytes deriveSecretKey(EncryptedData const &data, std::string const &password)
{
    bytes const derivedKey = wrapper::crypto::scrypt(password, data.kdfParams);

    unsigned int const derivedKeyLength = derivedKey.size();

    auto const itKeyBegin = derivedKey.begin();
    auto const itKeyHalf = derivedKey.begin() + derivedKeyLength/2;
    auto const itKeyEnd = derivedKey.end();

    bytes const derivedKeyFirstHalf(itKeyBegin, itKeyHalf);
    bytes const derivedKeySecondHalf(itKeyHalf, itKeyEnd);

    std::string computedMac = wrapper::crypto::hmacsha256(derivedKeySecondHalf, data.cipherText);

    if (computedMac != data.mac)
    {
        throw std::runtime_error(ERROR_MSG_MAC);
    }

   return wrapper::crypto::aes128ctrDecrypt(derivedKeyFirstHalf, data.cipherText, data.iv);
}

template <typename T>
void checkParam(T const &param, T const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + std::to_string(expectedParam) +", got: " + std::to_string(param));
    }
}

template <>
void checkParam<std::string>(std::string const &param, std::string const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + expectedParam +", got: " + param);
    }
}

}

KeyFileReader::KeyFileReader(std::string const &filePath, std::string const &password) :
        IFile(filePath)
{
    try
    {
        KeyFileReader::checkFile();

        auto const data = getFileContent();

        internal::checkParam(data.version, KEY_FILE_VERSION, ERROR_MSG_KEY_FILE_VERSION);
        internal::checkParam(data.cipher, KEY_FILE_CIPHER_ALGORITHM, ERROR_MSG_KEY_FILE_CIPHER);
        internal::checkParam(data.kdf, KEY_FILE_DERIVATION_FUNCTION, ERROR_MSG_KEY_FILE_DERIVATION_FUNCTION);

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
    try
    {
        EncryptedData data;

        std::ifstream stream(IFile::getFilePath());
        auto json = nlohmann::json::parse(stream);

        data.version = json["version"];
        data.kdf = json["crypto"]["kdf"];
        data.cipher = json["crypto"]["cipher"];

        data.kdfParams.dklen = json["crypto"]["kdfparams"]["dklen"];
        data.kdfParams.n = json["crypto"]["kdfparams"]["n"];
        data.kdfParams.r = json["crypto"]["kdfparams"]["r"];
        data.kdfParams.p = json["crypto"]["kdfparams"]["p"];
        data.kdfParams.salt = util::hexToString(json["crypto"]["kdfparams"]["salt"]);

        data.iv = util::hexToString(json["crypto"]["cipherparams"]["iv"]);
        data.cipherText = util::hexToString(json["crypto"]["ciphertext"]);
        data.mac = util::hexToString(json["crypto"]["mac"]);

        return data;
    }
    catch(...)
    {
        throw std::invalid_argument(ERROR_MSG_KEY_FILE);
    }
}

