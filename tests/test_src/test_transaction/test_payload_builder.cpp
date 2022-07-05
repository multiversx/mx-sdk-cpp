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
    TokenPayment sft = TokenPayment::semiFungible("SEMI-9efd0f", 1, BigUInt(5));
    std::string payload = ESDTNFTTransferPayloadBuilder()
            .setPayment(sft)
            .setDestination(Address("erd1testnlersh4z0wsv8kjx39me4rmnvjkwu8dsaea7ukdvvc9z396qykv7z7"))
            .build();
    EXPECT_EQ(payload, "ESDTNFTTransfer@53454d492d396566643066@01@05@5e60b9ff2385ea27ba0c3da4689779a8f7364acee1db0ee7bee59ac660a28974");

    TokenPayment nft = TokenPayment::nonFungible("OGS-3f1408", 2111);
    payload = ESDTNFTTransferPayloadBuilder()
            .setPayment(nft)
            .setDestination(Address("erd1q637cys8k0squ5xun22kwycqp63wa9edxuk779tjwazsvmyqcr2sgpkmx5"))
            .build();
    EXPECT_EQ(payload, "ESDTNFTTransfer@4f47532d336631343038@083f@01@06a3ec1207b3e00e50dc9a956713000ea2ee972d372def15727745066c80c0d5");

    TokenPayment metaESDT = TokenPayment::metaESDTFromAmount("LKMEX-aab910", 3983756, "54268.574065528382947328", 18);
    payload = ESDTNFTTransferPayloadBuilder()
            .setPayment(metaESDT)
            .setDestination(Address("erd1qqqqqqqqqqqqqpgqt7tyyswqvplpcqnhwe20xqrj7q7ap27d2jps7zczse"))
            .build();
    EXPECT_EQ(payload, "ESDTNFTTransfer@4c4b4d45582d616162393130@3cc98c@0b7de7c1695eab800000@000000000000000005005f964241c0607e1c02777654f30072f03dd0abcd5483");
}

TEST(MultiESDTNFTTransferPayloadBuilder, build)
{
    TokenPayment paymentOne = TokenPayment::nonFungible("ERDJS-38f249", 1);
    TokenPayment paymentTwo = TokenPayment::fungibleFromAmount("BAR-c80d29", "10.00", 18);

    std::string payload = MultiESDTNFTTransferPayloadBuilder()
            .setPayments({paymentOne, paymentTwo})
            .setDestination(Address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"))
            .build();

    EXPECT_EQ(payload,
              "MultiESDTNFTTransfer@0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1@02@4552444a532d333866323439@01@01@4241522d633830643239@00@8ac7230489e80000");

    paymentOne = TokenPayment::metaESDTFromAmount("LKMEX-aab910", 4007927, "11702.951439779957981665", 18);
    paymentTwo = TokenPayment::metaESDTFromAmount("LKFARM-9d1ea8", 8576971, "40071974.962988352918814287", 18);
    payload = MultiESDTNFTTransferPayloadBuilder()
            .setPayments({paymentOne, paymentTwo})
            .setDestination(Address("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl"))
            .build();
    EXPECT_EQ(payload,
              "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4c4b4d45582d616162393130@3d27f7@027a6b13a995f2aa41e1@4c4b4641524d2d396431656138@82dfcb@2125921d938f56cf687e4f");
}
