#ifndef ERDCPP_SIGNER_H
#define ERDCPP_SIGNER_H

class Signer
{
public:
  Signer(unsigned char* secretKey);

  std::string getSignature(std::string const& message);

private:
  unsigned char* m_sk;
};


#endif //ERDCPP_SIGNER_H
