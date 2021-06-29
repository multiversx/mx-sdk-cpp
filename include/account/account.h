#ifndef ERD_ACCOUNT_H
#define ERD_ACCOUNT_H

#include "address.h"
#include <string>

#ifndef DEFAULT_NONCE
    #define DEFAULT_NONCE 0U
#endif

#define DEFAULT_BALANCE "0"

class Account
{
public:
    explicit Account(Address address);

    explicit Account(Address address, std::string balance, uint64_t const &nonce);

    const Address& getAddress() const;

    const std::string& getBalance() const;

    const uint64_t& getNonce() const;

    void incrementNonce();

private:
    uint64_t m_nonce;
    std::string m_balance;
    Address const m_address;
};


#endif //ERD_ACCOUNT_H
