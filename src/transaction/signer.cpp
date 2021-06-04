#include <sodium.h>
#include <stdexcept>

#include "transaction/signer.h"
#include "cryptosignwrapper.h"
#include "params.h"



Signer:: Signer(bytes const &secretKey) :
        m_sk(secretKey)
{
    if (secretKey.size() != crypto_sign_SECRETKEYBYTES)
        throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
}

std::string Signer::getSignature(std::string const &message) const
{
    return wrapper::crypto::getSignature(m_sk, message);
}
