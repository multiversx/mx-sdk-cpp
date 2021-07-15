#ifndef KEY_FILE_READER_H
#define KEY_FILE_READER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "account/address.h"
#include "internal/internal.h"

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
    explicit KeyFileReader(std::string const &filePath, std::string const &password);

    Address getAddress() const;

    bytes getSeed() const;

protected:
    void checkFile() const override;

private:
    EncryptedData getFileContent() const;

    bytes m_secretKey;
};


#endif
