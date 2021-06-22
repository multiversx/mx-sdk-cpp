#include "gtest/gtest.h"

#include "proxy/proxyprovider.h"

TEST(ProxyProvider, getAccount_validProxy)
{
    ProxyProvider proxy("https://api.elrond.com");

    Address const address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");
    Account const account = proxy.getAccount(address);

    EXPECT_FALSE(account.getAddress().getBech32Address().empty());
    EXPECT_FALSE(account.getBalance().empty());
}

TEST(ProxyProvider, getAccount_invalidProxy)
{
    ProxyProvider proxy("https://api.elronddd.com");
    Address address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");

    EXPECT_THROW(proxy.getAccount(address),std::runtime_error);
}