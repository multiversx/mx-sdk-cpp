#include "gtest/gtest.h"
#include "transaction/payload_builder.h"

TEST(ESDTTransferPayloadBuilder, build)
{
    TokenPayment const payment = TokenPayment::fungibleFromAmount("COUNTER-8b028f", "100.00", 0);
    std::string const payload = ESDTTransferPayloadBuilder().setPayment(payment).build();

    EXPECT_EQ(payload, "ESDTTransfer@434f554e5445522d386230323866@64");
}
