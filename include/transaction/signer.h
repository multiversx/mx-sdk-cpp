#ifndef ERD_SIGNER_H
#define ERD_SIGNER_H

#include "internal/internal.h"

class Signer
{
public:
    Signer(bytes secretKey);

    std::string getSignature(std::string const &message) const;

private:
    bytes m_sk;
};


#endif //ERD_SIGNER_H
