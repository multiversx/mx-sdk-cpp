#include <cstring>
#include <sodium.h>
#include <stdexcept>

#include "params.h"
#include "strchr.h"
#include "transaction/signer.h"


Signer:: Signer(bytes const &secretKey) :
        m_sk(secretKey)
{
    if (secretKey.size() != crypto_sign_SECRETKEYBYTES)
        throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
}

std::string Signer::getSignature(std::string const &message) const
{
    //TODO: Put this code in a wrapper for libsodium library
    unsigned long long const msgLength = message.length();
    auto *msg = new unsigned char[msgLength + 1ULL];
    strcpy((char *) msg, message.c_str());

    unsigned char sig[crypto_sign_BYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];

    std::copy(m_sk.begin(), m_sk.end(), sk);

    unsigned long long signLength;
    crypto_sign_detached(sig, &signLength, msg, msgLength, sk);
    std::string ret = util::uCharToStr(sig, signLength);

    delete[] msg;
    return ret;
}
