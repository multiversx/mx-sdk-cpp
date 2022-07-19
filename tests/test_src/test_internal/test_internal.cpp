#include "gtest/gtest.h"

#include "internal/biguint.h"

struct bigUIntData
{
    std::string decValue;
    std::string hexValue;
    bool valid;
};

class BigUIntParametrized : public ::testing::TestWithParam<bigUIntData>
{
};

INSTANTIATE_TEST_SUITE_P

(
        ValidData,
        BigUIntParametrized,
        ::testing::Values
                (bigUIntData{"10", "0a", true},
                 bigUIntData{"11", "0b", true},
                 bigUIntData{"48", "30", true},
                 bigUIntData{"12", "0c", true},
                 bigUIntData{"1000000", "0f4240", true},
                 bigUIntData{"999999999999999999999999999999999999999999999", "2cd76fe086b93ce2f768a00b229fffffffffff", true}));

INSTANTIATE_TEST_SUITE_P

(
        InvalidData,
        BigUIntParametrized,
        ::testing::Values
                (bigUIntData{"abc", "", false},
                 bigUIntData{"-11", "", false},
                 bigUIntData{"-11.1", "", false},
                 bigUIntData{"11.1", "", false},
                 bigUIntData{"10,3", "", false},
                 bigUIntData{"103f", "", false},
                 bigUIntData{"123!", "", false}));

TEST_P(BigUIntParametrized, differentValues)
{
    bigUIntData const &currParam = GetParam();

    if (currParam.valid)
    {
        BigUInt number(currParam.decValue);
        EXPECT_EQ(number.getValue(), currParam.decValue);
        EXPECT_EQ(number.getHexValue(), currParam.hexValue);
    }
    else
    {
        EXPECT_THROW(BigUInt(currParam.decValue), std::invalid_argument);
    }
}

TEST(BigUInt, multiply)
{
    BigUInt v1("111222333444");
    BigUInt v2("333222111000");
    BigUInt v3 = v1 * v2;

    EXPECT_EQ(v1.getValue(), "111222333444"); // v1's internal value has not change
    EXPECT_EQ(v2.getValue(), "333222111000"); // v2's internal value has not change

    EXPECT_TRUE(v3 == BigUInt("37061740740555580284000"));
    EXPECT_TRUE(v3 * BigUInt("0") == BigUInt("0"));
    EXPECT_EQ(v3.getValue(), "37061740740555580284000"); // v3's internal value has not change
}

TEST(BigUInt, divmod)
{
    BigUInt v1("123");
    BigUInt v2("11");

    std::pair<BigUInt, BigUInt> v3 = v1.divmod(v2);
    EXPECT_EQ(v1.getValue(), "123"); // v1's internal value has not change
    EXPECT_EQ(v2.getValue(), "11"); // v2's internal value has not change
    EXPECT_TRUE(v3.first == BigUInt("11"));
    EXPECT_TRUE(v3.second == BigUInt("2"));

    v3 = v1.divmod(BigUInt("1000"));
    EXPECT_TRUE(v3.first == BigUInt("0"));
    EXPECT_TRUE(v3.second == BigUInt("123"));

    EXPECT_THROW(BigUInt("123").divmod(BigUInt("0")), std::exception);
}

TEST(BigUInt, div)
{
    BigUInt v1(145);
    BigUInt v2(12);

    BigUInt v3 = v1 / (v2);
    EXPECT_EQ(v1.getValue(), "145"); // v1's internal value has not change
    EXPECT_EQ(v2.getValue(), "12"); // v2's internal value has not change
    EXPECT_TRUE(v3 == BigUInt(12));

    EXPECT_TRUE(BigUInt(4) / BigUInt(4) == BigUInt(1));
    EXPECT_THROW(BigUInt(4) / BigUInt(0), std::exception);
}

TEST(BigUInt, greater_less)
{
    BigUInt v1(145);
    BigUInt v2(12);

    EXPECT_TRUE(v1 > v2);
    EXPECT_FALSE(v1 < v2);

    EXPECT_FALSE(v2 > v1);
    EXPECT_TRUE(v2 < v1);

    EXPECT_FALSE(v1 > v1);
    EXPECT_FALSE(v1 < v1);

    EXPECT_EQ(v1.getValue(), "145"); // v1's internal value has not change
    EXPECT_EQ(v2.getValue(), "12"); // v2's internal value has not change
}

TEST(BigUInt, add_substract)
{
    BigUInt v1(145);
    BigUInt v2(12);

    EXPECT_EQ(v1 + BigUInt(0), BigUInt(145));
    EXPECT_EQ(v1 + v2, BigUInt(157));

    EXPECT_EQ(v1 - BigUInt(0), BigUInt(145));
    EXPECT_EQ(v1 - v2, BigUInt(133));
    EXPECT_THROW(v2 - v1, std::exception); // underflow

    EXPECT_EQ(v1.getValue(), "145"); // v1's internal value has not change
    EXPECT_EQ(v2.getValue(), "12"); // v2's internal value has not change
}
