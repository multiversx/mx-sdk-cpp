#include <stdexcept>

#include "transaction/signer.h"
#include "cryptosignwrapper.h"
#include "errors.h"

Signer:: Signer(bytes const &seed)
{
    if (seed.size() != SEED_BYTES_LENGTH)
       throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);

    m_sk = wrapper::crypto::getSecretKey(seed);
}

std::string Signer::getSignature(std::string const &message) const
{
    return wrapper::crypto::getSignature(m_sk, message);
}
