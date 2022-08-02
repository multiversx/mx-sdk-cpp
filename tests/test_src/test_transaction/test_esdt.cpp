#include "gtest/gtest.h"

#include "transaction//esdt.h"

TEST(ESDTProperties, comparisonOperators)
{
    ESDTProperties esdt1, esdt2;

    EXPECT_TRUE(esdt1 == esdt2);
    EXPECT_TRUE(esdt1 == ESDT_ISSUANCE_DEFAULT_PROPERTIES);
    EXPECT_TRUE(esdt2 == ESDT_ISSUANCE_DEFAULT_PROPERTIES);
    EXPECT_FALSE(esdt1 != esdt2);

    esdt1.canChangeOwner = true;
    esdt2.canUpgrade = true;

    EXPECT_FALSE(esdt1 == esdt2);
    EXPECT_TRUE(esdt1 != ESDT_ISSUANCE_DEFAULT_PROPERTIES);
    EXPECT_TRUE(esdt1 != ESDT_ISSUANCE_DEFAULT_PROPERTIES);

    esdt1.canChangeOwner = false;
    esdt2.canUpgrade = false;

    EXPECT_TRUE(esdt1 == esdt2);
    EXPECT_TRUE(esdt1 == ESDT_ISSUANCE_DEFAULT_PROPERTIES);
    EXPECT_TRUE(esdt2 == ESDT_ISSUANCE_DEFAULT_PROPERTIES);
    EXPECT_FALSE(esdt1 != esdt2);
}
