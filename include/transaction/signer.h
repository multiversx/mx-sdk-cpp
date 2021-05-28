#ifndef ERDCPP_SIGNER_H
#define ERDCPP_SIGNER_H

#include "internal/internal.h"

class Signer
{
public:
  Signer(bytes secretKey);

  std::string getSignature(std::string const& message) const;

private:
  bytes m_sk;
};


#endif //ERDCPP_SIGNER_H
