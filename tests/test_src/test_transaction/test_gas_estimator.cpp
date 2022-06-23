#include "gtest/gtest.h"

#include "transaction/gas_estimator.h"

TEST(GasEstimator, defaultMainetNetworkConfig)
{
    NetworkConfig networkConfig = DEFAULT_MAINNET_NETWORK_CONFIG
    GasEstimator gasEstimator(networkConfig);

    EXPECT_EQ(gasEstimator.forEGLDTransfer(0), 50000);
    EXPECT_EQ(gasEstimator.forEGLDTransfer(3), 50000 + 3 * 1500);

    EXPECT_EQ(gasEstimator.forESDTTransfer(80), 50000 + 80 * 1500 + 200000 + 100000);
    EXPECT_EQ(gasEstimator.forESDTTransfer(100), 50000 + 100 * 1500 + 200000 + 100000);

    EXPECT_EQ(gasEstimator.forESDTNFTTransfer(80), 50000 + 80 * 1500 + 200000 + 800000);
    EXPECT_EQ(gasEstimator.forESDTNFTTransfer(100), 50000 + 100 * 1500 + 200000 + 800000);

    EXPECT_EQ(gasEstimator.forMultiESDTNFTTransfer(80, 1), 50000 + 80 * 1500 + (200000 + 800000) * 1);
    EXPECT_EQ(gasEstimator.forMultiESDTNFTTransfer(80, 3), 50000 + 80 * 1500 + (200000 + 800000) * 3);
}