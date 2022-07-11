#ifndef ERD_PROXY_PROVIDER_H
#define ERD_PROXY_PROVIDER_H

#include <map>

#include "data/ext.h"
#include "account/account.h"
#include "account/address.h"
#include "transaction/transaction.h"

class ProxyProvider
{
public:
    explicit ProxyProvider(std::string url);

    Account getAccount(Address const &address);

    std::string send(Transaction const &transaction);

    TransactionStatus getTransactionStatus(std::string const &txHash);

    std::string getESDTTokenBalance(Address const &address, std::string const &token) const;

    std::map<std::string, std::string> getAllESDTTokenBalances(Address const &address) const;

    NetworkConfig getNetworkConfig() const;

private:
    std::string m_url;
};

#endif //ERD_PROXY_PROVIDER_H


