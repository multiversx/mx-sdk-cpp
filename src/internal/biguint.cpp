#include <utility>

#include "bigint/integer.h"
#include "internal/biguint.h"
#include "errors.h"

#define BASE_10 10
#define BASE_16 16

BigUInt::BigUInt(std::string value)
{
    try
    {
        integer number(value, BASE_10);
        if (number.sign() == integer::NEGATIVE)
        {
            throw std::invalid_argument(ERROR_MSG_NEGATIVE_VALUE);
        }
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument(ERROR_MSG_VALUE + value + ", reason: " + e.what());
    }

    m_value = (std::move(value));
}

std::string BigUInt::getHexValue() const
{
    std::string ret;

    try
    {
        integer number(m_value, BASE_10);
        ret = number.str(BASE_16);
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument(
                ERROR_MSG_CANNOT_CONVERT_TO_BASE + std::to_string(BASE_16) +
                ", value: " + m_value +
                ", reason: " + e.what());
    }

    if (ret.size() % 2 != 0)
    {
        ret.insert(ret.begin(), '0');
    }

    return ret;
}

const std::string &BigUInt::getValue() const
{
    return m_value;
}

BigUInt BigUInt::operator*(const BigUInt &rhs)
{
    integer v1(m_value, BASE_10);
    integer v2(rhs.getValue(), BASE_10);
    std::string ret = (v1 * v2).str();

    return BigUInt(ret);
}

bool BigUInt::operator==(const BigUInt &rhs)
{
    return this->m_value == rhs.m_value;
}

std::pair<BigUInt, BigUInt> BigUInt::divmod(const BigUInt &rhs) const
{
    integer v1(m_value, BASE_10);
    integer v2(rhs.getValue(), BASE_10);
    std::pair<integer, integer> res = v1.divmod(v1, v2);

    BigUInt retV1 = BigUInt(res.first.str());
    BigUInt retV2 = BigUInt(res.second.str());

    return {retV1, retV2};
}
