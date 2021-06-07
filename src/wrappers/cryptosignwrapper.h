#ifndef ERD_WRAPPER_CRYPTO_SIGN_H
#define ERD_WRAPPER_CRYPTO_SIGN_H

#include <cstring>

#include "internal/internal.h"
#include "strchr.h"

#define PUBLIC_KEY_BYTES_LENGTH 32U
#define SECRET_KEY_BYTES_LENGTH 64U
#define SEED_BYTES_LENGTH 32U
#define SIGN_BYTES_LENGTH 64U

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message);

bytes getSecretKey(bytes publicKey, bytes const &seed);
}
}

#endif
