#ifndef ERD_ADDRESS_H
#define ERD_ADDRESS_H

#include <string>
#include "internal/internal.h"

class Address
{
    std::string const hrp = "erd";
public:

    Address(bytes const &publicKey);

    Address(std::string const &bech32Address);

    bytes getPublicKey() const;

    std::string getBech32Address() const;

private:

    std::string computeBech32Address() const;

    bytes m_pk;
    std::string m_bech32Address;
};

#endif //ERD_ADDRESS_H
