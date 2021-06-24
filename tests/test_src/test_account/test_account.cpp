#include "gtest/gtest.h"

#include "utils/hex.h"
#include "utils/errors.h"
#include "account/address.h"
#include "account/account.h"

class AddressConstructorFixture : public ::testing::Test
{
public:
    template <typename ConstrInputType, typename ErrorType>
    void expectException(ConstrInputType const &input, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             Address address(input);
                         }
                         catch(const ErrorType &e)
                         {
                             EXPECT_EQ( errMsg, e.what() );
                             throw;
                         }
                     }, ErrorType );
    }
    std::string bech32Address = "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th";
    bytes pkBytes = util::hexToBytes("0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1");
};

TEST_F(AddressConstructorFixture, invalidBech32Address)
{
    EXPECT_NO_THROW(Address adr(bech32Address));

    std::string::const_iterator pos = bech32Address.begin() + 14;
    bech32Address.erase(pos);
    expectException<std::string, std::invalid_argument>(bech32Address, ERROR_MSG_BECH32);
}

TEST_F(AddressConstructorFixture, invalidPublicKey)
{
    EXPECT_NO_THROW(Address adr(pkBytes));

    bytes::const_iterator pos = pkBytes.begin() + 14;
    pkBytes.erase(pos);
    expectException<bytes, std::length_error>(pkBytes, ERROR_MSG_KEY_BYTES_SIZE);
}

struct addrData
{
    std::string publicKey;
    std::string bech32Address;
};

class AddressParametrized : public ::testing::TestWithParam<addrData> {};

INSTANTIATE_TEST_CASE_P (
        ValidData,
        AddressParametrized,
        ::testing::Values
        (addrData {"0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1",   // Public key
                   "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"},    // Bech32 address

         addrData {"8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8",   // Public key
                   "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"},    // Bech32 address

         addrData {"b2a11555ce521e4944e09ab17549d85b487dcd26c84b5017a39e31a3670889ba",   // Public key
                   "erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8"}));  // Bech32 address

TEST_P(AddressParametrized, getPublicKey_getBech32Address)
{
    addrData const& currParam = GetParam();

    bytes const pubKey = util::hexToBytes(currParam.publicKey);
    std::string const bech32Address = currParam.bech32Address;

    Address adr1(pubKey);
    Address adr2(bech32Address);

    EXPECT_EQ(adr1.getPublicKey(), adr2.getPublicKey());
    EXPECT_EQ(adr1.getBech32Address(), adr2.getBech32Address());
}

TEST(Address, comparsionOperator_bech32Address)
{
    Address const adr1("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");
    Address const adr2("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");
    Address const adr3("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th");

    EXPECT_TRUE(adr1 == adr2);
    EXPECT_FALSE(adr1 == adr3);
}

TEST(Address, comparsionOperator_pubKey)
{
    Address const adr1(util::hexToBytes("0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"));
    Address const adr2(util::hexToBytes("0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"));
    Address const adr3("erd1sjsk3n2d0krq3pyxxtgf0q7j3t56sgusqaujj4n82l39t9h7jers6gslr4");

    EXPECT_TRUE(adr1 == adr2);
    EXPECT_FALSE(adr1 == adr3);
}

TEST(Address, comparsionOperator_bech32Address_pubKey)
{
    Address const adr1(util::hexToBytes("0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1"));
    Address const adr2("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th");
    Address const adr3(util::hexToBytes("8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8"));

    EXPECT_TRUE(adr1 == adr2);
    EXPECT_FALSE(adr1 == adr3);
}

TEST(Account, constructor_defaultValues)
{
    std::string const bech32Addr = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx";

    Address const address(bech32Addr);
    Account const account(address);

    EXPECT_TRUE (account.getAddress() == address);
    EXPECT_EQ(account.getBalance(), DEFAULT_BALANCE);
    EXPECT_EQ(account.getNonce(), DEFAULT_NONCE );
}

TEST(Account, constructor_customValues)
{
    std::string const bech32Addr = "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx";

    Address const address(bech32Addr);
    Account const account(address, "123456789", 123456789);

    EXPECT_TRUE (account.getAddress() == address);
    EXPECT_EQ(account.getBalance(), "123456789");
    EXPECT_EQ(account.getNonce(), 123456789 );
}
