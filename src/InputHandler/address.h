#ifndef ERDCPP_ADDRESS_H
#define ERDCPP_ADDRESS_H

#include <string.h>
#include <sodium.h>
#include "../Utils/bech32.h"
#include "../Utils/bites.h"

class Address
{
  std::string const hrp = "erd";
public:

  Address(std::vector<char> const pubKey);

  void getPublicKey(unsigned char* pk) const;

  std::string getSegwitAddress() const;

private:
  std::vector<char> m_pubKeyBytes;
};

#endif //ERDCPP_ADDRESS_H
