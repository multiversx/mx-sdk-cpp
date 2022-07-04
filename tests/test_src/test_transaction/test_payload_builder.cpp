#include "gtest/gtest.h"
#include "transaction/payload_builder.h"
#include "transaction/transaction_factory.h"


ContractCall generateSCCall()
{
    SCArguments args;
    args.add(Address("erd1qqqqqqqqqqqqqpgqtga4sxnc462aeqdpxxajs8k7pr64ltkh2jps3ewwyw"));
    args.add("swapTokensFixedInput");
    args.add(BigUInt(135960482163585));

    return ContractCall("multiPairSwap", args);
}

TEST(ESDTTransferPayloadBuilder, build)
{
    auto builder = ESDTTransferPayloadBuilder();

    TokenPayment payment = TokenPayment::fungibleFromAmount("COUNTER-8b028f", "100.00", 0);
    std::string payload = builder.setPayment(payment).build();
    std::string expectedPayload = "ESDTTransfer@434f554e5445522d386230323866@64";
    EXPECT_EQ(payload, expectedPayload);

    payment = TokenPayment::fungibleFromAmount("RIDE-7d18e9", "1634.132360763445665862", 18);
    payload = builder.setPayment(payment).build();
    expectedPayload = "ESDTTransfer@524944452d376431386539@589624641c2ba8e046";
    EXPECT_EQ(payload, expectedPayload);

    ContractCall contractCall = generateSCCall();
    payload = builder.withContractCall(contractCall).build();
    EXPECT_EQ(payload, expectedPayload + contractCall.asOnData());
}

TEST(ESDTNFTTransferPayloadBuilder, build)
{
    auto builder = ESDTNFTTransferPayloadBuilder();

    TokenPayment sft = TokenPayment::semiFungible("SEMI-9efd0f", 1, BigUInt(5));
    std::string payload = builder
            .setPayment(sft)
            .setDestination(Address("erd1testnlersh4z0wsv8kjx39me4rmnvjkwu8dsaea7ukdvvc9z396qykv7z7"))
            .build();
    std::string expectedPayload = "ESDTNFTTransfer@53454d492d396566643066@01@05@5e60b9ff2385ea27ba0c3da4689779a8f7364acee1db0ee7bee59ac660a28974";
    EXPECT_EQ(payload, expectedPayload);

    TokenPayment nft = TokenPayment::nonFungible("OGS-3f1408", 2111);
    payload = builder
            .setPayment(nft)
            .setDestination(Address("erd1q637cys8k0squ5xun22kwycqp63wa9edxuk779tjwazsvmyqcr2sgpkmx5"))
            .build();
    expectedPayload = "ESDTNFTTransfer@4f47532d336631343038@083f@01@06a3ec1207b3e00e50dc9a956713000ea2ee972d372def15727745066c80c0d5";
    EXPECT_EQ(payload, expectedPayload);

    TokenPayment metaESDT = TokenPayment::metaESDTFromAmount("LKMEX-aab910", 3983756, "54268.574065528382947328", 18);
    payload = builder
            .setPayment(metaESDT)
            .setDestination(Address("erd1qqqqqqqqqqqqqpgqt7tyyswqvplpcqnhwe20xqrj7q7ap27d2jps7zczse"))
            .build();
    expectedPayload = "ESDTNFTTransfer@4c4b4d45582d616162393130@3cc98c@0b7de7c1695eab800000@000000000000000005005f964241c0607e1c02777654f30072f03dd0abcd5483";
    EXPECT_EQ(payload, expectedPayload);

    ContractCall contractCall = generateSCCall();
    payload = builder.withContractCall(contractCall).build();
    EXPECT_EQ(payload, expectedPayload + contractCall.asOnData());
}

TEST(MultiESDTNFTTransferPayloadBuilder, build)
{
    auto builder = MultiESDTNFTTransferPayloadBuilder();

    TokenPayment paymentOne = TokenPayment::nonFungible("ERDJS-38f249", 1);
    TokenPayment paymentTwo = TokenPayment::fungibleFromAmount("BAR-c80d29", "10.00", 18);

    std::string payload = builder
            .setPayments({paymentOne, paymentTwo})
            .setDestination(Address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"))
            .build();
    std::string expectedPayload = "MultiESDTNFTTransfer@0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1@02@4552444a532d333866323439@01@01@4241522d633830643239@00@8ac7230489e80000";
    EXPECT_EQ(payload, expectedPayload);

    paymentOne = TokenPayment::metaESDTFromAmount("LKMEX-aab910", 4007927, "11702.951439779957981665", 18);
    paymentTwo = TokenPayment::metaESDTFromAmount("LKFARM-9d1ea8", 8576971, "40071974.962988352918814287", 18);
    payload = builder
            .setPayments({paymentOne, paymentTwo})
            .setDestination(Address("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl"))
            .build();
    expectedPayload = "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4c4b4d45582d616162393130@3d27f7@027a6b13a995f2aa41e1@4c4b4641524d2d396431656138@82dfcb@2125921d938f56cf687e4f";
    EXPECT_EQ(payload, expectedPayload);

    ContractCall contractCall = generateSCCall();
    payload = builder.withContractCall(contractCall).build();
    EXPECT_EQ(payload, expectedPayload + contractCall.asOnData());
}

TEST(TransactionFactory, createEGLDTransfer)
{
    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createEGLDTransfer(123,
                                BigUInt(3333),
                                Address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"),
                                Address("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl"),
                                99999,
                                "hello")
            .withVersion(444)
            .build();

   EXPECT_EQ(tx.serialize(), "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":444}");
}

TEST(TransactionFactory, createESDTTransfer)
{
    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    TokenPayment paymentOne = TokenPayment::nonFungible("ERDJS-38f249", 1);
    Transaction tx = txFactory
            .createESDTTransfer(paymentOne,
                                123,
                                Address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"),
                                Address("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl"),
                                99999)
            .withContractCall(generateSCCall())
            .build();

    EXPECT_EQ(tx.serialize(), "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"gasPrice\":99999,\"gasLimit\":627500,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDRhNTMyZDMzMzg2NjMyMzQzOUAwMTZkNzU2Yzc0Njk1MDYxNjk3MjUzNzc2MTcwQDAwMDAwMDAwMDAwMDAwMDAwNTAwNWEzYjU4MWE3OGFlOTVkYzgxYTEzMWJiMjgxZWRlMDhmNTVmYWVkNzU0ODNANzM3NzYxNzA1NDZmNmI2NTZlNzM0NjY5Nzg2NTY0NDk2ZTcwNzU3NEA3YmE3YzQzZTJiODE=\",\"chainID\":\"1\",\"version\":1}");
}
