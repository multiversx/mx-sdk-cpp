#ifndef ERD_SIGNER_H
#define ERD_SIGNER_H

#include "internal/internal.h"
#include "account/address.h"

class Signer
{
public:
    explicit Signer(bytes const &seed);

    std::string getSignature(std::string const &message) const;

    bool verify(std::string const &signature, std::string const &message) const;

    static bool verify(std::string const &signature, std::string const &message, bytes const &publicKey);

    static bool verify(std::string const &signature, std::string const &message, Address const &address);

private:
    bytes m_sk;
};


#endif //ERD_SIGNER_H
