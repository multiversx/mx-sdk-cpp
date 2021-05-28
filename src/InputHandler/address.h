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

  Address(std::string bech32Address);

  bytes getPublicKey() const;

  std::string getBech32Address() const;

private:

  std::string computeBech32Address() const;

  bytes m_pk;
  std::string m_bech32Address;
};

#endif //ERDCPP_ADDRESS_H
