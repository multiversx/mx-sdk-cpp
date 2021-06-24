#include "gtest/gtest.h"

#include "provider/proxyprovider.h"
#include "filehandler/pemreader.h"
#include "utils/errors.h"


TEST(ProxyProvider, getAccount_validProxy)
{
    ProxyProvider proxy("https://testnet-api.elrond.com");

    Address const address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");
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


TEST(ProxyProvider, send_transaction)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Transaction transaction;

    PemFileReader pem("..//..//testData//keysValid1.pem");
    Signer signer(pem.getSeed());

    transaction.m_sender = std::make_shared<Address>(pem.getAddress().getBech32Address());
    transaction.m_receiver = std::make_shared<Address>("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");
    transaction.m_chainID = "T";
    transaction.m_nonce = 8;
    transaction.m_value = "1000000000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;
    transaction.sign(signer);

    std::cerr<< "Tx hash: " << proxy.send(transaction).hash;
}


TEST(ProxyProvider, transactionStatus)
{
    ProxyProvider proxy("https://testnet-api.elrond.com");

    EXPECT_TRUE(proxy.getTransactionStatus("5f0a94b2df847d1b74bd6f8c364602d07be77c3e66bf9b7bb1208715c8eebcb9").isSuccessful());

}
