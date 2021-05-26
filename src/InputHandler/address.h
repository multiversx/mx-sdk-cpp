#ifndef ERDCPP_ADDRESS_H
#define ERDCPP_ADDRESS_H

#include <string>
#include <vector>

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
