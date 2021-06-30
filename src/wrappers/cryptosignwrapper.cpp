#include "cryptosignwrapper.h"
#include "strchr.h"

#include <sodium.h>

#if \
    (PUBLIC_KEY_LENGTH != crypto_sign_PUBLICKEYBYTES) || \
    (SECRET_KEY_LENGTH != crypto_sign_SECRETKEYBYTES) || \
    (SEED_LENGTH  != crypto_sign_SEEDBYTES) ||           \
    (SIGNATURE_LENGTH != crypto_sign_BYTES)
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

    unsigned char sig[SIGNATURE_LENGTH];
    unsigned long long sigLength;

    crypto_sign_detached(sig, &sigLength, msg, message.length(), sk);

    return util::uCharToStr(sig, sigLength);
}

bytes getSecretKey(bytes const &seed)
{
    auto sd = reinterpret_cast<const unsigned char*>(seed.data());

    unsigned char pk[PUBLIC_KEY_LENGTH];
    unsigned char sk[SECRET_KEY_LENGTH];

    crypto_sign_seed_keypair(pk, sk, sd);

    return bytes(sk, sk + SECRET_KEY_LENGTH);
}

bytes getPublicKey(bytes const &secretKey)
{
    auto sk = reinterpret_cast<const unsigned char*>(secretKey.data());

    unsigned char pk[PUBLIC_KEY_LENGTH];

    crypto_sign_ed25519_sk_to_pk(pk, sk);

    return bytes(pk, pk + PUBLIC_KEY_LENGTH);
}

bool verify(std::string const &signature, std::string const &message, bytes const &publicKey)
{
    auto sig = reinterpret_cast<const unsigned char*>(signature.data());
    auto msg = reinterpret_cast<const unsigned char*>(message.data());
    auto pk = reinterpret_cast<const unsigned char*>(publicKey.data());
    auto msgLen = message.size();

    int const res = crypto_sign_verify_detached(sig, msg, msgLen, pk);

    return res == 0;
}

}
}

#endif
