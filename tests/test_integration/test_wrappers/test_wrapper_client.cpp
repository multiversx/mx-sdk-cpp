#include "gtest/gtest.h"
#include "wrappers/httpwrapper.h"
#include "wrappers/jsonwrapper.h"

#define LOCAL_PROXY_URL std::string("http://127.0.0.1:7950")

TEST(ClientWrapper, get_validSubDomain_validRequest)
{
    wrapper::http::Client client(LOCAL_PROXY_URL);
    wrapper::http::Result res = client.get("/address/erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, STATUS_CODE_OK);
    EXPECT_EQ(res.statusMessage, "OK");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("success"));

    auto json = nlohmann::json::parse(res.body);
    EXPECT_EQ(json["error"], "");
    EXPECT_EQ(json["data"]["account"]["address"], "erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");
}

TEST(ClientWrapper, get_validSubDomain_invalidRequest)
{
    wrapper::http::Client client(LOCAL_PROXY_URL);
    wrapper::http::Result res = client.get("/address/erd1qqqqqq");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 500);
    EXPECT_EQ(res.statusMessage, "Internal Server Error");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("error"));

    auto json = nlohmann::json::parse(res.body);
    EXPECT_NE(json["error"], "");
}

TEST(ClientWrapper, get_invalidSubDomain)
{
    wrapper::http::Client client(LOCAL_PROXY_URL);
    wrapper::http::Result res = client.get("/address_INVALID_PATH/erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqplllst77y4l");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 404);
    EXPECT_EQ(res.statusMessage, "Not Found");
}

TEST(ClientWrapper, post_validSubDomain_badRequest)
{
    wrapper::http::Client client(LOCAL_PROXY_URL);
    wrapper::http::Result res = client.post("/transaction/send", "test");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 400);
    EXPECT_EQ(res.statusMessage, "Bad Request");

    EXPECT_FALSE(res.body.empty());
    EXPECT_TRUE(res.body.find("error"));

    auto json = nlohmann::json::parse(res.body);
    EXPECT_NE(json["error"], "");
}

TEST(ClientWrapper, post_invalidSubDomain)
{
    wrapper::http::Client client(LOCAL_PROXY_URL);
    wrapper::http::Result res = client.post("/transaction/send_INVALID_PATH", "test");

    EXPECT_FALSE(res.error);
    EXPECT_EQ(res.status, 404);
    EXPECT_EQ(res.statusMessage, "Not Found");
}
