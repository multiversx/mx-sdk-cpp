#ifndef ERD_WRAPPER_CRYPTO_SIGN_H
#define ERD_WRAPPER_CRYPTO_SIGN_H

#include <string>

#include "internal/internal.h"

#define PUBLIC_KEY_LENGTH 32U
#define SECRET_KEY_LENGTH 64U
#define SEED_LENGTH 32U
#define SIGNATURE_LENGTH 64U
#define HMAC_SHA256_BYTES 32U

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message);

bytes getSeed(bytes const &secretKey);

bytes getSecretKey(bytes const &seed);

bytes getPublicKey(bytes const &secretKey);

bool verify(std::string const &signature, std::string const &message, bytes const &publicKey);

bytes scrypt(std::string const &password, KdfParams const &kdfParams);

std::string hmacsha256(bytes const &key, std::string const &cipherText);

bytes aes128ctrDecrypt(bytes const &key, std::string const &cipherText, std::string const &iv);
}
}

#endif
