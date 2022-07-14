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

TEST(TransactionFactory, createMultiESDTNFT)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    TokenPayment token1 = TokenPayment::nonFungible("ERDCPP-38f249", 3);
    TokenPayment token2 = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    std::vector<TokenPayment> tokens = {token1, token2};
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 1, sender, receiver, gasPrice)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":1,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":2,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":3}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":4,\"options\":5}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 4, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(5)
            .withOptions(6)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2302000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMUA2NjZmNmZANjI2ZjZm\",\"chainID\":\"1\",\"version\":5,\"options\":6}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":5,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2302000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMUA2NjZmNmZANjI2ZjZm\",\"signature\":\"b20db92ec61c498bcfe2460c01bba12e8fce6adcdb5d7be98175d741dbf8b33e0f71b9a987eb823a085817950b265e1186114ce7a5deb5782d4baf3cbf3bf80d\",\"chainID\":\"1\",\"version\":6,\"options\":7}");
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTIssue)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    Address sender = pem.getAddress();
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createESDTIssue(1, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":1,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYQ==\",\"chainID\":\"1\",\"version\":1}");

    ESDTProperties esdtProperties;
    esdtProperties.canAddSpecialRoles = true;
    esdtProperties.canChangeOwner = true;
    tx = txFactory
            .createESDTIssue(2, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":2,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NUA2MzYxNmU1NDcyNjE2ZTczNjY2NTcyNGU0NjU0NDM3MjY1NjE3NDY1NTI2ZjZjNjVANjY2MTZjNzM2NQ==\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTIssue(3, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(4)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NUA2MzYxNmU1NDcyNjE2ZTczNjY2NTcyNGU0NjU0NDM3MjY1NjE3NDY1NTI2ZjZjNjVANjY2MTZjNzM2NQ==\",\"chainID\":\"1\",\"version\":4}");

    tx = txFactory
            .createESDTIssue(4, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(5)
            .withOptions(6)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NUA2MzYxNmU1NDcyNjE2ZTczNjY2NTcyNGU0NjU0NDM3MjY1NjE3NDY1NTI2ZjZjNjVANjY2MTZjNzM2NQ==\",\"chainID\":\"1\",\"version\":5,\"options\":6}");

    tx = txFactory
            .createESDTIssue(5, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":5,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NUA2MzYxNmU1NDcyNjE2ZTczNjY2NTcyNGU0NjU0NDM3MjY1NjE3NDY1NTI2ZjZjNjVANjY2MTZjNzM2NQ==\",\"signature\":\"eb689a5831471b476468ab2598053cde9c7ebb690e1dfe3d6fa87de2f387bee87828dcee3f8acc67d090ba35a16397a5a958e4114eb546f20c52d5728cb0b500\",\"chainID\":\"1\",\"version\":6,\"options\":7}");
    EXPECT_TRUE(tx.verify());
}
