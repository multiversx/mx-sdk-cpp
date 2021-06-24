#include "gtest/gtest.h"

#include "provider/apiresponse.h"

TEST(ErdGenericApiResponse, constructor_validData)
{
    EXPECT_NO_THROW(ErdGenericApiResponse apiResponse(R"({"data":"something","error":"some error","code":"some code"})"));
}

TEST(ErdGenericApiResponse, constructor_invalidData)
{
    EXPECT_THROW({
                     try
                     {
                         ErdGenericApiResponse apiResponse(R"(Invalid json)");
                     }
                     catch(const std::invalid_argument &e)
                     {
                         EXPECT_EQ( ERROR_MSG_JSON_SERIALIZED + R"(Invalid json)", e.what() );
                         throw;
                     }
                 }, std::invalid_argument );
}

TEST(ErdGenericApiResponse, getters_allExist)
{
    ErdGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"error":"some error","code":"some code"})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_EQ(apiResponse.getError(), "some error");
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_missingCode)
{
    ErdGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"error":"some error"})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_THROW({
                     try
                     {
                         apiResponse.getCode();
                     }
                     catch(const std::invalid_argument &e)
                     {
                         EXPECT_EQ( ERROR_MSG_JSON_KEY_NOT_FOUND + "code", e.what() );
                         throw;
                     }
                 }, std::invalid_argument );
    EXPECT_EQ(apiResponse.getError(), "some error");
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_missingError)
{
    ErdGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"code":"some code"})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_TRUE(apiResponse.getError().empty());
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_emptyError)
{
    ErdGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"code":"some code","error":""})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_TRUE(apiResponse.getError().empty());
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_missingData)
{
    ErdGenericApiResponse apiResponse(R"({"error":"some error","code":"some code"})");

    EXPECT_THROW({
                     try
                     {
                         apiResponse.getData<nlohmann::json>();
                     }
                     catch(const std::invalid_argument &e)
                     {
                         EXPECT_EQ( ERROR_MSG_JSON_KEY_NOT_FOUND + "data", e.what() );
                         throw;
                     }
                 }, std::invalid_argument );
    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_EQ(apiResponse.getError(), "some error");
}
