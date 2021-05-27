#include "address.h"
#include <sodium.h>
#include "../Utils/bites.h"
#include "../Utils/bech32.h"

Address::Address(bytes const& publicKey) :
    m_pk(publicKey),
    m_segWitAddress(){}

Address::Address(std::string segWitAddress) :
    m_pk(),
    m_segWitAddress(segWitAddress){}

std::string Address::getSegWitAddress()  const
{
  return (m_segWitAddress.empty()) ?
  (computeBech32Address()) : (m_segWitAddress);
}

std::string Address::computeBech32Address() const
{
  unsigned char pk[crypto_sign_PUBLICKEYBYTES];
  std::copy(m_pk.begin(), m_pk.end(), pk);

  return util::bech32::encode(hrp, util::convertBits(pk, crypto_sign_PUBLICKEYBYTES,  8, 5, true));
}

bytes Address::getPublicKey() const
{
  return m_pk;
}
