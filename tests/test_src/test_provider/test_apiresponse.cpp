#include "gtest/gtest.h"

#include "provider/apiresponse.h"

TEST(ErdGenericApiResponse, constructor_validData)
{
    EXPECT_NO_THROW(MXGenericApiResponse apiResponse(R"({"data":"something","error":"some error","code":"some code"})"));
}

TEST(ErdGenericApiResponse, constructor_invalidData)
{
    EXPECT_THROW({
                     try
                     {
                         MXGenericApiResponse apiResponse(R"(Invalid json)");
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
    MXGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"error":"some error","code":"some code"})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_EQ(apiResponse.getError(), "some error");
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_missingCode)
{
    MXGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"error":"some error"})");

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
    MXGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"code":"some code"})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_TRUE(apiResponse.getError().empty());
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_emptyError)
{
    MXGenericApiResponse apiResponse(R"({"data":{"account":{"address":"erd1qq"}},"code":"some code","error":""})");

    nlohmann::json expectedData;
    expectedData["account"]["address"] = "erd1qq";

    EXPECT_EQ(apiResponse.getCode(), "some code");
    EXPECT_TRUE(apiResponse.getError().empty());
    EXPECT_EQ(apiResponse.getData<nlohmann::json>(), expectedData);
}

TEST(ErdGenericApiResponse, getters_missingData)
{
    MXGenericApiResponse apiResponse(R"({"error":"some error","code":"some code"})");

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

struct apiResponseData
{
    std::string rawData;
    bool expectSuccessful;
};

class ErdApiResponseCheckOperation : public ::testing::TestWithParam<apiResponseData>
{
public:
    void EXPECT_UNSUCCESSFUL_OPERATION (MXGenericApiResponse const &response)
    {
        EXPECT_THROW({
                         try
                         {
                             response.checkSuccessfulOperation();
                         }
                         catch(const std::runtime_error &e)
                         {
                             std::string errDescription = e.what();
                             EXPECT_TRUE(errDescription.find(ERROR_MSG_HTTP_REQUEST_FAILED) != std::string::npos);
                             EXPECT_TRUE(errDescription.find(response.getCode()) != std::string::npos);
                             EXPECT_TRUE(errDescription.find(ERROR_MSG_REASON) != std::string::npos);
                             EXPECT_TRUE(errDescription.find(response.getError()) != std::string::npos);
                             throw;
                         }
                     }, std::runtime_error);
    }
};

INSTANTIATE_TEST_CASE_P (
        Successful,
        ErdApiResponseCheckOperation,
        ::testing::Values
                (apiResponseData {R"({"data":{"account":{"address":"erd1qq"}},"code":"successful","error":""})", true},
                 apiResponseData {R"({"data":{"account":{"address":"erd1qq"}},"code":"success","error":""})", true},
                 apiResponseData {R"({"data":{"account":{"address":"erd1qq"}},"code":"success"})", true}));

INSTANTIATE_TEST_CASE_P (
        Unsuccessful,
        ErdApiResponseCheckOperation,
        ::testing::Values
                (apiResponseData {R"({"data":{"account":{"address":"erd1qq"}},"code":"fail","error":"some error"})", false},
                 apiResponseData {R"({"data":null,"code":"fail","error":"some error"})", false},
                 apiResponseData {R"({"data":null,"code":"fail","error":""})", false},
                 apiResponseData {R"({"data":null,"code":"fail"})", false},
                 apiResponseData {R"({"data":null,"code":"", "error":""})", false},
                 apiResponseData {R"({"data":null,"code":"success","error":"some error"})", false}));


TEST_P(ErdApiResponseCheckOperation, checkSuccessfulOperation_successful)
{
    apiResponseData const &currParam = GetParam();

    MXGenericApiResponse apiResponse(currParam.rawData);

    if (currParam.expectSuccessful)
    {
        EXPECT_NO_THROW(apiResponse.checkSuccessfulOperation());
    }
    else
    {
        EXPECT_UNSUCCESSFUL_OPERATION(apiResponse);
    }
}

TEST(ErdGenericApiResponse, utility_requireAttribute)
{
    nlohmann::json data;
    data["account"]["address"] = "erd1qq";

    EXPECT_NO_THROW(utility::requireAttribute(data, "account"));
    EXPECT_NO_THROW(utility::requireAttribute(data["account"], "address"));

    EXPECT_THROW({
                     try
                     {
                         utility::requireAttribute(data, "code");
                     }
                     catch(const std::invalid_argument &e)
                     {
                         EXPECT_EQ( ERROR_MSG_JSON_KEY_NOT_FOUND + "code", e.what() );
                         throw;
                     }
                 }, std::invalid_argument );
}
