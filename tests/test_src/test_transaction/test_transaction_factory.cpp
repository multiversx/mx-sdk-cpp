#include "gtest/gtest.h"
#include "filehandler/pemreader.h"
#include "transaction/transaction_factory.h"
#include "test_common.h"

ContractCall generateSCCall()
{
    SCArguments args;
    args.add(Address("erd1qqqqqqqqqqqqqpgqtga4sxnc462aeqdpxxajs8k7pr64ltkh2jps3ewwyw"));
    args.add("swapTokensFixedInput");
    args.add(BigUInt(135960482163585));

    return ContractCall("multiPairSwap", args);
}

TEST(TransactionFactory, createEGLDTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    uint64_t nonce = 123;
    BigUInt value(3333);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;
    std::string data("hello");

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createEGLDTransfer(nonce, value, sender, receiver, gasPrice, data)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createEGLDTransfer(nonce, value, sender, receiver, gasPrice, data)
            .withVersion(444)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":444}");

    tx = txFactory
            .createEGLDTransfer(nonce, value, sender, receiver, gasPrice, data)
            .withVersion(444)
            .withOptions(222)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":444,\"options\":222}");

    tx = txFactory
            .createEGLDTransfer(nonce, value, sender, receiver, gasPrice, data)
            .withVersion(444)
            .withOptions(222)
            .buildSigned(pem);
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"signature\":\"03e076e58eab1a441f28cb68ef8ab2568e3c39888396ecf86c4a375fdd8b7823ad25059791c620a22b74168248abbcf22cd2e5300e6942be1236b0244e3a7f04\",\"chainID\":\"1\",\"version\":444,\"options\":222}");

    tx = txFactory
            .createEGLDTransfer(nonce, value, sender, receiver, gasPrice, data)
            .withVersion(444)
            .withOptions(222)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"signature\":\"03e076e58eab1a441f28cb68ef8ab2568e3c39888396ecf86c4a375fdd8b7823ad25059791c620a22b74168248abbcf22cd2e5300e6942be1236b0244e3a7f04\",\"chainID\":\"1\",\"version\":444,\"options\":222}");

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

    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"gasPrice\":99999,\"gasLimit\":627500,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDRhNTMyZDMzMzg2NjMyMzQzOUAwMTZkNzU2Yzc0Njk1MDYxNjk3MjUzNzc2MTcwQDAwMDAwMDAwMDAwMDAwMDAwNTAwNWEzYjU4MWE3OGFlOTVkYzgxYTEzMWJiMjgxZWRlMDhmNTVmYWVkNzU0ODNANzM3NzYxNzA1NDZmNmI2NTZlNzM0NjY5Nzg2NTY0NDk2ZTcwNzU3NEA3YmE3YzQzZTJiODE=\",\"chainID\":\"1\",\"version\":1}");
}
