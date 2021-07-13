#ifndef KEY_FILE_READER_H
#define KEY_FILE_READER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "account/address.h"
#include "internal/internal.h"

struct KdfParams
{
    uint32_t dklen = 32;
    uint32_t n = 4096;
    uint32_t r = 8;
    uint32_t p = 1;
    std::string salt;
};

struct EncryptedData
{
    uint32_t version;
    std::string id;
    std::string address;
    std::string bech32;
    KdfParams kdfparams;
    std::string iv;
    std::string kdf;
    std::string cipher;
    std::string cipherText;
    std::string mac;
};

class KeyFileReader : public IFile
{
public:
    explicit KeyFileReader(std::string const &filePath);

    Address getAddress() const;

    bytes getSeed() const;

protected:
    void checkFile() const override;

private:
    EncryptedData getFileContent() const;

    EncryptedData m_encryptedData;
};


#endif
