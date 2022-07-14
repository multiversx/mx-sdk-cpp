#include "gtest/gtest.h"
#include "filehandler/pemreader.h"
#include "transaction/transaction_factory.h"
#include "test_common.h"

TEST(TransactionFactory, createEGLDTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    BigUInt value(3333);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;
    std::string data("hello");

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createEGLDTransfer(1, value, sender, receiver, gasPrice, data)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":1,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createEGLDTransfer(2, value, sender, receiver, gasPrice, data)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":2,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":3}");

    tx = txFactory
            .createEGLDTransfer(3, value, sender, receiver, gasPrice, data)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"chainID\":\"1\",\"version\":4,\"options\":5}");

    tx = txFactory
            .createEGLDTransfer(4, value, sender, receiver, gasPrice, data)
            ->withVersion(5)
            .withOptions(6)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"3333\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":57500,\"data\":\"aGVsbG8=\",\"signature\":\"cf77752dc29ed19e986e10a639ee3abb9d936ed9dbd123be1f44f31c9df5ed042fa4164ea12bad43ba497e1adcc8c1ada9b4b35142f7a94e49747b853b542b0d\",\"chainID\":\"1\",\"version\":5,\"options\":6}");
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 1);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;
    std::string data("hello");

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);

    Transaction tx = txFactory
            .createESDTTransfer(token, 1, sender, receiver, gasPrice)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":1,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTTransfer(token, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":2,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":3}");

    tx = txFactory
            .createESDTTransfer(token, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":413000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAx\",\"chainID\":\"1\",\"version\":4,\"options\":5}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createESDTTransfer(token, 4, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(5)
            .withOptions(6)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":434000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAxQDY2NmY2ZkA2MjZmNmY=\",\"chainID\":\"1\",\"version\":5,\"options\":6}");

    tx = txFactory
            .createESDTTransfer(token, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":5,\"value\":\"0\",\"receiver\":\"erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":434000,\"data\":\"RVNEVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDAxQDY2NmY2ZkA2MjZmNmY=\",\"signature\":\"04ebcbdb54da6799bf07be1cae2df930bea8de2c4489cb054db9e1e199f68c14ae9136b244c5d0c88c0b0d455467c27c7ef655026623e4ac06c523a26fe30c07\",\"chainID\":\"1\",\"version\":6,\"options\":7}");
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTNFTTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createESDTNFTTransfer(token, 1, sender, receiver, gasPrice)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":1,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTNFTTransfer(token, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":2,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":3}");

    tx = txFactory
            .createESDTNFTTransfer(token, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":4,\"options\":5}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createESDTNFTTransfer(token, 4, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(5)
            .withOptions(6)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1240500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODNANjY2ZjZmQDYyNmY2Zg==\",\"chainID\":\"1\",\"version\":5,\"options\":6}");

    tx = txFactory
            .createESDTNFTTransfer(token, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":5,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1240500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODNANjY2ZjZmQDYyNmY2Zg==\",\"signature\":\"ba179ed73d36f06b11d492cc8bb38bbc49511d51271b7e76e051efb6fef2960d3c83f4be2875d2cc9105cbb878b85c0846e8b6b458ce334c96633bf7a788d108\",\"chainID\":\"1\",\"version\":6,\"options\":7}");
    EXPECT_TRUE(tx.verify());
}
