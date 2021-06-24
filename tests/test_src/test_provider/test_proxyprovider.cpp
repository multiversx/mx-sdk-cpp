#include "gtest/gtest.h"

#include "provider/proxyprovider.h"
#include "filehandler/pemreader.h"

enum txStatusResponse
{
    pending,
    executed,
    successful,
    failed,
    invalid
};

struct txStatus
{
    std::string rawStatus;
    txStatusResponse expectedStatus;
};

class TransactionStatusParametrized : public ::testing::TestWithParam<txStatus>
{
public:
    void EXPECT_TRANSACTION_STATUS_EQ(TransactionStatus const &txStat, txStatusResponse const &expectedTxStat)
    {
        switch (expectedTxStat)
        {
            case pending:
            {
                EXPECT_TRUE(txStat.isPending());
                break;
            }
            case executed:
            {
                EXPECT_TRUE(txStat.isExecuted());
                break;
            }
            case successful:
            {
                EXPECT_TRUE(txStat.isExecuted());
                break;
            }
            case failed:
            {
                EXPECT_TRUE(txStat.isFailed());
                break;
            }
            case invalid:
            {
                EXPECT_TRUE(txStat.isInvalid());
                break;
            }
            default:
            {
                FAIL();
                break;
            }
        }
    }
};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionStatusParametrized,
        ::testing::Values(
                txStatus{"received", pending},
                txStatus{"pending", pending},
                txStatus{"partially-executed", pending},

                txStatus{"executed", successful},
                txStatus{"success", successful},
                txStatus{"successful", successful},

                txStatus{"executed", executed},
                txStatus{"success", executed},
                txStatus{"successful", executed},
                txStatus{"invalid", executed},

                txStatus{"fail", failed},
                txStatus{"failed", failed},
                txStatus{"unsuccessful", failed},
                txStatus{"invalid", failed},

                txStatus{"invalid", invalid}));

TEST_P(TransactionStatusParametrized, allTransactionStats)
{
    txStatus const& currParam = GetParam();

    TransactionStatus const transactionStatus(currParam.rawStatus);

    EXPECT_TRANSACTION_STATUS_EQ(transactionStatus, currParam.expectedStatus);
}

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
