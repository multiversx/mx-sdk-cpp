#ifndef ERD_ACCOUNT_H
#define ERD_ACCOUNT_H

#include "address.h"
#include <string>

class Account
{
public:
    explicit Account(Address address);

    const Address& getAddress() const;

    const std::string& getBalance() const;

private:
    std::string m_balance;
    Address const m_address;
};


#endif //ERD_ACCOUNT_H
