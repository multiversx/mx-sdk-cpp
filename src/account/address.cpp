#include "account/address.h"
#include "bits.h"
#include "bech32/bech32.h"
#include "params.h"

#include <sodium.h>
#include <stdexcept>

Address::Address(bytes const &publicKey) :
        m_pk(publicKey),
        m_bech32Address()
{
    if (publicKey.size() != crypto_sign_PUBLICKEYBYTES)
        throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);
}

Address::Address(std::string const &bech32Address) :
        m_pk(),
        m_bech32Address(bech32Address)
{
    if (util::bech32::decode(bech32Address).first != hrp)
        throw std::invalid_argument(ERROR_MSG_BECH32);
}

bytes Address::getPublicKey() const
{
    return (m_pk.empty()) ?
        (computePkFromBech32()) :(m_pk);
}

std::string Address::getBech32Address() const
{
    return (m_bech32Address.empty()) ?
        (computeBech32Address()) : (m_bech32Address);
}

std::string Address::computeBech32Address() const
{
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    std::copy(m_pk.begin(), m_pk.end(), pk);

    return util::bech32::encode(hrp, util::convertBits(pk, crypto_sign_PUBLICKEYBYTES, kNoBitsInByte, kNoBitsInBech32, true));
}

bytes Address::computePkFromBech32() const
{
    bytes pk5BitsPerByte = util::bech32::decode(m_bech32Address).second;
    unsigned int const pk5BitsSize = pk5BitsPerByte.size();

    auto pkCharPtr = new unsigned char[pk5BitsSize];
    std::copy(pk5BitsPerByte.begin(), pk5BitsPerByte.end(), pkCharPtr);

    bytes ret = util::convertBits(pkCharPtr, pk5BitsSize, kNoBitsInBech32, kNoBitsInByte, false);
    delete[] pkCharPtr;
    return ret;
}

