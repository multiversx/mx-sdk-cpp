#include "account/account.h"

#include <utility>

Account::Account(Address address) :
        m_address(std::move(address)),
        m_balance()
{}

const Address& Account::getAddress() const
{
    return m_address;
}

const std::string& Account::getBalance() const
{
    return m_balance;
}

