#include "cryptosignwrapper.h"
#include "strchr.h"

#include <sodium.h>

#if \
    (PUBLIC_KEY_BYTES_LENGTH != crypto_sign_PUBLICKEYBYTES) || \
    (SECRET_KEY_BYTES_LENGTH != crypto_sign_SECRETKEYBYTES) || \
    (SEED_BYTES_LENGTH  != crypto_sign_SEEDBYTES) ||           \
    (SIGN_BYTES_LENGTH != crypto_sign_BYTES)
#pragma message "Error. Libsodium library was updated. Update define parameters in the wrapper!"

#else

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message)
{
    auto msg = reinterpret_cast<unsigned char const *>(message.data());
    auto sk = reinterpret_cast<unsigned char const *>(secretKey.data());

    unsigned char sig[SIGN_BYTES_LENGTH];
    unsigned long long sigLength;

    crypto_sign_detached(sig, &sigLength, msg, message.length(), sk);

    return util::uCharToStr(sig, sigLength);
}

bytes getSecretKey(bytes const &seed)
{
    auto sd = reinterpret_cast<const unsigned char*>(seed.data());

    unsigned char pk[PUBLIC_KEY_BYTES_LENGTH];
    unsigned char sk[SECRET_KEY_BYTES_LENGTH];

    crypto_sign_seed_keypair(pk, sk, sd);

    return bytes(sk, sk + SECRET_KEY_BYTES_LENGTH);
}
}
}

#endif
