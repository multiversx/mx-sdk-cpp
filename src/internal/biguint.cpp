#include <utility>

#include "bigint/integer.h"
#include "internal/biguint.h"
#include "errors.h"

#define BASE_10 10
#define BASE_16 16

BigUInt::BigUInt(std::string value) : m_value(std::move(value))
{}

std::string BigUInt::getHexValue() const
{
    std::string ret;

    try
    {
        integer number(m_value, BASE_10);
        number = abs(number);
        ret = number.str(BASE_16);
    }
    catch (...)
    {
        throw std::invalid_argument(ERROR_MSG_VALUE + m_value);
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

std::pair<BigUInt, BigUInt> BigUInt::divmod(const BigUInt &rhs) const
{
    integer v1(m_value, BASE_10);
    integer v2(rhs.getValue(), BASE_10);
    std::pair<integer, integer> res = v1.divmod(v1, v2);

    BigUInt retV1 = BigUInt(res.first.str());
    BigUInt retV2 = BigUInt(res.second.str());

    return {retV1, retV2};
}
