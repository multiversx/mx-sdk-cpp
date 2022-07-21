#include "account/address.h"
#include "bits.h"
#include "bech32/bech32.h"
#include "errors.h"

#include <cryptosignwrapper.h>
#include <stdexcept>

Address::Address(bytes const &publicKey) :
        m_pk(publicKey)
{
    if (publicKey.size() != PUBLIC_KEY_LENGTH)
        throw std::length_error(ERROR_MSG_KEY_BYTES_SIZE);

    m_bech32Address = computeBech32Address();
}

Address::Address(std::string const &bech32Address) :
        m_bech32Address(bech32Address)
{
    if (util::bech32::decode(bech32Address).first != hrp)
        throw std::invalid_argument(ERROR_MSG_BECH32);

    m_pk = computePkFromBech32();
}

bool Address::operator==(const Address &address) const
{
    return this->m_pk == address.m_pk;
}

bytes Address::getPublicKey() const
{
    return m_pk;
}

std::string Address::getBech32Address() const
{
    return m_bech32Address;
}

std::string Address::computeBech32Address() const
{
    bytes const pk5BitsPerByte = util::convertBits(m_pk, kNoBitsInByte, kNoBitsInBech32, true);

    return util::bech32::encode(hrp, pk5BitsPerByte);
}

bytes Address::computePkFromBech32() const
{
    bytes const pk5BitsPerByte = util::bech32::decode(m_bech32Address).second;

    return util::convertBits(pk5BitsPerByte, kNoBitsInBech32, kNoBitsInByte, false);
}

Address &Address::operator=(const Address &rhs)
{
    this->m_pk = rhs.getPublicKey();
    this->m_bech32Address = rhs.getBech32Address();

    return *this;
}
