#ifndef MX_ACCOUNT_H
#define MX_ACCOUNT_H

#include "address.h"
#include "internal/biguint.h"

#ifndef DEFAULT_NONCE
    #define DEFAULT_NONCE 0U
#endif

#define DEFAULT_BALANCE BigUInt(0)

class Account
{
public:
    explicit Account(Address address);

    explicit Account(Address address, BigUInt balance, uint64_t const &nonce);

    const Address& getAddress() const;

    const BigUInt& getBalance() const;

    const uint64_t& getNonce() const;

    void incrementNonce();

private:
    uint64_t m_nonce;
    BigUInt m_balance;
    Address m_address;
};


#endif //MX_ACCOUNT_H
