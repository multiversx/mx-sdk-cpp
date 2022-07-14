#include <algorithm>
#include <utility>

#include "errors.h"
#include "transaction/token_payment.h"

namespace internal
{

bool isZero(char c)
{
    return c == '0';
}

bool isPoint(char c)
{
    return c == '.';
}

bool hasLeadingZeroes(const std::string &s)
{
    if (s.length() < 2)
    {
        return false;
    }

    std::string::const_iterator first = s.begin();
    std::string::const_iterator second = first + 1;

    return isZero(*first) && !isPoint(*second);
}

std::string removePoint(const std::string &s)
{
    std::string valWithoutPoint(s);
    valWithoutPoint.erase(std::remove(valWithoutPoint.begin(), valWithoutPoint.end(), '.'), valWithoutPoint.end());

    return valWithoutPoint;
}

bool isValidNumber(const std::string &s)
{
    if (hasLeadingZeroes(s))
    {
        return false;
    }

    std::string val = removePoint(s);
    std::string::const_iterator it = val.begin();
    while (it != val.end() && std::isdigit(*it)) ++it;
    return !val.empty() && it == val.end();
}

BigUInt bigUIntFromVal(std::string value, uint32_t numDecimals)
{
    size_t const pointCt = std::count(value.begin(), value.end(), '.');
    if (pointCt > 1)
    {
        throw std::invalid_argument(ERROR_MSG_VALUE + value);
    }

    if (!isValidNumber(value))
    {
        throw std::invalid_argument(ERROR_MSG_VALUE + value);
    }

    if (pointCt == 0)
    {
        std::string const zeroes(numDecimals, '0');
        return BigUInt(value + zeroes);
    }

    size_t const dotPosition = value.find_last_of('.');
    std::string first = value.substr(0, dotPosition);
    first = (first == "0") ? "" : first;

    std::string second = value.substr(dotPosition + 1, value.length());
    size_t const secondLen = second.length();
    if (secondLen > numDecimals)
    {
        second = second.substr(0, numDecimals);
    }
    else
    {
        int zeroesCt = int(numDecimals - secondLen);
        zeroesCt = std::max(zeroesCt, 0);
        second += std::string(zeroesCt, '0');;
    }

    return BigUInt(first + second);
}

}

TokenPayment::TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
        : m_tokenIdentifier(std::move(tokenIdentifier)),
          m_nonce(nonce),
          m_value(std::move(value)),
          m_numDecimals(numDecimals)
{}

TokenPayment TokenPayment::fungibleFromAmount(std::string tokenIdentifier, std::string amount, uint32_t numDecimals)
{
    BigUInt amountBigUInt = internal::bigUIntFromVal(std::move(amount), numDecimals);
    return {std::move(tokenIdentifier), 0, amountBigUInt, numDecimals};
}

TokenPayment TokenPayment::fungibleFromBigUInt(std::string tokenIdentifier, BigUInt value, uint32_t numDecimals)
{
    return {std::move(tokenIdentifier), 0, std::move(value), numDecimals};
}


TokenPayment TokenPayment::nonFungible(std::string tokenIdentifier, uint64_t nonce)
{
    return {std::move(tokenIdentifier), nonce, BigUInt("1"), 0};
}

TokenPayment TokenPayment::semiFungible(std::string tokenIdentifier, uint64_t nonce, BigUInt quantity)
{
    return {std::move(tokenIdentifier), nonce, std::move(quantity), 0};
}

TokenPayment TokenPayment::metaESDTFromAmount(std::string tokenIdentifier, uint64_t nonce, std::string amount, uint32_t numDecimals)
{
    BigUInt amountBigInt = internal::bigUIntFromVal(std::move(amount), numDecimals);
    return {std::move(tokenIdentifier), nonce, amountBigInt, numDecimals};
}

TokenPayment TokenPayment::metaESDTFromBigUInt(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
{
    return {std::move(tokenIdentifier), nonce, std::move(value), numDecimals};
}

BigUInt TokenPayment::value() const
{
    return m_value;
}

uint64_t TokenPayment::nonce() const
{
    return m_nonce;
}

std::string TokenPayment::tokenIdentifier() const
{
    return m_tokenIdentifier;
}

std::string TokenPayment::toString() const
{
    return m_value.getValue();
}

std::string TokenPayment::toPrettyString() const
{
    BigUInt denominator = internal::bigUIntFromVal("1", m_numDecimals);
    std::pair<BigUInt, BigUInt> divMod = m_value.divmod(denominator);

    int zeroesCt = int(denominator.getValue().length() - divMod.second.getValue().length()) - 1;
    zeroesCt = std::max(zeroesCt, 0);
    std::string zeroes(zeroesCt, '0');
    std::string second = zeroes + divMod.second.getValue();

    return divMod.first.getValue() + "." + second + " " + m_tokenIdentifier;
}
