#include <utility>

#include "gtest/gtest.h"

#include "transaction/transaction_factory.h"
#include "provider/proxyprovider.h"
#include "filehandler/pemreader.h"
#include "utils/errors.h"
#include "utils/cfg.h"

#if GTESTS_HTTPS_PRECONDITIONS

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
                     catch (const std::runtime_error &e)
                     {
                         std::string const errDescription = e.what();

                         EXPECT_TRUE(errDescription.find(ERROR_MSG_HTTP_REQUEST_FAILED) != std::string::npos);
                         EXPECT_TRUE(errDescription.find(ERROR_MSG_REASON) != std::string::npos);
                         EXPECT_TRUE(errDescription.find("not found") != std::string::npos);
                         throw;
                     }
                 }, std::runtime_error);
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

void EXPECT_NETWORK_CONFIG_EQ(const NetworkConfig &cfg1, const NetworkConfig &cfg2)
{
    EXPECT_EQ(cfg1.chainId, cfg2.chainId);
    EXPECT_EQ(cfg1.gasPerDataByte, cfg2.gasPerDataByte);
    EXPECT_EQ(cfg1.minGasPrice, cfg2.minGasPrice);
    EXPECT_EQ(cfg1.minGasLimit, cfg2.minGasLimit);

    EXPECT_EQ(cfg1, cfg2);
}

TEST(ProxyProvider, getNetworkConfig)
{
    ProxyProvider proxyTestnet("https://testnet-gateway.elrond.com");
    NetworkConfig defaultTestnet = DEFAULT_TESTNET_NETWORK_CONFIG;
    EXPECT_NETWORK_CONFIG_EQ(proxyTestnet.getNetworkConfig(), defaultTestnet);

    ProxyProvider proxyMainnet("https://api.elrond.com");
    NetworkConfig defaultMainnet = DEFAULT_MAINNET_NETWORK_CONFIG;
    EXPECT_NETWORK_CONFIG_EQ(proxyMainnet.getNetworkConfig(), defaultMainnet);
}

class GenericProxyProviderTxFixture : public ::testing::Test
{
public:
    explicit GenericProxyProviderTxFixture(std::string proxyUrl) :
            m_proxy(std::move(proxyUrl)),
            m_pem("..//..//testData//keysValid1.pem"),
            m_senderAddr(m_pem.getAddress()),
            m_senderAcc(m_proxy.getAccount(m_senderAddr)),
            m_txFactory({"",0,0,0})
    {
        m_networkConfig = m_proxy.getNetworkConfig();
        m_txFactory = TransactionFactory(m_networkConfig);
    }

    void signTransaction(Transaction &transaction) const
    {
        Signer const signer(m_pem.getSeed());
        transaction.sign(signer);
    }

    void EXPECT_TRANSACTION_SENT_SUCCESSFULLY(Transaction const &transaction)
    {
        std::string const txHash = m_proxy.send(transaction).hash;
        EXPECT_FALSE(txHash.empty());

        TransactionStatus const txStatus = m_proxy.getTransactionStatus(txHash);
        EXPECT_TRUE(txStatus.isPending() || txStatus.isExecuted());
    }

    TransactionFactory m_txFactory;
    NetworkConfig m_networkConfig;
    ProxyProvider m_proxy;
    PemFileReader m_pem;
    Address m_senderAddr;
    Account m_senderAcc;
};


class TestnetProxyProviderTxFixture : public GenericProxyProviderTxFixture
{
public:
    TestnetProxyProviderTxFixture() :
            GenericProxyProviderTxFixture("https://testnet-gateway.elrond.com")
    {}
};

class DevnetProxyProviderTxFixture : public GenericProxyProviderTxFixture
{
public:
    DevnetProxyProviderTxFixture() :
            GenericProxyProviderTxFixture("https://devnet-gateway.elrond.com")
    {}
};

TEST_F(TestnetProxyProviderTxFixture, send_validTx)
{
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    transaction.m_chainID = "T";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = "10000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;

    signTransaction(transaction);
    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(TestnetProxyProviderTxFixture, send_validTx_signedHashedTx)
{
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transaction.m_chainID = "T";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = "1000000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;
    transaction.m_options = std::make_shared<uint32_t>(1U);
    transaction.m_version = 2U;

    signTransaction(transaction);
    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(TestnetProxyProviderTxFixture, send_invalidTx_noSignature)
{
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    transaction.m_chainID = "T";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = "10000000000";
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;

    EXPECT_THROW({
                     try
                     {
                         m_proxy.send(transaction);
                     }
                     catch (const std::runtime_error &e)
                     {
                         std::string const errDescription = e.what();

                         EXPECT_TRUE(errDescription.find(ERROR_MSG_HTTP_REQUEST_FAILED) != std::string::npos);
                         EXPECT_TRUE(errDescription.find(ERROR_MSG_REASON) != std::string::npos);
                         EXPECT_TRUE(errDescription.find("signature") != std::string::npos);
                         throw;
                     }
                 }, std::runtime_error);
}

TEST_F(TestnetProxyProviderTxFixture, send_issueESDTTransaction_noESDTProperties)
{
    Transaction transaction = m_txFactory.createESDTIssue(
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    1000000000,
                    "JonDoe",
                    "JDO",
                    BigUInt(444000),
                    3)
            ->buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(TestnetProxyProviderTxFixture, send_issueESDTTransaction_withESDTProperties)
{
    ESDTProperties esdtProperties;
    esdtProperties.canMint = true;
    esdtProperties.canBurn = true;
    esdtProperties.canFreeze = true;

    Transaction transaction = m_txFactory.createESDTIssue(
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    1000000000,
                    "JonDoe",
                    "JDO",
                    BigUInt(444000),
                    3,
                    esdtProperties)
            ->buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(TestnetProxyProviderTxFixture, send_ESDT_noFunction)
{
    TokenPayment tokenPayment = TokenPayment::fungibleFromBigUInt("JDO-8a7f9b", BigUInt(1));
    Transaction transaction = m_txFactory.createESDTTransfer(
                    tokenPayment,
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
                    1000000000)
            ->buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(DevnetProxyProviderTxFixture, send_ESDT_function_unWrapEgld_noParams)
{
    ContractCall contractCall("unwrapEgld");
    TokenPayment tokenPayment = TokenPayment::fungibleFromBigUInt("WEGLD-b9cba1", BigUInt("10000000000000000"));
    Transaction transaction = m_txFactory.createESDTTransfer(
                    tokenPayment,
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    Address("erd1qqqqqqqqqqqqqpgqhpy2hy6p0pvw6z5l3w3ykrf9aj5r5s3n0n4s6vk5dw"),
                    1000000000)
            ->withContractCall(contractCall)
            .buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

TEST_F(DevnetProxyProviderTxFixture, send_ESDT_function_swapTokensFixedInput_noParams)
{
    ContractCall contractCall("swapTokensFixedInput");
    SCArguments args;
    args.add("MEX-bd9937");
    args.add(BigUInt("2689907400000000000")); //swap 0.01 WEGLD for ~2.7 MEX
    contractCall.setArgs(args);

    TokenPayment tokenPayment = TokenPayment::fungibleFromBigUInt("WEGLD-b9cba1", BigUInt("10000000000000000"));
    Transaction transaction = m_txFactory.createESDTTransfer(
                    tokenPayment,
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    Address("erd1qqqqqqqqqqqqqpgqx0xh8fgpr5kjh9n7s53m7qllw42m5t7u0n4suz39xc"),
                    1000000000)
            ->withContractCall(contractCall)
            .buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
}

#endif
