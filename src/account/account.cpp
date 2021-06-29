#include "account/account.h"

#include <utility>

Account::Account(Address address) :
        m_address(std::move(address)),
        m_balance(DEFAULT_BALANCE),
        m_nonce(DEFAULT_NONCE)
{}

Account::Account(Address address, std::string balance, uint64_t const &nonce) :
        m_address(std::move(address)),
        m_balance(std::move(balance)),
        m_nonce(nonce)
{}

const Address& Account::getAddress() const
{
    return m_address;
}

const std::string& Account::getBalance() const
{
    return m_balance;
}

const uint64_t& Account::getNonce() const
{
    return m_nonce;
}

void Account::incrementNonce()
{
    ++m_nonce;
}

