#ifndef ERD_WRAPPER_CRYPTO_SIGN_H
#define ERD_WRAPPER_CRYPTO_SIGN_H

#include <sodium.h>
#include <cstring>

#include "strchr.h"

#define PUBLIC_KEY_BYTES crypto_sign_PUBLICKEYBYTES
#define SECRET_KEY_BYTES crypto_sign_SECRETKEYBYTES
#define SEED_BYTES crypto_sign_SEEDBYTES
#define SIGN_BYTES crypto_sign_BYTES

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message)
{
    unsigned long long const msgLength = message.length();
    auto *msg = new unsigned char[msgLength + 1ULL];
    strcpy((char *) msg, message.c_str());

    unsigned char sig[SIGN_BYTES];
    unsigned char sk[SECRET_KEY_BYTES];

    std::copy(secretKey.begin(), secretKey.end(), sk);

    unsigned long long signLength;
    crypto_sign_detached(sig, &signLength, msg, msgLength, sk);
    std::string ret = util::uCharToStr(sig, signLength);

    delete[] msg;
    return ret;
}
}
}

#endif
