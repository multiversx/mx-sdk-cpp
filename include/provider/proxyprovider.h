#ifndef ERD_PROXY_PROVIDER_H
#define ERD_PROXY_PROVIDER_H

#include "data/ext.h"
#include "account/account.h"
#include "account/address.h"
#include "transaction/transaction.h"

class ProxyProvider
{
public:
    ProxyProvider(std::string url);

    Account getAccount(Address const &address);

    TransactionHash send(Transaction const &transaction);

    TransactionStatus getTransactionStatus(std::string const &txHash);

private:
    std::string m_url;
};

#endif //ERD_PROXY_PROVIDER_H


