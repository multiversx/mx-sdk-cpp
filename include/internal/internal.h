#ifndef ERD_INTERNAL_H
#define ERD_INTERNAL_H

#include <string>
#include <vector>
#include <cstdint>

#define KEY_FILE_VERSION 4U
#define KEY_FILE_DERIVATION_FUNCTION std::string("scrypt")
#define KEY_FILE_CIPHER_ALGORITHM std::string("aes-128-ctr")

typedef std::vector<uint8_t> bytes;

// See also https://github.com/multiversx/mx-sdk-js-wallet/blob/884226cdd9dc9621aba18218f024d7b4f76b26dc/src/crypto/derivationParams.ts
struct KdfParams
{
    uint32_t dklen = 32;
    uint32_t n = 4096;
    uint32_t r = 8;
    uint32_t p = 1;
    std::string salt;
};

// See also https://github.com/multiversx/mx-sdk-js-wallet/blob/884226cdd9dc9621aba18218f024d7b4f76b26dc/src/crypto/encryptedData.ts
struct EncryptedData
{
    uint32_t version;
    std::string id;
    std::string kdf;
    KdfParams kdfParams;
    std::string iv;
    std::string cipher;
    std::string cipherText;
    std::string mac;
};

#endif //ERD_INTERNAL_H
