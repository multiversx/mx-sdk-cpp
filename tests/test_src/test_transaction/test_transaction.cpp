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
    std::string receiverUserName;
    std::string senderUserName;
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

class TransactionSerializeParametrized : public ::testing::TestWithParam<txData> {};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionSerializeParametrized,
        ::testing::Values(
        /* Signer seed         */  txData {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */          0,
        /* Value               */          "0",
        /* Receiver username   */          "",
        /* Sender username     */          "",
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
        /* Expected signature  */          "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00"},

        /* Signer seed         */  txData {"1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf",
        /* Nonce               */          8,
        /* Value               */          "10000000000000000000",
        /* Receiver username   */          "",
        /* Sender username     */          "",
        /* Receiver            */          Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */          Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "1",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "",
        /* Signature           */          nullptr,
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */          "4a6d8186eae110894e7417af82c9bf9592696c0600faf110972e0e5310d8485efc656b867a2336acec2b4c1e5f76c9cc70ba1803c6a46455ed7f1e2989a90105"}
                                           ));


TEST_P(TransactionSerializeParametrized, getSignature_serialize)
{
    txData const& currParam = GetParam();

    Transaction transaction;
    transaction.m_nonce =  currParam.nonce;
    transaction.m_value =  currParam.value;
    transaction.m_receiverUserName =  (currParam.receiverUserName.empty()) ? nullptr : std::make_shared<bytes>(currParam.receiverUserName.begin(),currParam.receiverUserName.end());
    transaction.m_senderUserName =  (currParam.senderUserName.empty()) ? nullptr : std::make_shared<bytes>(currParam.senderUserName.begin(),currParam.senderUserName.end()) ;;
    transaction.m_receiver =  std::make_shared<Address>(currParam.receiver);
    transaction.m_sender =  std::make_shared<Address>(currParam.sender);
    transaction.m_gasPrice =  currParam.gasPrice;
    transaction.m_gasLimit =  currParam.gasLimit;
    transaction.m_data =  (currParam.data.empty()) ? nullptr : std::make_shared<bytes>(currParam.data.begin(),currParam.data.end()) ;
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


class TransactionDeserializeParametrized : public ::testing::TestWithParam<txData>
{
public:
    void EXPECT_PTR_BYTE_EQ_STR(std::shared_ptr<bytes> const &txData, std::string const &paramData)
    {
        if (paramData.empty())
        {
            EXPECT_EQ(txData, nullptr);
        }
        else
        {
            EXPECT_EQ(*txData, bytes(paramData.begin(), paramData.end()));
        }
    }

    template <class T>
    void EXPECT_PTR_EQ(std::shared_ptr<T> const &txData, std::shared_ptr<T>const &paramData)
    {
        if (paramData == nullptr)
        {
            EXPECT_EQ(txData, nullptr);
        }
        else
        {
            EXPECT_EQ(*txData, *paramData);
        }
    }
};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        TransactionDeserializeParametrized,
        ::testing::Values(
        /* Signer seed         */ txData {"",
        /* Nonce               */         0,
        /* Value               */         "0",
        /* Receiver username   */         "",
        /* Sender username     */         "",
        /* Receiver            */         Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */         Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */         1000000000,
        /* Gas limit           */         50000,
        /* Chain ID            */         "1",
        /* Version             */         DEFAULT_VERSION,
        /* Data                */         "foo",
        /* Signature           */         nullptr,
        /* Options             */         DEFAULT_OPTIONS,
        /* Expected serialized */         "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */         ""},

        /* Signer seed         */  txData {"",
        /* Nonce               */          8,
        /* Value               */          "10000000000000000000",
        /* Receiver username   */          "",
        /* Sender username     */          "",
        /* Receiver            */          Address("erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"),
        /* Sender              */          Address("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "1",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "",
        /* Signature           */          nullptr,
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":8,\"value\":\"10000000000000000000\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"chainID\":\"1\",\"version\":1}",
        /* Expected signature  */          ""},

        /* Signer seed         */  txData {"",
        /* Nonce               */          5,
        /* Value               */          "10000000000000000000",
        /* Receiver username   */          "",
        /* Sender username     */          "",
        /* Receiver            */          Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender              */          Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "T",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "test",
        /* Signature           */          std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}",
        /* Expected signature  */          ""},

        /* Signer seed         */  txData {"",
        /* Nonce               */          5,
        /* Value               */          "10000000000000000000",
        /* Receiver username   */          "",
        /* Sender username     */          "",
        /* Receiver            */          Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender              */          Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "T",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "test",
        /* Signature           */          std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}",
        /* Expected signature  */          ""},

        /* Signer seed         */  txData {"",
        /* Nonce               */          5,
        /* Value               */          "10000000000000000000",
        /* Receiver username   */          "Jon",
        /* Sender username     */          "Doe",
        /* Receiver            */          Address("erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt"),
        /* Sender              */          Address("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4"),
        /* Gas price           */          1000000000,
        /* Gas limit           */          50000,
        /* Chain ID            */          "T",
        /* Version             */          DEFAULT_VERSION,
        /* Data                */          "test",
        /* Signature           */          std::make_shared<std::string>("62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706"),
        /* Options             */          DEFAULT_OPTIONS,
        /* Expected serialized */          "{\"nonce\":5,\"value\":\"10000000000000000000\",\"receiver\":\"erd10536tc3s886yqxtln74u6mztuwl5gy9k9gp8fttxda0klgxg979srtg5wt\",\"sender\":\"erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4\",\"receiverUsername\":\"Sm9u\",\"senderUsername\":\"RG9l\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"dGVzdA==\",\"signature\":\"62af8fa927e4f1ebd64fb8d7cca8aac9d5d33fefa4b185d44bb16ecefc2a7214304b4654406fe76fa36207fbb91f245586f66500cc554a3eb798faab8c435706\",\"chainID\":\"T\",\"version\":1}",
        /* Expected signature  */          ""}
                ));



TEST_P(TransactionDeserializeParametrized, deserialize)
{
    txData const& currParam = GetParam();

    Transaction transaction;
    transaction.deserialize(currParam.expectedSerialized);

    EXPECT_EQ(transaction.m_nonce,  currParam.nonce);
    EXPECT_EQ(transaction.m_value,  currParam.value);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_receiverUserName,  currParam.receiverUserName);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_senderUserName,  currParam.senderUserName);
    EXPECT_EQ(transaction.m_receiver->getBech32Address(),  currParam.receiver.getBech32Address());
    EXPECT_EQ(transaction.m_sender->getBech32Address(), currParam.sender.getBech32Address());
    EXPECT_EQ(transaction.m_gasPrice, currParam.gasPrice);
    EXPECT_EQ(transaction.m_gasLimit, currParam.gasLimit);
    EXPECT_PTR_BYTE_EQ_STR(transaction.m_data, currParam.data);
    EXPECT_EQ(transaction.m_chainID,  currParam.chainID);
    EXPECT_EQ(transaction.m_version,  currParam.version);
    EXPECT_PTR_EQ(transaction.m_signature,  currParam.signature);
    EXPECT_EQ(transaction.m_options,  currParam.options);

}


