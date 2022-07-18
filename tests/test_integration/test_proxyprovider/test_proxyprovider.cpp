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
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transaction.m_chainID = "local-testnet";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = BigUInt("10000000000");
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;

    signTransaction(transaction);
    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_INTRA_SHARD_EXECUTION();
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_validTx_signedHashedTx)
{
    Transaction transaction;
    transaction.m_sender = std::make_shared<Address>(m_senderAddr);
    transaction.m_receiver = std::make_shared<Address>("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");
    transaction.m_chainID = "local-testnet";
    transaction.m_nonce = m_senderAcc.getNonce();
    transaction.m_value = BigUInt("1000000000000");
    transaction.m_gasPrice = 1000000000;
    transaction.m_gasLimit = 50000;
    transaction.m_options = std::make_shared<uint32_t>(1U);
    transaction.m_version = 2U;

    signTransaction(transaction);
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

TEST_F(LocalTestnetProxyProviderTxFixture, send_issueESDTTransaction_noESDTProperties)
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

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_CROSS_SHARD_EXECUTION();
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_issueESDTTransaction_withESDTProperties)
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

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_CROSS_SHARD_EXECUTION();
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
}

TEST_F(LocalTestnetProxyProviderTxFixture, send_ESDT_noFunction)
{
    TokenPayment tokenPayment = TokenPayment::fungibleFromBigUInt("JDO-8a7f9b", BigUInt(1));
    Transaction transaction = m_txFactory.createESDTTransfer(
                    tokenPayment,
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
                    1000000000)
            ->buildSigned(m_pem.getSeed());

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_INTRA_SHARD_EXECUTION();
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
}


TEST_F(LocalTestnetProxyProviderTxFixture, getAllESDTokenBalances_multipleTokens)
{
    Transaction transaction = m_txFactory.createESDTIssue(
                    m_senderAcc.getNonce(),
                    m_senderAddr,
                    1000000000,
                    "Alice",
                    "ALC",
                    BigUInt(123),
                    0)
            ->buildSigned(m_pem.getSeed());

    std::string txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_CROSS_SHARD_EXECUTION();
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);

    transaction = m_txFactory.createESDTIssue(
                    m_senderAcc.getNonce() + 1,
                    m_senderAddr,
                    1000000000,
                    "Bob",
                    "BOB",
                    BigUInt(321),
                    0)
            ->buildSigned(m_pem.getSeed());

    txHash = EXPECT_TRANSACTION_SENT_SUCCESSFULLY(transaction);
    WAIT_CROSS_SHARD_EXECUTION();
    EXPECT_ACCOUNT_NONCE(transaction.m_nonce + 1);
    EXPECT_TRANSACTION_SUCCESSFUL(txHash);

    WAIT_CROSS_SHARD_EXECUTION();

    auto esdts = m_proxy.getAllESDTBalances(getAddressFromPem());
    EXPECT_GE(esdts.size(), 2);
    bool foundALC = false;
    bool foundBOB = false;
    for (const auto &esdtBalance: esdts)
    {
        if (esdtBalance.first.find("ALC") != std::string::npos)
        {
            foundALC = true;
            EXPECT_EQ(esdtBalance.second.getValue(), BigUInt(123).getValue());
        }
        if (esdtBalance.first.find("BOB") != std::string::npos)
        {
            foundBOB = true;
            EXPECT_EQ(esdtBalance.second.getValue(), BigUInt(321).getValue());
        }
    }

    EXPECT_TRUE(foundALC && foundBOB);
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
