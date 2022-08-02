#include "gtest/gtest.h"

#include "utils/hex.h"
#include "utils/errors.h"
#include "transaction/signer.h"
#include "wrappers/cryptosignwrapper.h"


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
    bytes const seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    std::string msg = R"({"nonce":0,"value":"0","receiver":"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r","sender":"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1})";
    std::string signature = signer.getSignature(msg);
    std::string expectedSignature = "b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00";

    EXPECT_EQ(util::stringToHex(signature), expectedSignature);
}

TEST(Signer, verify_signature_by_signer)
{
    bytes const seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    Signer signer(seed);

    std::string msg = R"({"nonce":0,"value":"0","receiver":"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r","sender":"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1})";
    std::string signature = util::hexToString("b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00");

    EXPECT_TRUE(signer.verify(signature, msg));
}

TEST(Signer, verify_signature_by_publicKey)
{
    bytes const seed = util::hexToBytes("1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf");
    bytes const secretKey = wrapper::crypto::getSecretKey(seed);
    bytes const publicKey = wrapper::crypto::getPublicKey(secretKey);

    std::string msg = R"({"nonce":0,"value":"0","receiver":"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r","sender":"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz","gasPrice":1000000000,"gasLimit":50000,"data":"Zm9v","chainID":"1","version":1})";
    std::string signature = util::hexToString("b5fddb8c16fa7f6123cb32edc854f1e760a3eb62c6dc420b5a4c0473c58befd45b621b31a448c5b59e21428f2bc128c80d0ee1caa4f2bf05a12be857ad451b00");

    EXPECT_TRUE(Signer::verify(signature, msg, publicKey));
}

TEST(Signer, verify_signature_by_address)
{
    Address signerAddr("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");

    std::string msg = R"({"nonce":8,"value":"10000000000000000000","receiver":"erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r","sender":"erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz","gasPrice":1000000000,"gasLimit":50000,"chainID":"1","version":1})";
    std::string signature = util::hexToString("4a6d8186eae110894e7417af82c9bf9592696c0600faf110972e0e5310d8485efc656b867a2336acec2b4c1e5f76c9cc70ba1803c6a46455ed7f1e2989a90105");

    EXPECT_TRUE(Signer::verify(signature, msg, signerAddr));
}
