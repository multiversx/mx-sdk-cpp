#ifndef ERD_PROXY_PROVIDER_H
#define ERD_PROXY_PROVIDER_H

#include "account/account.h"
#include "account/address.h"

#include <string>

class ProxyProvider
{
public:
    ProxyProvider(std::string url);

    Account getAccount(Address const &address);

private:
    std::string m_url;
};

#endif //ERD_API_H


