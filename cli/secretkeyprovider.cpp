#include "secretkeyprovider.h"

#include "utils/errors.h"
#include "filehandler/pemreader.h"
#include "filehandler/keyfilereader.h"

SecretKeyProvider::SecretKeyProvider(const cxxopts::ParseResult &input)
{
    auto const keyPath = input["key"].as<std::string>();
    auto const fileExtension = IFile::getFileExtension(keyPath);

    if (fileExtension == "pem")
    {
        m_keyFile = std::make_shared<PemFileReader>(keyPath);
    }
    else if (fileExtension == "json")
    {
        auto const password = input["password"].as<std::string>();

        if (password.empty())
        {
            throw std::invalid_argument(ERROR_MSG_MISSING_PASSWORD);
        }
        m_keyFile = std::make_shared<KeyFileReader>(keyPath, password);
    }
    else
    {
        throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);
    }
}

bytes SecretKeyProvider::getSeed() const
{
    return m_keyFile->getSeed();
}

Address SecretKeyProvider::getAddress() const
{
    return m_keyFile->getAddress();
}
