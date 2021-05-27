#ifndef ERDCPP_ADDRESS_H
#define ERDCPP_ADDRESS_H

#include <string>
#include <vector>

#include "internal.h"

class Address
{
  std::string const hrp = "erd";
public:

  Address(bytes const& publicKey);

  Address(std::string segWitAddress);

  bytes getPublicKey() const;

  std::string getSegWitAddress() const;

private:

  std::string computeBech32Address() const;

  bytes m_pk;
  std::string m_segWitAddress;
};

#endif //ERDCPP_ADDRESS_H
