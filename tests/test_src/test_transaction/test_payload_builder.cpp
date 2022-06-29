#include "gtest/gtest.h"
#include "transaction/payload_builder.h"

TEST(ESDTTransferPayloadBuilder, build)
{
    TokenPayment payment = TokenPayment::fungibleFromAmount("COUNTER-8b028f", "100.00", 0);
    std::string payload = ESDTTransferPayloadBuilder().setPayment(payment).build();

    EXPECT_EQ(payload, "ESDTTransfer@434f554e5445522d386230323866@64");

    payment = TokenPayment::fungibleFromAmount("RIDE-7d18e9", "1634.132360763445665862", 18);
    payload = ESDTTransferPayloadBuilder().setPayment(payment).build();
    EXPECT_EQ(payload, "ESDTTransfer@524944452d376431386539@589624641c2ba8e046");
}
