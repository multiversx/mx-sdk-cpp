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

TEST(TransactionFactory, createESDTNFTTransfer)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    uint64_t nonce = 123;
    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createESDTNFTTransfer(token, nonce, sender, receiver, gasPrice)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTNFTTransfer(token, nonce, sender, receiver, gasPrice)
            .withVersion(321)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":321}");

    tx = txFactory
            .createESDTNFTTransfer(token, nonce, sender, receiver, gasPrice)
            .withVersion(321)
            .withOptions(432)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1219500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODM=\",\"chainID\":\"1\",\"version\":321,\"options\":432}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createESDTNFTTransfer(token, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(321)
            .withOptions(432)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1240500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODNANjY2ZjZmQDYyNmY2Zg==\",\"chainID\":\"1\",\"version\":321,\"options\":432}");

    tx = txFactory
            .createESDTNFTTransfer(token, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(321)
            .withOptions(432)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":1240500,\"data\":\"RVNEVE5GVFRyYW5zZmVyQDQ1NTI0NDQzNTA1MDJkMzMzODY2MzIzNDM5QDA0QDAxQDAwMDAwMDAwMDAwMDAwMDAwNTAwMWUyYTE0MjhkZDFlM2E1MTQ2YjM5NjBkOWUwZjRhNTAzNjk5MDRlZTU0ODNANjY2ZjZmQDYyNmY2Zg==\",\"signature\":\"1d97ab70664cb1faf52e4f212f7ae03d01d8bdab81f791cc62298bc525f2fcad7d11d2239d20c8797c7221415d6a9d711fed29129a8e5adcde8dafc2311aee08\",\"chainID\":\"1\",\"version\":321,\"options\":432}");
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createMultiESDTNFT)
{
    PemFileReader pem(getCanonicPath("testData/alicePem.pem"));

    uint64_t nonce = 123;
    TokenPayment token1 = TokenPayment::nonFungible("ERDCPP-38f249", 3);
    TokenPayment token2 = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    std::vector<TokenPayment> tokens = {token1, token2};
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createMultiESDTNFTTransfer(tokens, nonce, sender, receiver, gasPrice)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, nonce, sender, receiver, gasPrice)
            .withVersion(321)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":321}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, nonce, sender, receiver, gasPrice)
            .withVersion(321)
            .withOptions(222)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2281000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMQ==\",\"chainID\":\"1\",\"version\":321,\"options\":222}");

    SCArguments args;
    args.add("boo");
    ContractCall contractCall("foo");
    contractCall.setArgs(args);
    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(321)
            .withOptions(222)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2302000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMUA2NjZmNmZANjI2ZjZm\",\"chainID\":\"1\",\"version\":321,\"options\":222}");

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, nonce, sender, receiver, gasPrice)
            .withContractCall(contractCall)
            .withVersion(321)
            .withOptions(222)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":123,\"value\":\"0\",\"receiver\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":2302000,\"data\":\"TXVsdGlFU0RUTkZUVHJhbnNmZXJAMDAwMDAwMDAwMDAwMDAwMDA1MDAxZTJhMTQyOGRkMWUzYTUxNDZiMzk2MGQ5ZTBmNGE1MDM2OTkwNGVlNTQ4M0AwMkA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwM0AwMUA0NTUyNDQ0MzUwNTAyZDMzMzg2NjMyMzQzOUAwNEAwMUA2NjZmNmZANjI2ZjZm\",\"signature\":\"b65d9107c745a2776094f152242adac50fef50ee59a68e2472c3212cf42d8fead653cd487d8116c90a239a4e50b14a151c03019510f26fca454c2b4cfc95b006\",\"chainID\":\"1\",\"version\":321,\"options\":222}");
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
              "{\"nonce\":2,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NQ==\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTIssue(3, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":3,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NQ==\",\"chainID\":\"1\",\"version\":1}");

    tx = txFactory
            .createESDTIssue(4, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":4,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NQ==\",\"chainID\":\"1\",\"version\":3}");

    tx = txFactory
            .createESDTIssue(5, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(4)
            .withOptions(3)
            .build();
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":5,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NQ==\",\"chainID\":\"1\",\"version\":4,\"options\":3}");

    tx = txFactory
            .createESDTIssue(6, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(5)
            .withOptions(4)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx.serialize(),
              "{\"nonce\":6,\"value\":\"50000000000000000\",\"receiver\":\"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u\",\"sender\":\"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th\",\"gasPrice\":99999,\"gasLimit\":60000000,\"data\":\"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0ZDY5NmU3NEA2NjYxNmM3MzY1QDYzNjE2ZTQyNzU3MjZlQDY2NjE2YzczNjVANjM2MTZlNDM2ODYxNmU2NzY1NGY3NzZlNjU3MkA3NDcyNzU2NUA2MzYxNmU1NTcwNjc3MjYxNjQ2NUA2NjYxNmM3MzY1QDYzNjE2ZTQxNjQ2NDUzNzA2NTYzNjk2MTZjNTI2ZjZjNjU3M0A3NDcyNzU2NQ==\",\"signature\":\"613f51f93bfb53243596db21ede7a0cb81a9225fbe3e07f9e6f1e8ab12d89057c5b73000fa3c44f5a91373b2fce2660781077c80d084fd1ad8a5f5ba7c0a7001\",\"chainID\":\"1\",\"version\":5,\"options\":4}");
    EXPECT_TRUE(tx.verify());
}
