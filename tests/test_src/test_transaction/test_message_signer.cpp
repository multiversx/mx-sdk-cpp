#include "gtest/gtest.h"

#include "utils/hex.h"
#include "transaction/messagesigner.h"

// See also: https://github.com/multiversx/mx-chain-go/blob/7d133422048b7e65f0a8730cfdd1fff9d51077e9/examples/messageSign_test.go

TEST(MessageSigner, getSignature_verify)
{
    Address const address("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th");
    std::string const message = "custom message of Alice";
    bytes const seed = util::hexToBytes("413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9");

    MessageSigner signer(seed);
    std::string const signature = signer.getSignature(message);

    EXPECT_EQ(signature, util::hexToString("b83647b88cdc7904895f510250cc735502bf4fd86331dd1b76e078d6409433753fd6f619fc7f8152cf8589a4669eb8318b2e735e41309ed3b60e64221d814f08"));

    EXPECT_TRUE(signer.verify(signature, message));
    EXPECT_TRUE(MessageSigner::verify(signature, message, address));
    EXPECT_TRUE(MessageSigner::verify(signature, message, address.getPublicKey()));
}
