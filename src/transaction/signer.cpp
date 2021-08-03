#include <stdexcept>

#include "transaction/signer.h"
#include "cryptosignwrapper.h"
#include "errors.h"

Signer::Signer(bytes const &seed)
{
    if (seed.size() != SEED_LENGTH)
       throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);

    m_sk = wrapper::crypto::getSecretKey(seed);
}

std::string Signer::getSignature(std::string const &message) const
{
    return wrapper::crypto::getSignature(m_sk, message);
}

bool Signer::verify(std::string const &signature, std::string const &message) const
{
    bytes const publicKey = wrapper::crypto::getPublicKey(m_sk);

    return wrapper::crypto::verify(signature, message, publicKey);
}

bool Signer::verify(std::string const &signature, std::string const &message, bytes const &publicKey)
{
    return wrapper::crypto::verify(signature, message, publicKey);
}

bool Signer::verify(const std::string &signature, const std::string &message, const Address &address)
{
    bytes const publicKey = address.getPublicKey();

    return wrapper::crypto::verify(signature, message, publicKey);
}
