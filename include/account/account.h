#ifndef ERD_ACCOUNT_H
#define ERD_ACCOUNT_H

#include "address.h"
#include <string>

class Account
{
public:
    Account(Address const &address);

    const Account getAddress() const;

    const std::string getBalance() const;

private:
    std::string m_balance;
    Address const m_address;
};


#endif //ERD_ACCOUNT_H
