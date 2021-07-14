#ifndef ERD_WRAPPER_CRYPTO_SIGN_H
#define ERD_WRAPPER_CRYPTO_SIGN_H

#include <string>

#include "internal/internal.h"

#define PUBLIC_KEY_LENGTH 32U
#define SECRET_KEY_LENGTH 64U
#define SEED_LENGTH 32U
#define SIGNATURE_LENGTH 64U

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message);

bytes getSecretKey(bytes const &seed);

bytes getPublicKey(bytes const &secretKey);

bool verify(std::string const &signature, std::string const &message, bytes const &publicKey);

bytes scryptsy(std::string const &password, KdfParams const &kdfParams);
}
}

#endif
