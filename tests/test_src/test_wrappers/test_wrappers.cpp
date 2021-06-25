#include "gtest/gtest.h"

#include "utils/hex.h"
#include "account/address.h"
#include "wrappers/jsonwrapper.h"
#include "wrappers/httpwrapper.h"
#include "wrappers/cryptosignwrapper.h"

class OrderedJsonFixture : public ::testing::Test
{
public:
    template <typename ValType, typename ErrorType>
    void expectExceptionAt(wrapper::json::OrderedJson const &json, std::string const &key, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             json.at<ValType>(key);
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }

    template <typename ErrorType>
    void expectExceptionDeserialize(wrapper::json::OrderedJson &json, std::string const &msg, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             json.deserialize(msg);
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }

    template <typename ErrorType>
    void expectExceptionSerialize(wrapper::json::OrderedJson const &json, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             json.serialize();
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }
};

TEST_F(OrderedJsonFixture, contains_set_at)
{
    wrapper::json::OrderedJson json;

    std::string const key1 = "key1";
    std::string const key2 = "key2";

    EXPECT_FALSE(json.contains(key1));
    EXPECT_FALSE(json.contains(key2));

    expectExceptionAt<uint32_t, std::invalid_argument>(json, key1, ERROR_MSG_JSON_KEY_NOT_FOUND + key1);
    expectExceptionAt<uint32_t, std::invalid_argument>(json, key2, ERROR_MSG_JSON_KEY_NOT_FOUND + key2);

    json.set(key1, 1U);
    json.set(key2, 2U);

    EXPECT_TRUE(json.contains(key1));
    EXPECT_TRUE(json.contains(key2));

    EXPECT_NO_THROW(json.at<uint32_t>(key1));
    EXPECT_NO_THROW(json.at<uint32_t>(key2));

    EXPECT_EQ(json.at<uint32_t>(key1), 1U);
    EXPECT_EQ(json.at<uint32_t>(key2), 2U);

    json.set(key1,0U);
    json.set(key2,1U);

    EXPECT_TRUE(json.contains(key1));
    EXPECT_TRUE(json.contains(key2));

    EXPECT_NO_THROW(json.at<uint32_t>(key1));
    EXPECT_NO_THROW(json.at<uint32_t>(key2));

    EXPECT_EQ(json.at<uint32_t>(key1), 0U);
    EXPECT_EQ(json.at<uint32_t>(key2), 1U);
}

TEST_F(OrderedJsonFixture, deserialize)
{
    wrapper::json::OrderedJson json;

    json.deserialize("{\"pi\":3.141,\"happy\":true,\"name\":\"Joe\"}");

    EXPECT_EQ(json.at<double>("pi"), 3.141);
    EXPECT_EQ(json.at<bool>("happy"), true);
    EXPECT_EQ(json.at<std::string>("name"), "Joe");

    expectExceptionDeserialize<std::invalid_argument>(json, "invalid serialized json", ERROR_MSG_JSON_SERIALIZED + "invalid serialized json");
}

TEST_F(OrderedJsonFixture, serialize_empty)
{
    wrapper::json::OrderedJson json;

    EXPECT_TRUE(json.empty());
    expectExceptionSerialize<std::invalid_argument>(json, ERROR_MSG_JSON_SERIALIZE_EMPTY);

    json.set<double>("pi", 3.141);
    json.set<bool>("happy", true);
    json.set<std::string>("name", "Joe");

    EXPECT_FALSE(json.empty());
    EXPECT_EQ(json.serialize(), "{\"pi\":3.141,\"happy\":true,\"name\":\"Joe\"}");
}

TEST(CryptoWrapper, getSecretKey)
{
    bytes const seed = util::hexToBytes("e253a571ca153dc2aee845819f74bcc9773b0586edead15a94cb7235a5027436");
    bytes const pubKey = util::hexToBytes("b2a11555ce521e4944e09ab17549d85b487dcd26c84b5017a39e31a3670889ba");

    bytes expectedSk(seed);
    expectedSk.insert(std::end(expectedSk), std::begin(pubKey), std::end(pubKey));

    bytes const secretKey = wrapper::crypto::getSecretKey(seed);

    EXPECT_EQ(secretKey, expectedSk);
    EXPECT_EQ(secretKey.size(), SECRET_KEY_LENGTH);
}

TEST(CryptoWrapper, getSignature)
{
    bytes const seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    bytes const secretKey = wrapper::crypto::getSecretKey(seed);

    std::string const msg = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    std::string const signature = wrapper::crypto::getSignature(secretKey, msg);
    std::string const expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";

    EXPECT_EQ(signature.size(), SIGNATURE_LENGTH);
    EXPECT_EQ(util::stringToHex(signature), expectedSignature);

}

TEST(CryptoWrapper, getPublicKey)
{
    bytes const seed = util::hexToBytes("413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9");
    bytes const secretKey =  wrapper::crypto::getSecretKey(seed);
    bytes const expectedPublicKey = util::hexToBytes("0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1");

    EXPECT_EQ(wrapper::crypto::getPublicKey(secretKey), expectedPublicKey);
}

TEST(CryptoWrapper, verify)
{
    std::string const message = "{\"nonce\":0,\"value\":\"0\",\"receiver\":\"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r\",\"sender\":\"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz\",\"gasPrice\":1000000000,\"gasLimit\":50000,\"data\":\"Zm9v\",\"chainID\":\"1\",\"version\":1}";
    std::string const signature = util::hexToString("b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00");

    Address signerAddr("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");

    EXPECT_TRUE(wrapper::crypto::verify(signature, message, signerAddr.getPublicKey()));
}

TEST(ClientWrapper, get_validSubDomain_validRequest)
{
    wrapper::http::Client client("https://testnet-gateway.elrond.com");
    wrapper::http::Result res =  client.get("/address/erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, STATUS_CODE_OK);
    EXPECT_EQ(res.statusMessage, "OK");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("success"));

    auto json = nlohmann::json::parse(res.body);

    EXPECT_TRUE(json["error"] == "");
    EXPECT_EQ(json["data"]["account"]["address"], "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
    EXPECT_NE(json["data"]["account"]["balance"], "0");
}

TEST(ClientWrapper, get_validSubDomain_invalidRequest)
{
    wrapper::http::Client client("https://testnet-gateway.elrond.com");
    wrapper::http::Result res =  client.get("/address/erd1qqqqqq");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 500);
    EXPECT_EQ(res.statusMessage, "Internal Server Error");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("error"));

    auto json = nlohmann::json::parse(res.body);

    EXPECT_FALSE(json["error"] == "");
}

TEST(ClientWrapper, get_invalidSubDomain)
{
    wrapper::http::Client client("https://testnet-gateway.elrond.com");
    wrapper::http::Result res =  client.get("/address_INVALID_PATH/erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 404);
    EXPECT_EQ(res.statusMessage, "Not Found");
}


TEST(ClientWrapper, post_validSubDomain_testRequest)
{
    wrapper::http::Client client("https://testnet-gateway.elrond.com");
    wrapper::http::Result res =  client.post("/transaction/send", "test");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 400);
    EXPECT_EQ(res.statusMessage, "Bad Request");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("error"));

    auto json = nlohmann::json::parse(res.body);

    EXPECT_FALSE(json["error"] == "");
}

TEST(ClientWrapper, post_invalidSubDomain)
{
    wrapper::http::Client client("https://testnet-gateway.elrond.com");
    wrapper::http::Result res =  client.post("/transaction/send_INVALID_PATH", "test");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 404);
    EXPECT_EQ(res.statusMessage, "Not Found");
}
