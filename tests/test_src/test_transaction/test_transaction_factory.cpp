#include "gtest/gtest.h"
#include "filehandler/pemreader.h"
#include "transaction/transaction_factory.h"
#include "test_common.h"

#define NO_OPTION 0

Transaction createTransaction(
        uint64_t nonce,
        BigUInt const &value,
        Address const &sender,
        Address const &receiver,
        uint64_t gasPrice,
        uint64_t gasLimit,
        std::string data,
        std::string const &signature,
        std::string const &chainID,
        uint64_t version,
        uint32_t options)
{
    auto dta = data.empty() ? DEFAULT_DATA  : std::make_shared<bytes>(data.begin(), data.end());
    auto sig = signature.empty() ? DEFAULT_SIGNATURE : std::make_shared<std::string>(signature);
    auto opt = options == NO_OPTION ? DEFAULT_OPTIONS : std::make_shared<uint32_t>(options);

    return Transaction(
            nonce,
            value,
            receiver,
            sender,
            DEFAULT_RECEIVER_NAME,
            DEFAULT_SENDER_NAME,
            gasPrice,
            gasLimit,
            dta,
            sig,
            chainID,
            version,
            opt);
}

TEST(TransactionFactory, createEGLDTransfer)
{
    PemFileReader pem(getCanonicalRootPath("elrond-sdk-erdcpp/tests/testData/alicePem.pem"));
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
    EXPECT_EQ(tx, createTransaction(1, value, sender, receiver, gasPrice, 57500, data, "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createEGLDTransfer(2, value, sender, receiver, gasPrice, data)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx, createTransaction(2, value, sender, receiver, gasPrice, 57500, data, "", cfg.chainId, 3, NO_OPTION));

    tx = txFactory
            .createEGLDTransfer(3, value, sender, receiver, gasPrice, data)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx, createTransaction(3, value, sender, receiver, gasPrice, 57500, data, "", cfg.chainId, 4, 5));

    tx = txFactory
            .createEGLDTransfer(4, value, sender, receiver, gasPrice, data)
            ->withVersion(5)
            .withOptions(6)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(4, value, sender, receiver, gasPrice, 57500, data, "cf77752dc29ed19e986e10a639ee3abb9d936ed9dbd123be1f44f31c9df5ed042fa4164ea12bad43ba497e1adcc8c1ada9b4b35142f7a94e49747b853b542b0d", cfg.chainId, 5, 6));
    EXPECT_TRUE(tx.verify());

    tx = txFactory
            .createEGLDTransfer(5, value, sender, receiver, gasPrice) // no data field
            ->withOptions(1) // options=1; version=2 => should sign keccak(txSerialized)
            .withVersion(2)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(5, value, sender, receiver, gasPrice, 50000, "", "dce93ad3cd16c9fe1d13959a989116d4dd37a6bc30cecb36765317d6eb1a7a918e5fc24c52db5c400c20fc293de940812fe87ea05aef80a05ec9ddf742ec850c", cfg.chainId, 2, 1));
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTTransfer)
{
    PemFileReader pem(getCanonicalRootPath("elrond-sdk-erdcpp/tests/testData/alicePem.pem"));
    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 1);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);

    Transaction tx = txFactory
            .createESDTTransfer(token, 1, sender, receiver, gasPrice)
            ->build();
    EXPECT_EQ(tx, createTransaction(1, DEFAULT_VALUE, sender, receiver, gasPrice, 413000, "ESDTTransfer@4552444350502d333866323439@01", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createESDTTransfer(token, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx, createTransaction(2, DEFAULT_VALUE, sender, receiver, gasPrice, 413000, "ESDTTransfer@4552444350502d333866323439@01", "", cfg.chainId, 3, NO_OPTION));

    tx = txFactory
            .createESDTTransfer(token, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx, createTransaction(3, DEFAULT_VALUE, sender, receiver, gasPrice, 413000, "ESDTTransfer@4552444350502d333866323439@01", "", cfg.chainId, 4, 5));

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
    EXPECT_EQ(tx, createTransaction(4, DEFAULT_VALUE, sender, receiver, gasPrice, 434000, "ESDTTransfer@4552444350502d333866323439@01@666f6f@626f6f", "", cfg.chainId, 5, 6));

    tx = txFactory
            .createESDTTransfer(token, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(5, DEFAULT_VALUE, sender, receiver, gasPrice, 434000, "ESDTTransfer@4552444350502d333866323439@01@666f6f@626f6f", "04ebcbdb54da6799bf07be1cae2df930bea8de2c4489cb054db9e1e199f68c14ae9136b244c5d0c88c0b0d455467c27c7ef655026623e4ac06c523a26fe30c07", cfg.chainId, 6, 7));
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTNFTTransfer)
{
    PemFileReader pem(getCanonicalRootPath("elrond-sdk-erdcpp/tests/testData/alicePem.pem"));
    TokenPayment token = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createESDTNFTTransfer(token, 1, sender, receiver, gasPrice)
            ->build();
    EXPECT_EQ(tx, createTransaction(1, DEFAULT_VALUE, sender, sender, gasPrice, 1219500, "ESDTNFTTransfer@4552444350502d333866323439@04@01@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createESDTNFTTransfer(token, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx, createTransaction(2, DEFAULT_VALUE, sender, sender, gasPrice, 1219500, "ESDTNFTTransfer@4552444350502d333866323439@04@01@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483", "", cfg.chainId, 3, NO_OPTION));

    tx = txFactory
            .createESDTNFTTransfer(token, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx, createTransaction(3, DEFAULT_VALUE, sender, sender, gasPrice, 1219500, "ESDTNFTTransfer@4552444350502d333866323439@04@01@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483", "", cfg.chainId, 4, 5));

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
    EXPECT_EQ(tx, createTransaction(4, DEFAULT_VALUE, sender, sender, gasPrice, 1240500, "ESDTNFTTransfer@4552444350502d333866323439@04@01@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@666f6f@626f6f", "", cfg.chainId, 5, 6));

    tx = txFactory
            .createESDTNFTTransfer(token, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(5, DEFAULT_VALUE, sender, sender, gasPrice, 1240500, "ESDTNFTTransfer@4552444350502d333866323439@04@01@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@666f6f@626f6f", "ba179ed73d36f06b11d492cc8bb38bbc49511d51271b7e76e051efb6fef2960d3c83f4be2875d2cc9105cbb878b85c0846e8b6b458ce334c96633bf7a788d108", cfg.chainId, 6, 7));
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createMultiESDTNFT)
{
    PemFileReader pem(getCanonicalRootPath("elrond-sdk-erdcpp/tests/testData/alicePem.pem"));
    TokenPayment token1 = TokenPayment::nonFungible("ERDCPP-38f249", 3);
    TokenPayment token2 = TokenPayment::nonFungible("ERDCPP-38f249", 4);
    Address sender = pem.getAddress();
    Address receiver("erd1qqqqqqqqqqqqqpgqrc4pg2xarca9z34njcxeur622qmfjp8w2jps89fxnl");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createMultiESDTNFTTransfer({}, 1, sender, receiver, gasPrice) // no payment
            ->build();
    EXPECT_EQ(tx, createTransaction(1, DEFAULT_VALUE, sender, sender, gasPrice, 182000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@00", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createMultiESDTNFTTransfer({token1}, 1, sender, receiver, gasPrice) // one token payment
            ->build();
    EXPECT_EQ(tx, createTransaction(1, DEFAULT_VALUE, sender, sender, gasPrice, 1231500, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@01@4552444350502d333866323439@03@01", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    std::vector<TokenPayment> tokens = {token1, token2};
    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 1, sender, receiver, gasPrice) // two token payments
            ->build();
    EXPECT_EQ(tx, createTransaction(1, DEFAULT_VALUE, sender, sender, gasPrice, 2281000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4552444350502d333866323439@03@01@4552444350502d333866323439@04@01", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 2, sender, receiver, gasPrice)
            ->withVersion(3)
            .build();
    EXPECT_EQ(tx, createTransaction(2, DEFAULT_VALUE, sender, sender, gasPrice, 2281000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4552444350502d333866323439@03@01@4552444350502d333866323439@04@01", "", cfg.chainId, 3, NO_OPTION));

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 3, sender, receiver, gasPrice)
            ->withVersion(4)
            .withOptions(5)
            .build();
    EXPECT_EQ(tx, createTransaction(3, DEFAULT_VALUE, sender, sender, gasPrice, 2281000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4552444350502d333866323439@03@01@4552444350502d333866323439@04@01", "", cfg.chainId, 4, 5));

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
    EXPECT_EQ(tx, createTransaction(4, DEFAULT_VALUE, sender, sender, gasPrice, 2302000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4552444350502d333866323439@03@01@4552444350502d333866323439@04@01@666f6f@626f6f", "", cfg.chainId, 5, 6));

    tx = txFactory
            .createMultiESDTNFTTransfer(tokens, 5, sender, receiver, gasPrice)
            ->withContractCall(contractCall)
            .withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(5, DEFAULT_VALUE, sender, sender, gasPrice, 2302000, "MultiESDTNFTTransfer@000000000000000005001e2a1428dd1e3a5146b3960d9e0f4a50369904ee5483@02@4552444350502d333866323439@03@01@4552444350502d333866323439@04@01@666f6f@626f6f", "b20db92ec61c498bcfe2460c01bba12e8fce6adcdb5d7be98175d741dbf8b33e0f71b9a987eb823a085817950b265e1186114ce7a5deb5782d4baf3cbf3bf80d", cfg.chainId, 6, 7));
    EXPECT_TRUE(tx.verify());
}

TEST(TransactionFactory, createESDTIssue)
{
    PemFileReader pem(getCanonicalRootPath("elrond-sdk-erdcpp/tests/testData/alicePem.pem"));
    Address sender = pem.getAddress();
    Address expectedReceiver("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u");
    uint64_t gasPrice = 99999;

    NetworkConfig cfg = DEFAULT_MAINNET_NETWORK_CONFIG;
    TransactionFactory txFactory(cfg);
    Transaction tx = txFactory
            .createESDTIssue(1, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10)
            ->build();
    EXPECT_EQ(tx, createTransaction(1, BigUInt(ESDT_ISSUANCE_VALUE), sender, expectedReceiver, gasPrice, 60000000, "issue@416c696365@414c43@03e8@0a", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    ESDTProperties esdtProperties;
    esdtProperties.canAddSpecialRoles = true;
    esdtProperties.canChangeOwner = true;
    tx = txFactory
            .createESDTIssue(2, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->build();
    EXPECT_EQ(tx, createTransaction(2, BigUInt(ESDT_ISSUANCE_VALUE), sender, expectedReceiver, gasPrice, 60000000, "issue@416c696365@414c43@03e8@0a@63616e467265657a65@66616c7365@63616e57697065@66616c7365@63616e5061757365@66616c7365@63616e4d696e74@66616c7365@63616e4275726e@66616c7365@63616e4368616e67654f776e6572@74727565@63616e55706772616465@66616c7365@63616e4164645370656369616c526f6c6573@74727565@63616e5472616e736665724e4654437265617465526f6c65@66616c7365", "", cfg.chainId, DEFAULT_VERSION, NO_OPTION));

    tx = txFactory
            .createESDTIssue(3, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(4)
            .build();
    EXPECT_EQ(tx, createTransaction(3, BigUInt(ESDT_ISSUANCE_VALUE), sender, expectedReceiver, gasPrice, 60000000, "issue@416c696365@414c43@03e8@0a@63616e467265657a65@66616c7365@63616e57697065@66616c7365@63616e5061757365@66616c7365@63616e4d696e74@66616c7365@63616e4275726e@66616c7365@63616e4368616e67654f776e6572@74727565@63616e55706772616465@66616c7365@63616e4164645370656369616c526f6c6573@74727565@63616e5472616e736665724e4654437265617465526f6c65@66616c7365", "", cfg.chainId, 4, NO_OPTION));

    tx = txFactory
            .createESDTIssue(4, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(5)
            .withOptions(6)
            .build();
    EXPECT_EQ(tx, createTransaction(4, BigUInt(ESDT_ISSUANCE_VALUE), sender, expectedReceiver, gasPrice, 60000000, "issue@416c696365@414c43@03e8@0a@63616e467265657a65@66616c7365@63616e57697065@66616c7365@63616e5061757365@66616c7365@63616e4d696e74@66616c7365@63616e4275726e@66616c7365@63616e4368616e67654f776e6572@74727565@63616e55706772616465@66616c7365@63616e4164645370656369616c526f6c6573@74727565@63616e5472616e736665724e4654437265617465526f6c65@66616c7365", "", cfg.chainId, 5, 6));

    tx = txFactory
            .createESDTIssue(5, sender, gasPrice, "Alice", "ALC", BigUInt(1000), 10, esdtProperties)
            ->withVersion(6)
            .withOptions(7)
            .buildSigned(pem.getSeed());
    EXPECT_EQ(tx, createTransaction(5, BigUInt(ESDT_ISSUANCE_VALUE), sender, expectedReceiver, gasPrice, 60000000, "issue@416c696365@414c43@03e8@0a@63616e467265657a65@66616c7365@63616e57697065@66616c7365@63616e5061757365@66616c7365@63616e4d696e74@66616c7365@63616e4275726e@66616c7365@63616e4368616e67654f776e6572@74727565@63616e55706772616465@66616c7365@63616e4164645370656369616c526f6c6573@74727565@63616e5472616e736665724e4654437265617465526f6c65@66616c7365", "eb689a5831471b476468ab2598053cde9c7ebb690e1dfe3d6fa87de2f387bee87828dcee3f8acc67d090ba35a16397a5a958e4114eb546f20c52d5728cb0b500", cfg.chainId, 6, 7));
    EXPECT_TRUE(tx.verify());
}
