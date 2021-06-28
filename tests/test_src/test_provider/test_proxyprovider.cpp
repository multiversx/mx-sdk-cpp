#include "gtest/gtest.h"

#include "provider/proxyprovider.h"
#include "filehandler/pemreader.h"
#include "utils/errors.h"


TEST(ProxyProvider, getAccount)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");

    Address const address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    Account const account = proxy.getAccount(address);

    EXPECT_FALSE(account.getAddress().getBech32Address().empty());
    EXPECT_FALSE(account.getBalance().empty());
    EXPECT_FALSE(account.getBalance() == DEFAULT_BALANCE);
    EXPECT_FALSE(account.getNonce() == 0);
}

TEST(ProxyProvider, send_validTx)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");

    PemFileReader pem("..//..//testData//keysValid1.pem");
    Address sender(pem.getAddress());
    Account senderAcc = proxy.getAccount(sender);

    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(sender);
    transaction.m_receiver = std::make_shared<Address>("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    transaction.m_chainID = "T";
    transaction.m_nonce = senderAcc.getNonce() + 1;
    transaction.m_value = "10000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;

    Signer signer(pem.getSeed());
    transaction.sign(signer);

    std::string const txHash = proxy.send(transaction).hash;
    EXPECT_FALSE(txHash.empty());

    TransactionStatus const txStatus = proxy.getTransactionStatus(txHash);
    EXPECT_TRUE (txStatus.isPending() || txStatus.isExecuted());
}

TEST(ProxyProvider, send_invalidTx_noSignature)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");

    PemFileReader pem("..//..//testData//keysValid1.pem");
    Address sender(pem.getAddress());
    Account senderAcc = proxy.getAccount(sender);

    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(sender);
    transaction.m_receiver = std::make_shared<Address>("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    transaction.m_chainID = "T";
    transaction.m_nonce = senderAcc.getNonce() + 1;
    transaction.m_value = "10000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;

    EXPECT_THROW({
                     try
                     {
                         proxy.send(transaction);
                     }
                     catch(const std::runtime_error &e)
                     {
                         std::string const errDescription = e.what();

                         EXPECT_TRUE(errDescription.find(ERROR_MSG_HTTP_REQUEST_FAILED) != std::string::npos);
                         EXPECT_TRUE(errDescription.find(ERROR_MSG_REASON) != std::string::npos);
                         EXPECT_TRUE(errDescription.find("signature") != std::string::npos);
                         throw;
                     }
                 }, std::runtime_error );
}


TEST(ProxyProvider, getTransactionStatus_validHash)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");

    TransactionStatus const txStatus = proxy.getTransactionStatus("5f0a94b2df847d1b74bd6f8c364602d07be77c3e66bf9b7bb1208715c8eebcb9");
    EXPECT_TRUE(txStatus.isExecuted());
}

TEST(ProxyProvider, getTransactionStatus_invalidHash)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");

    EXPECT_THROW({
                     try
                     {
                         proxy.getTransactionStatus("test");
                     }
                     catch(const std::runtime_error &e)
                     {
                         std::string const errDescription = e.what();

                         EXPECT_TRUE(errDescription.find(ERROR_MSG_HTTP_REQUEST_FAILED) != std::string::npos);
                         EXPECT_TRUE(errDescription.find(ERROR_MSG_REASON) != std::string::npos);
                         EXPECT_TRUE(errDescription.find("not found") != std::string::npos);
                         throw;
                     }
                 }, std::runtime_error );
}

TEST(ProxyProvider, getESDTokenBalance)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Address const address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    auto const balance = proxy.getESDTTokenBalance(address, "this esdt does not exist");

    EXPECT_TRUE(balance == DEFAULT_BALANCE);
}

TEST(ProxyProvider, getAllESDTokenBalances_noTokens)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Address const address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    auto const esdts = proxy.getAllESDTTokenBalances(address);

    EXPECT_TRUE(esdts.empty());
}

TEST(ProxyProvider, getAllESDTokenBalances_multipleTokens)
{
    ProxyProvider proxy("https://testnet-gateway.elrond.com");
    Address const address("erd1nqtv8gdsf55xj9eg0wc8ar6ml46kpld2c86aq670mxgcf49sduzqaeyngx");
    auto const esdts = proxy.getAllESDTTokenBalances(address);

    EXPECT_TRUE(esdts.find("00040-4c4d18") != esdts.end());
    EXPECT_FALSE(esdts.at("00040-4c4d18").empty());

    EXPECT_TRUE(esdts.find("0009O-8742a4") != esdts.end());
    EXPECT_FALSE(esdts.at("0009O-8742a4").empty());
}