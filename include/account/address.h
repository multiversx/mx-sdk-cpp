#ifndef MX_ADDRESS_H
#define MX_ADDRESS_H

#include <string>
#include "internal/internal.h"

class Address
{
    std::string const hrp = "erd";

    int const kNoBitsInByte = 8;
    int const kNoBitsInBech32 = 5;

public:

    explicit Address(bytes const &publicKey);

    explicit Address(std::string const &bech32Address);

    Address& operator=(Address const& rhs);

    bool operator==(const Address &address) const;

    bytes getPublicKey() const;

    std::string getBech32Address() const;

private:

    std::string computeBech32Address() const;

    bytes computePkFromBech32() const;

    bytes m_pk;
    std::string m_bech32Address;
};

#endif //MX_ADDRESS_H
