#include "cryptosignwrapper.h"

#include <sodium.h>

#if \
    (PUBLIC_KEY_BYTES != crypto_sign_PUBLICKEYBYTES) || \
    (SECRET_KEY_BYTES != crypto_sign_SECRETKEYBYTES) || \
    (SEED_BYTES  != crypto_sign_SEEDBYTES) ||           \
    (SIGN_BYTES != crypto_sign_BYTES)
#pragma message "Error. Libsodium library was updated. Update define parameters in the wrapper!"

#else

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

bytes getSecretKey(bytes const &publicKey, bytes const &seed)
{
    unsigned char sk[SECRET_KEY_BYTES];
    unsigned char pk[PUBLIC_KEY_BYTES];
    unsigned char sd[SEED_BYTES];

    std::copy(seed.begin(), seed.end(), sd);
    std::copy(publicKey.begin(), publicKey.end(), pk);

    crypto_sign_seed_keypair(pk, sk, sd);

    return bytes(sk, sk + crypto_sign_SECRETKEYBYTES);
}
}
}

#endif
