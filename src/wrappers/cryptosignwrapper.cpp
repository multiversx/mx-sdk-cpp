#include "cryptosignwrapper.h"
#include "strchr.h"
#include "errors.h"

#include <sodium.h>
#include <stdexcept>
#include "aes_128_ctr/aes.hpp"
#include "keccak/sha3.hpp"

#define CHAR_PTR(x) (const_cast<char *>((x).data()))
#define UCHAR_PTR(x) (reinterpret_cast<unsigned char *>(CHAR_PTR(x)))
#define CONST_UCHAR_PTR(x) reinterpret_cast<unsigned char const *>((x).data())

#if \
    (PUBLIC_KEY_LENGTH != crypto_sign_PUBLICKEYBYTES) || \
    (SECRET_KEY_LENGTH != crypto_sign_SECRETKEYBYTES) || \
    (SEED_LENGTH  != crypto_sign_SEEDBYTES) ||           \
    (SIGNATURE_LENGTH != crypto_sign_BYTES) ||           \
    (HMAC_SHA256_BYTES != crypto_auth_hmacsha256_BYTES)
#pragma message "Error. Libsodium library was updated. Update define parameters in the wrapper!"

#else

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message)
{
    auto msg = CONST_UCHAR_PTR(message);
    auto sk = CONST_UCHAR_PTR(secretKey);

    unsigned char sig[SIGNATURE_LENGTH];
    unsigned long long sigLength;

    crypto_sign_detached(sig, &sigLength, msg, message.length(), sk);

    return util::uCharToStr(sig, sigLength);
}

bytes getSeed(bytes const &secretKey)
{
    auto sk = CONST_UCHAR_PTR(secretKey);

    unsigned char seed[SEED_LENGTH];

    crypto_sign_ed25519_sk_to_seed(seed, sk);

    return bytes(seed, seed + SEED_LENGTH);
}

bytes getSecretKey(bytes const &seed)
{
    auto sd = CONST_UCHAR_PTR(seed);

    unsigned char pk[PUBLIC_KEY_LENGTH];
    unsigned char sk[SECRET_KEY_LENGTH];

    crypto_sign_seed_keypair(pk, sk, sd);

    return bytes(sk, sk + SECRET_KEY_LENGTH);
}

bytes getPublicKey(bytes const &secretKey)
{
    auto sk = CONST_UCHAR_PTR(secretKey);

    unsigned char pk[PUBLIC_KEY_LENGTH];

    crypto_sign_ed25519_sk_to_pk(pk, sk);

    return bytes(pk, pk + PUBLIC_KEY_LENGTH);
}

bool verify(std::string const &signature, std::string const &message, bytes const &publicKey)
{
    auto sig = CONST_UCHAR_PTR(signature);
    auto msg = CONST_UCHAR_PTR(message);
    auto pk = CONST_UCHAR_PTR(publicKey);
    auto msgLen = message.size();

    int const res = crypto_sign_verify_detached(sig, msg, msgLen, pk);

    return res == 0;
}

bytes scrypt(std::string const &password, KdfParams const &kdfParams)
{
    unsigned int const keyLength = kdfParams.dklen;
    unsigned char derivedKey[keyLength];

    auto passw = CONST_UCHAR_PTR(password);
    auto salt = CONST_UCHAR_PTR(kdfParams.salt);

    if (crypto_pwhash_scryptsalsa208sha256_ll
            (passw, password.size(),
             salt, kdfParams.salt.size(),
             kdfParams.n,
             kdfParams.r,
             kdfParams.p,
             derivedKey, keyLength) !=0)
    {
        throw std::runtime_error(ERROR_MSG_SCRYPTSY);
    }

    return bytes(derivedKey, derivedKey + keyLength);
}

std::string hmacsha256(bytes const &key, std::string const &cipherText)
{
    auto k = CONST_UCHAR_PTR(key);
    auto cipher = CONST_UCHAR_PTR(cipherText);

    unsigned char digest[HMAC_SHA256_BYTES];

    crypto_auth_hmacsha256_state state;

    crypto_auth_hmacsha256_init(&state, k, key.size());
    crypto_auth_hmacsha256_update(&state, cipher, cipherText.size());
    crypto_auth_hmacsha256_final(&state, digest);

    return std::string(digest, digest + HMAC_SHA256_BYTES);
}

bytes aes128ctrDecrypt(bytes const &key, std::string cipherText, std::string const &iv)
{
    auto k = CONST_UCHAR_PTR(key);
    auto initVector = CONST_UCHAR_PTR(iv);
    auto cipher = UCHAR_PTR(cipherText);
    auto cipherSize = cipherText.size();

    AES_ctx ctx{};

    AES_init_ctx_iv(&ctx, k, initVector);
    AES_CTR_xcrypt_buffer(&ctx, cipher, cipherSize);

    return bytes(cipher, cipher + cipherSize);
}

std::string sha3Keccak(std::string const &message)
{
    auto msg = UCHAR_PTR(message);

    uint8_t out[SHA3_KECCAK_BYTES];
    sha3(msg, message.size(), out, SHA3_KECCAK_BYTES);

    return std::string(out, out + SHA3_KECCAK_BYTES);
}

}
}

#endif
