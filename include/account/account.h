#ifndef ERD_ACCOUNT_H
#define ERD_ACCOUNT_H

#include "address.h"
#include <string>

class Account
{
public:
    explicit Account(Address address);

    explicit Account(Address address, std::string balance, uint64_t const &nonce);

    const Address& getAddress() const;

    const std::string& getBalance() const;

    const uint64_t& getNonce() const;

private:
    uint64_t m_nonce;
    std::string m_balance;
    Address const m_address;
};


#endif //ERD_ACCOUNT_H
