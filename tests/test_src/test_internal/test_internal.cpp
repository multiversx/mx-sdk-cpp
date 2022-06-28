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
