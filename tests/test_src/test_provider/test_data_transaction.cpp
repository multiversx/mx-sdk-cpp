#include "gtest/gtest.h"

#include "provider/data/data_transaction.h"

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
