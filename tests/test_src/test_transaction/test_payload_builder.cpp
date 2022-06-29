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

TEST(ESDTNFTTransferPayloadBuilder, build)
{
    TokenPayment payment = TokenPayment::semiFungible("SEMI-9efd0f", 1, BigUInt(5));

    std::string payload = ESDTNFTTransferPayloadBuilder()
            .setPayment(payment)
            .setDestination(Address("erd1testnlersh4z0wsv8kjx39me4rmnvjkwu8dsaea7ukdvvc9z396qykv7z7"))
            .build();

    EXPECT_EQ(payload, "ESDTNFTTransfer@53454d492d396566643066@01@05@5e60b9ff2385ea27ba0c3da4689779a8f7364acee1db0ee7bee59ac660a28974");
}
