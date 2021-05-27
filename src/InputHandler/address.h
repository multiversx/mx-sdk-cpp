#ifndef ERDCPP_ADDRESS_H
#define ERDCPP_ADDRESS_H

#include <string>
#include <vector>

class Address
{
  std::string const hrp = "erd";
public:

  Address(std::vector<char> const pubKey);

  Address(std::string segwitAddress);

  std::string getSegWitAddress() const;

  void getPublicKey(unsigned char* pk) const;

private:

  std::string computeBech32Address() const;

  std::vector<char> m_pubKeyBytes;
  std::string m_segWitAddress;
};

#endif //ERDCPP_ADDRESS_H
