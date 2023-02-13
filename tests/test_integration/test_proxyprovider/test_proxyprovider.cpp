#include "gtest/gtest.h"

#include "thread"
#include "utils/common.h"
#include "utils/errors.h"
#include "filehandler/pemreader.h"
#include "provider/proxyprovider.h"
#include "transaction/transaction_factory.h"

#define LOCAL_PROXY_URL std::string("http://127.0.0.1:7950")
#define PEM_PATH util::getCanonicalRootPath("mx-sdk-cpp/testnet/testnet-local/sandbox/node/config/walletKey.pem")
#define ROUNDS_IN_EPOCH 20
#define ROUND_TIME 6
#define TIME_INTRA_SHARD_EXECUTION 6
#define TIME_CROSS_SHARD_EXECUTION 18
#define INTRA_SHARD true
#define CROSS_SHARD !INTRA_SHARD

void wait(uint8_t const &seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void checkMapContainsESDTBalance(const std::map<std::string, BigUInt> &esdts, const std::string &ticker, const BigUInt &balance)
{
    bool found = false;
    for (const auto &esdtBalance: esdts)
    {
        if (esdtBalance.first.find(ticker) != std::string::npos)
        {
            EXPECT_EQ(esdtBalance.second.getValue(), balance.getValue());
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
}

class GenericProxyProviderTxFixture : public ::testing::Test
{
public:
    explicit GenericProxyProviderTxFixture(std::string proxyUrl, std::string const &pemSourcePath) :
            m_proxy(std::move(proxyUrl)),
            m_pem(pemSourcePath),
            m_senderAddr(m_pem.getAddress()),
            m_senderAcc(m_proxy.getAccount(m_senderAddr)),
            m_txFactory({"", 0, 0, 0})
    {
        m_networkConfig = m_proxy.getNetworkConfig();
        m_txFactory = TransactionFactory(m_networkConfig);
    }

    void EXPECT_TRANSACTION_SENT_SUCCESSFULLY(Transaction const &transaction, bool const intraShard)
    {
        std::string txHash = m_proxy.send(transaction);
        EXPECT_FALSE(txHash.empty());

        uint8_t waitTime = intraShard ? TIME_INTRA_SHARD_EXECUTION : TIME_CROSS_SHARD_EXECUTION;
        wait(waitTime);

        TransactionStatus txStatus = m_proxy.getTransactionStatus(txHash);
        EXPECT_TRUE(txStatus.isSuccessful());

        m_senderAcc = m_proxy.getAccount(m_senderAddr);
        EXPECT_TRUE(m_senderAcc.getNonce() == transaction.m_nonce + 1);
    }

    void EXPECT_ACCOUNT_HAS_ESDTS(Address const &address, const std::map<std::string, BigUInt> &expectedESDTs) const
    {
        auto esdts = m_proxy.getAllESDTBalances(address);
        EXPECT_GE(esdts.size(), expectedESDTs.size());

        for (const auto &esdtBalance: expectedESDTs)
        {
            checkMapContainsESDTBalance(esdts, esdtBalance.first, esdtBalance.second);
        }
    }

    void issueESDT(std::string const &token,
                   std::string const &ticker,
                   BigUInt const &initialSupply,
                   uint32_t const &numOfDecimals,
                   ESDTProperties const &esdtProperties = ESDT_ISSUANCE_DEFAULT_PROPERTIES)
    {
        Transaction transaction = m_txFactory.createESDTIssue(
                        m_senderAcc.getNonce(),
                        m_senderAddr,
                        1000000000,
                        token,
                        ticker,
                        initialSupply,
                        numOfDecimals,
                        esdtProperties)
                ->buildSigned(m_pem.getSeed());

        EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, CROSS_SHARD);
    }

    void transferESDT(TokenPayment const &tokenPayment, Address const &receiver)
    {
        Transaction transaction = m_txFactory.createESDTTransfer(
                tokenPayment,
                m_senderAcc.getNonce(),
                m_senderAddr,
                receiver,
                1000000000)->buildSigned(m_pem.getSeed());

        EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, INTRA_SHARD);
    }

    std::string getTokenID(std::string const &ticker) const
    {
        auto esdts = m_proxy.getAllESDTBalances(m_senderAddr);
        EXPECT_GE(esdts.size(), 1);

        std::string ret;
        for (const auto &esdtBalance: esdts)
        {
            if (esdtBalance.first.find(ticker) != std::string::npos)
            {
                ret = esdtBalance.first;
                break;
            }
        }

        EXPECT_FALSE(ret.empty());
        return ret;
    }

    TransactionFactory m_txFactory;
    NetworkConfig m_networkConfig;
    ProxyProvider m_proxy;
    PemFileReader m_pem;
    Address m_senderAddr;
    Account m_senderAcc;
};

class LocalTestnetProxyProviderTxFixture : public GenericProxyProviderTxFixture
{
public:
    LocalTestnetProxyProviderTxFixture() :
            GenericProxyProviderTxFixture(LOCAL_PROXY_URL, PEM_PATH)
    {}
};

TEST_F(LocalTestnetProxyProviderTxFixture, getNetworkConfig)
{
    NetworkConfig expectedConfig{
            .chainId = "local-testnet",
            .gasPerDataByte = 1500,
            .minGasLimit = 50000,
            .minGasPrice = 1000000000,
    };
    NetworkConfig config = m_proxy.getNetworkConfig();

    EXPECT_EQ(config.chainId, expectedConfig.chainId);
    EXPECT_EQ(config.gasPerDataByte, expectedConfig.gasPerDataByte);
    EXPECT_EQ(config.minGasPrice, expectedConfig.minGasPrice);
    EXPECT_EQ(config.minGasLimit, expectedConfig.minGasLimit);
    EXPECT_EQ(config, expectedConfig);
}

TEST_F(LocalTestnetProxyProviderTxFixture, getAccount)
{
    EXPECT_FALSE(m_senderAcc.getAddress().getBech32Address().empty());
    EXPECT_FALSE(m_senderAcc.getBalance().getValue().empty());
    EXPECT_FALSE(m_senderAcc.getBalance().getValue() == DEFAULT_BALANCE.getValue());
}

TEST_F(LocalTestnetProxyProviderTxFixture, getESDTokenBalance_invalidToken)
{
    EXPECT_THROW({
        try
        {
            BigUInt balance = m_proxy.getESDTBalance(m_senderAddr, "this esdt does not exist");
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

TEST_F(LocalTestnetProxyProviderTxFixture, getAllESDTokenBalances_noTokens)
{
    EXPECT_THROW({
                     try
                     {
                         Address const address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
                         auto const esdts = m_proxy.getAllESDTBalances(address);
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

TEST_F(LocalTestnetProxyProviderTxFixture, getTransactionStatus_invalidHash)
{
    EXPECT_THROW({
                     try
                     {
                         m_proxy.getTransactionStatus("invalid hash");
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

TEST_F(LocalTestnetProxyProviderTxFixture, send_invalidTx_noSignature)
{
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    BigUInt initialBalanceSender = m_senderAcc.getBalance();
    BigUInt initialBalanceReceiver = m_proxy.getAccount(receiver).getBalance();

    Transaction transaction = m_txFactory.createEGLDTransfer(
                    m_senderAcc.getNonce(),
                    BigUInt(12356789),
                    m_senderAddr,
                    receiver,
                    1000000000)
            ->build();

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

    EXPECT_EQ(m_proxy.getAccount(m_senderAddr).getBalance(), initialBalanceSender);
    EXPECT_EQ(m_proxy.getAccount(receiver).getBalance(), initialBalanceReceiver);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_validTx_moveBalance)
{
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    BigUInt value("1000000000000");

    BigUInt initialBalanceSender = m_senderAcc.getBalance();
    BigUInt initialBalanceReceiver = m_proxy.getAccount(receiver).getBalance();

    Transaction transaction = m_txFactory.createEGLDTransfer(
                    m_senderAcc.getNonce(),
                    value,
                    m_senderAddr,
                    receiver,
                    1000000000)
            ->buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, INTRA_SHARD);

    BigUInt finalBalanceSender = m_senderAcc.getBalance();
    BigUInt finalBalanceReceiver = m_proxy.getAccount(receiver).getBalance();
    EXPECT_TRUE(finalBalanceSender < initialBalanceSender - value); // less, because transfer includes fees
    EXPECT_TRUE(finalBalanceReceiver == initialBalanceReceiver + value);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_validTx_signedHashedTx_moveBalance)
{
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    BigUInt value("1234567890000");

    BigUInt initialBalanceSender = m_senderAcc.getBalance();
    BigUInt initialBalanceReceiver = m_proxy.getAccount(receiver).getBalance();

    Transaction transaction = m_txFactory.createEGLDTransfer(
                    m_senderAcc.getNonce(),
                    value,
                    m_senderAddr,
                    receiver,
                    1000000000)
            ->withOptions(1)
            .withVersion(2)
            .buildSigned(m_pem.getSeed());

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, INTRA_SHARD);

    BigUInt finalBalanceSender = m_senderAcc.getBalance();
    BigUInt finalBalanceReceiver = m_proxy.getAccount(receiver).getBalance();
    EXPECT_TRUE(finalBalanceSender < initialBalanceSender - value); // less, because transfer includes fees
    EXPECT_TRUE(finalBalanceReceiver == initialBalanceReceiver + value);
}

TEST_F(LocalTestnetProxyProviderTxFixture, esdt_issue_transfer_getESDTBalance)
{
    wait(ROUND_TIME * ROUNDS_IN_EPOCH);

    issueESDT("Alice", "ALC", BigUInt(123), 0);
    issueESDT("Bob", "BOB", BigUInt(321), 0, ESDTProperties{
            .canFreeze = true,
            .canWipe = true,
            .canMint = true});

    // Wait for meta to transfer esdt to account in shard
    wait(TIME_CROSS_SHARD_EXECUTION);

    std::string aliceTokenID = getTokenID("ALC");
    std::string bobTokenID = getTokenID("BOB");

    EXPECT_ACCOUNT_HAS_ESDTS(m_senderAddr, {{aliceTokenID, BigUInt(123)},
                                            {bobTokenID,   BigUInt(321)}});

    TokenPayment aliceTokens = TokenPayment::fungibleFromAmount(aliceTokenID, "23", 0);
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transferESDT(aliceTokens, receiver);

    EXPECT_EQ(m_proxy.getESDTBalance(receiver, aliceTokenID), BigUInt(23));
    EXPECT_ACCOUNT_HAS_ESDTS(m_senderAddr, {{aliceTokenID, BigUInt(100)},
                                            {bobTokenID,   BigUInt(321)}});
}

// Following tests are disabled, since they interact with a SC which is not deployed on local testnet.
TEST_F(LocalTestnetProxyProviderTxFixture, DISABLED_send_ESDT_function_unWrapEgld_noParams)
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

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, INTRA_SHARD);
}

TEST_F(LocalTestnetProxyProviderTxFixture, DISABLED_send_ESDT_function_swapTokensFixedInput_noParams)
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

    EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction, INTRA_SHARD);
}
