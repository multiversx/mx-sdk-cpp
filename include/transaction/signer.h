#ifndef ERD_SIGNER_H
#define ERD_SIGNER_H

#include "internal/internal.h"

class Signer
{
public:
    explicit Signer(bytes const &seed);

    std::string getSignature(std::string const &message) const;

    bool verify(std::string const &signature, std::string const &message) const;

    static bool verify(std::string const &signature, std::string const &message, bytes const &publicKey);

private:
    bytes m_sk;
};


#endif //ERD_SIGNER_H
