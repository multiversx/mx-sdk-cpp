#include "gtest/gtest.h"

#include "utils/hex.h"
#include "utils/errors.h"
#include "transaction/signer.h"
#include "transaction/transaction.h"


class SignerConstructorFixture : public ::testing::Test
{
public:
    template <typename ErrorType>
    void expectException(bytes const &seed, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             Signer signer(seed);
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }
};

TEST_F(SignerConstructorFixture, invalidSeed)
{
    bytes const validSeed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    EXPECT_NO_THROW(Signer signer(validSeed));

    bytes invalidSeed(validSeed);
    invalidSeed.emplace_back(4);

    expectException<std::length_error>(invalidSeed, ERROR_MSG_KEY_BYTES_SIZE);
}

TEST(Signer, getSignature)
{
    bytes seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    std::string msg = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    std::string signature = signer.getSignature(msg);
    std::string expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";

    EXPECT_EQ(util::stringToHex(signature), expectedSignature);
}

struct txData
{
    std::string signerSeed;
    uint64_t nonce;
    std::string value;
    std::shared_ptr<bytes> receiverUserName;
    std::shared_ptr<bytes> senderUserName;
    Address receiver;
    Address sender;
    uint64_t gasPrice;
    uint64_t gasLimit;
    std::string chainID;
    uint64_t version;
    std::string data;
    std::shared_ptr<std::string> signature;
    std::shared_ptr<uint32_t> options;
    std::string expectedSerialized;
    std::string expectedSignature;
};

class TransactionParametrized : public ::testing::TestWithParam<txData> {};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionParametrized,
        ::testing::Values
        /* Signer seed         */ (txData {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */          0,
        /* Value               */          "0",
        /* Receiver username   */          nullptr,
        /* Sender username     */          nullptr,
        /* Receiver            */          Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */          Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "1",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "foo",
        /* Signature           */          nullptr,
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */          "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00"}
));  

TEST_P(TransactionParametrized, getSignature_serialize)
{
    txData const& currParam = GetParam();

    Transaction transaction;
    transaction.m_nonce =  currParam.nonce;
    transaction.m_value =  currParam.value;
    transaction.m_receiverUserName =  currParam.receiverUserName;
    transaction.m_senderUserName =  currParam.senderUserName;
    transaction.m_receiver =  std::make_shared<Address>(currParam.receiver);
    transaction.m_sender =  std::make_shared<Address>(currParam.sender);
    transaction.m_gasPrice =  currParam.gasPrice;
    transaction.m_gasLimit =  currParam.gasLimit;
    transaction.m_data =  std::make_shared<bytes>(currParam.data.begin(),currParam.data.end());
    transaction.m_chainID =  currParam.chainID;
    transaction.m_version =  currParam.version;
    transaction.m_signature =  currParam.signature;
    transaction.m_options =  currParam.options;

    EXPECT_EQ(transaction.serialize(), currParam.expectedSerialized);

    bytes const signerSeed = util::hexToBytes(currParam.signerSeed);
    Signer const signer(signerSeed);
    transaction.sign(signer);

    EXPECT_EQ((*transaction.m_signature), currParam.expectedSignature);
}















TEST(Signer, getSignature2)
{
    bytes seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    Address sender("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");
    Address receiver("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r");

    std::string const dataStr = "foo";
    auto data  = std::make_shared<bytes>(dataStr.begin(),dataStr.end());
    Transaction transaction(0, "0", receiver, sender, DEFAULT_SENDER_NAME, DEFAULT_RECEIVER_NAME, 1000000000, 50000, data, DEFAULT_SIGNATURE, DEFAULT_CHAIN_ID, DEFAULT_VERSION, DEFAULT_OPTIONS);
    std::string expectedSerializedTx = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    EXPECT_EQ(expectedSerializedTx,transaction.serialize());

    std::string expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";
    std::string actualSignature = signer.getSignature(expectedSerializedTx);
    EXPECT_EQ(util::stringToHex(actualSignature),expectedSignature);
}



