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