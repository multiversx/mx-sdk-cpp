#include "address.h"

Address::Address(const std::vector<char> pubKey) : m_pubKeyBytes(pubKey){}

std::string Address::getSegwitAddress() const
{
  unsigned char pk[crypto_sign_PUBLICKEYBYTES];
  getPublicKey(pk);

  return util::bech32::encode(hrp, util::convertBits(pk, crypto_sign_PUBLICKEYBYTES,  8, 5, true));
}


void Address::getPublicKey(unsigned char* pk) const
{
  std::copy(m_pubKeyBytes.begin(), m_pubKeyBytes.end(), pk);
}