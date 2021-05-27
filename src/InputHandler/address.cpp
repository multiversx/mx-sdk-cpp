#include "address.h"
#include <sodium.h>
#include "../Utils/bites.h"
#include "../Utils/bech32.h"

Address::Address(const std::vector<char> pubKey) :
    m_pubKeyBytes(pubKey),
    m_segWitAddress(){}

Address::Address(std::string segwitAddress) :
    m_pubKeyBytes(),
    m_segWitAddress(segwitAddress){}

std::string Address::getSegWitAddress()  const
{
  std::string ret;

  if (m_segWitAddress.empty())
  {
    ret = computeBech32Address();
  }
  else
  {
    ret = m_segWitAddress;
  }

  return ret;
}

std::string Address::computeBech32Address() const
{
  unsigned char pk[crypto_sign_PUBLICKEYBYTES];
  getPublicKey(pk);

  return util::bech32::encode(hrp, util::convertBits(pk, crypto_sign_PUBLICKEYBYTES,  8, 5, true));
}

void Address::getPublicKey(unsigned char* pk) const
{
  if (m_pubKeyBytes.size() == crypto_sign_PUBLICKEYBYTES)
  {
    std::copy(m_pubKeyBytes.begin(), m_pubKeyBytes.end(), pk);
  }
}
