#include <utility>

#include "gtest/gtest.h"

#include "transaction/transaction_factory.h"
#include "provider/proxyprovider.h"
#include "filehandler/pemreader.h"
#include "utils/errors.h"
#include "thread"
#include "test_common.h"

const std::string localProxyUrl("http://127.0.0.1:7950");
const std::string pemPath = getCanonicalRootPath("testnet/testnet-local/sandbox/node/config/walletKey.pem");
const uint8_t intra_shard_execution = 6;
const uint8_t cross_shard_execution = 18;

Address getAddressFromPem()
{
    PemFileReader pem(pemPath);
    auto addr = pem.getAddress();
    return addr;
}

TEST(ProxyProvider, getAccount)
{
    ProxyProvider proxy(localProxyUrl);

    Address const address(getAddressFromPem());
    Account const account = proxy.getAccount(address);

    EXPECT_FALSE(account.getAddress().getBech32Address().empty());
    EXPECT_FALSE(account.getBalance().getValue().empty());
    EXPECT_FALSE(account.getBalance().getValue() == DEFAULT_BALANCE.getValue());
    EXPECT_FALSE(account.getNonce() == 0);
}

TEST(ProxyProvider, getTransactionStatus_invalidHash)
{
    ProxyProvider proxy(localProxyUrl);

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
    ProxyProvider proxy(localProxyUrl);
    Address const address(getAddressFromPem());
    BigUInt balance = proxy.getESDTBalance(address, "this esdt does not exist");

    EXPECT_TRUE(balance == DEFAULT_BALANCE);
}

TEST(ProxyProvider, getAllESDTokenBalances_noTokens)
{
    ProxyProvider proxy(localProxyUrl);
    Address const address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    auto const esdts = proxy.getAllESDTBalances(address);

    EXPECT_TRUE(esdts.empty());
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
    explicit GenericProxyProviderTxFixture(std::string proxyUrl, std::string pemSourcePath) :
            m_proxy(std::move(proxyUrl)),
            m_pem(pemSourcePath),
            m_senderAddr(m_pem.getAddress()),
            m_senderAcc(m_proxy.getAccount(m_senderAddr)),
            m_txFactory({"", 0, 0, 0})
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        auto updatedAccount = m_proxy.getAccount(m_senderAddr);
        m_networkConfig = m_proxy.getNetworkConfig();
        m_txFactory = TransactionFactory(m_networkConfig);
    }

    void signTransaction(Transaction &transaction) const
    {
        Signer const signer(m_pem.getSeed());
        transaction.sign(signer);
    }

    std::string EXPECT_TRANSACTION_SENT_SUCCESSFULLY(Transaction const &transaction)
    {
        std::string txHash = m_proxy.send(transaction);
        EXPECT_FALSE(txHash.empty());

        return txHash;
    }

    void EXPECT_TRANSACTION_SUCCESSFUL(std::string txHash)
    {
        TransactionStatus txStatus = m_proxy.getTransactionStatus(txHash);
        EXPECT_TRUE(txStatus.isSuccessful());
    }

    void EXPECT_ACCOUNT_NONCE(uint64_t nonce)
    {
        Account const updatedAccount = m_proxy.getAccount(m_senderAddr);
        EXPECT_TRUE(updatedAccount.getNonce() == nonce);
    }

    void WAIT_INTRA_SHARD_EXECUTION()
    {
        std::this_thread::sleep_for(std::chrono::seconds(intra_shard_execution));
    }

    void WAIT_CROSS_SHARD_EXECUTION()
    {
        std::this_thread::sleep_for(std::chrono::seconds(cross_shard_execution));
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

        std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
        WAIT_CROSS_SHARD_EXECUTION();
        EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
        EXPECT_TRANSACTION_SUCCESSFUL(txHash);

        m_senderAcc.incrementNonce();
    }

    void transferESDT(TokenPayment const &tokenPayment, Address const &receiver)
    {
        Transaction transaction = m_txFactory.createESDTTransfer(
                tokenPayment,
                m_senderAcc.getNonce(),
                m_senderAddr,
                receiver,
                1000000000)->buildSigned(m_pem.getSeed());

        std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
        WAIT_INTRA_SHARD_EXECUTION();
        EXPECT_TRANSACTION_SUCCESSFUL(txHash);
        EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
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

    static void checkMapContainsESDTBalance(const std::map<std::string, BigUInt> &esdts, const std::string &ticker, const BigUInt &balance)
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
            GenericProxyProviderTxFixture(localProxyUrl, pemPath)
    {}
};

class DevnetProxyProviderTxFixture : public GenericProxyProviderTxFixture
{
public:
    DevnetProxyProviderTxFixture() :
            GenericProxyProviderTxFixture("https://devnet-gateway.elrond.com", "..//..//testData//keysValid1.pem")
    {}
};

TEST_F(LocalTestnetProxyProviderTxFixture, send_validTx)
{
    Transaction transaction = m_txFactory.createEGLDTransfer(
                    m_senderAcc.getNonce(),
                    BigUInt("1000000000000"),
                    m_senderAddr,
                    Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
                    1000000000)
            ->buildSigned(m_pem.getSeed());

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_INTRA_SHARD_EXECUTION();
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_validTx_signedHashedTx)
{
    Transaction transaction = m_txFactory.createEGLDTransfer(
                    m_senderAcc.getNonce(),
                    BigUInt("1000000000000"),
                    m_senderAddr,
                    Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
                    1000000000)
            ->withOptions(1)
            .withVersion(2)
            .buildSigned(m_pem.getSeed());

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_INTRA_SHARD_EXECUTION();
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_invalidTx_noSignature)
{
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transaction.m_chainID = "local-testnet";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = BigUInt("10000000000");
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

TEST_F(LocalTestnetProxyProviderTxFixture, esdt_issue_and_transfer)
{
    issueESDT("Alice", "ALC", BigUInt(123), 0);
    issueESDT("Bob", "BOB", BigUInt(321), 0, ESDTProperties{
            .canFreeze = true,
            .canWipe = true,
            .canMint = true});

    // Wait for meta to transfer esdt to account in shard
    WAIT_CROSS_SHARD_EXECUTION();

    std::string aliceTokenID = getTokenID("ALC");
    std::string bobTokenID = getTokenID("BOB");

    EXPECT_ACCOUNT_HAS_ESDTS(m_senderAddr,
                             {{aliceTokenID, BigUInt(123)},
                              {bobTokenID,   BigUInt(321)}});

    TokenPayment alicePayment = TokenPayment::fungibleFromAmount(aliceTokenID, "23", 0);
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transferESDT(alicePayment, receiver);

    EXPECT_ACCOUNT_HAS_ESDTS(m_senderAddr,
                             {{aliceTokenID, BigUInt(100)},
                              {bobTokenID,   BigUInt(321)}});
    EXPECT_ACCOUNT_HAS_ESDTS(receiver, {{aliceTokenID, BigUInt(23)}});
}

TEST_F(DevnetProxyProviderTxFixture, DISABLED_send_ESDT_function_unWrapEgld_noParams)
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

TEST_F(DevnetProxyProviderTxFixture, DISABLED_send_ESDT_function_swapTokensFixedInput_noParams)
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
