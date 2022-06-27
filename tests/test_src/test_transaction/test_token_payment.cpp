#include "gtest/gtest.h"

#include "transaction/token_payment.h"


TEST(TokenPayment, ESDT)
{
    std::string identifier = "USDC-c76f1f";
    size_t numDecimals = 6;

    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "1", numDecimals).nonce(), 0);
    EXPECT_EQ(TokenPayment::fungibleFromAmount(identifier, "1", numDecimals).tokenIdentifier(), identifier);

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

TEST(TokenPayment, metaESDT)
{
    uint32_t numDecimals = 18;
    uint64_t nonce = 12345678;
    std::string identifier = "MEXFARML-28d646";

    TokenPayment tokenPaymentFromAmount = TokenPayment::metaESDTFromAmount(identifier, nonce, "0.100004", numDecimals);
    EXPECT_EQ(tokenPaymentFromAmount.tokenIdentifier(), identifier);
    EXPECT_EQ(tokenPaymentFromAmount.nonce(), nonce);
    EXPECT_EQ(tokenPaymentFromAmount.toString(), "100004000000000000");
    EXPECT_EQ(tokenPaymentFromAmount.toPrettyString(), "0.100004000000000000 MEXFARML-28d646");

    TokenPayment tokenPaymentFromBigUInt = TokenPayment::metaESDTFromBigUInt(identifier, nonce, BigUInt("100004"), numDecimals);
    EXPECT_EQ(tokenPaymentFromBigUInt.tokenIdentifier(), identifier);
    EXPECT_EQ(tokenPaymentFromBigUInt.nonce(), nonce);
    EXPECT_EQ(tokenPaymentFromBigUInt.toString(), "100004");
    EXPECT_EQ(tokenPaymentFromBigUInt.toPrettyString(), "0.000000000000100004 MEXFARML-28d646");

    TokenPayment tokenPaymentFromBigUIntWithoutDecimals = TokenPayment::metaESDTFromBigUInt(identifier, nonce, BigUInt("100004"));
    EXPECT_EQ(tokenPaymentFromBigUIntWithoutDecimals.tokenIdentifier(), identifier);
    EXPECT_EQ(tokenPaymentFromBigUIntWithoutDecimals.nonce(), nonce);
    EXPECT_EQ(tokenPaymentFromBigUIntWithoutDecimals.toString(), "100004");
    EXPECT_EQ(tokenPaymentFromBigUIntWithoutDecimals.toPrettyString(), "100004.0 MEXFARML-28d646");
}
