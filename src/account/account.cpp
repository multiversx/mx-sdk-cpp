#include "account/account.h"

Account::Account(Address const &address) :
        m_address(address),
        m_balance()
{}

const Account Account::getAddress() const
{
    return m_address;
}

const std::string Account::getBalance() const
{
    return m_balance;
}

