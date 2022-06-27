#include "gtest/gtest.h"

#include "transaction/token_payment.h"


TEST(TokenPayment, withESDT)
{
    std::string identifier = "USDC-c76f1f";
    size_t numDecimals = 6;

    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "1", numDecimals).toString(), "1000000");
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "1.1", numDecimals).toString(), "1100000");
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "0.1", numDecimals).toString(), "100000");
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "0.123456789", numDecimals).toString(), "123456");
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "123.456789", numDecimals).toString(), "123456789");
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "12345.6789", numDecimals).toString(), "12345678900");

    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("1000000"), numDecimals).toString(), "1000000");

    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("1"), numDecimals).toPrettyString(), "0.000001 USDC-c76f1f");
    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("1000000"), numDecimals).toPrettyString(), "1.000000 USDC-c76f1f");
    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("123000000"), numDecimals).toPrettyString(), "123.000000 USDC-c76f1f");
    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("12345678"), numDecimals).toPrettyString(), "12.345678 USDC-c76f1f");
    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("123456789"), numDecimals).toPrettyString(), "123.456789 USDC-c76f1f");

    EXPECT_EQ(TokenPayment::fungibleFromBigUInt(identifier, BigUInt("123456789")).toPrettyString(), "123456789.0 USDC-c76f1f"); // without specifying numDecimals
}
