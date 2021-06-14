#include "gtest/gtest.h"

#include "wrappers/jsonwrapper.h"


class OrderedJsonFixture : public ::testing::Test
{
public:
    template <typename ValType, typename ErrorType>
    void expectException(wrapper::json::OrderedJson const &json, std::string const &key, errorMessage const &errMsg)
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
};

TEST_F(OrderedJsonFixture, contains_set_at)
{
    wrapper::json::OrderedJson json;

    std::string const key1 = "key1";
    std::string const key2 = "key2";

    EXPECT_FALSE(json.contains(key1));
    EXPECT_FALSE(json.contains(key2));

    expectException<uint32_t, std::invalid_argument>(json, key1, ERROR_MSG_JSON_KEY_NOT_FOUND + key1);
    expectException<uint32_t, std::invalid_argument>(json, key2, ERROR_MSG_JSON_KEY_NOT_FOUND + key2);

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
