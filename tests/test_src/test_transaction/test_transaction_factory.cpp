#include "gtest/gtest.h"
#include "filehandler/pemreader.h"
#include "transaction/transaction_factory.h"
#include "test_common.h"

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
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"signature\":\"03e076e58eab1a441f28cb68ef8ab2568e3c39888396ecf86c4a375fdd8b7823ad25059791c620a22b74168248abbcf22cd2e5300e6942be1236b0244e3a7f04\",\"chainID\":\"1\",\"version\":444,\"options\":222}");
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    uint64_t nonce = 123;
    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 1);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;
    std::string data("hello");

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);

    Transaction tx = txFactory
            .createESDTTransfer(token, nonce, sender, receiver, gasPrice)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTTransfer(token, nonce, sender, receiver, gasPrice)
            .withVersion(444)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":444}");

    tx = txFactory
            .createESDTTransfer(token, nonce, sender, receiver, gasPrice)
            .withVersion(444)
            .withOptions(222)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":444,\"options\":222}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createESDTTransfer(token, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(444)
            .withOptions(222)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":434000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAxQDY2NmY2ZkA2MjZmNmY=\",\"chainID\":\"1\",\"version\":444,\"options\":222}");

    tx = txFactory
            .createESDTTransfer(token, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(444)
            .withOptions(222)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":434000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAxQDY2NmY2ZkA2MjZmNmY=\",\"signature\":\"3535898675f851241f0c211f1fd0b222d4fb06ea42056599a61233f12a088e8cbbd7ce14f3d85fd4f0691bbbde11ee49298a5fc192788da4753afe32f906ad0c\",\"chainID\":\"1\",\"version\":444,\"options\":222}");
    EXPECT_TRUE(tx.verify());
}
