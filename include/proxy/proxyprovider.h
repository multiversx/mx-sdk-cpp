#ifndef ERD_PROXY_PROVIDER_H
#define ERD_PROXY_PROVIDER_H

#include "data/ext.h"
#include "account/account.h"
#include "account/address.h"
#include "transaction/transaction.h"

#include <string>

class ProxyProvider
{
public:
    ProxyProvider(std::string url);

    Account getAccount(Address const &address);

    std::string send(Transaction const &transaction);

    bool transactionExecuted(std::string const &txHash);

private:
    std::string m_url;
};

#endif //ERD_API_H


