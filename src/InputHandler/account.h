#ifndef ERDCPP_ACCOUNT_H
#define ERDCPP_ACCOUNT_H

#include "address.h"
#include <string>

class Account
{
public:
  Account(Address const&  address);

  const Account& getAccount() const;

  const std::string& getBalance() const;

private:
  std::string m_balance;
  Address const m_address;
};


#endif //ERDCPP_ACCOUNT_H
