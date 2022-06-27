#include <utility>
#include <algorithm>

#include "transaction/token_payment.h"

namespace internal
{
BigUInt bigUIntFromVal(double value, uint32_t numDecimals)
{
    std::string valStr = std::to_string(value);
    auto maxDoubleLen = std::to_string(std::numeric_limits<double>::max()).length();
    if (valStr.length() >= maxDoubleLen)
    {
        valStr.erase(std::remove(valStr.begin(), valStr.end(), '.'), valStr.end());
        return BigUInt(valStr);
    }

    auto v = value * std::pow(10, numDecimals);
    auto xxx = std::numeric_limits<uint64_t>::max();
    BigUInt val = BigUInt(std::to_string(uint64_t(v)));
    // std::string zeroes = std::string(numDecimals, '0');
    //  BigUInt multiplier = BigUInt("1" + zeroes);

    return val;// * multiplier;
}

bool is_number(const std::string &s)
{
    // check first numbers are != 0
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

}

TokenPayment::TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
        : m_tokenIdentifier(std::move(tokenIdentifier)),
          m_nonce(nonce),
          m_value(std::move(value)),
          m_numDecimals(numDecimals)
{}

TokenPayment TokenPayment::fungibleFromAmount(std::string tokenIdentifier, std::string value, uint32_t numDecimals)
{
    size_t pointCt = std::count(value.begin(), value.end(), '.');
    if (pointCt > 1)
    {
        //do smth
    }

    std::string valWithoutPoint = value;
    valWithoutPoint.erase(std::remove(valWithoutPoint.begin(), valWithoutPoint.end(), '.'), valWithoutPoint.end());

    if (!internal::is_number(valWithoutPoint))
    {
        // another err
    }

    if (pointCt == 1)
    {
        auto const dotPosition = value.find_last_of('.');
        auto first = value.substr(0, dotPosition);
        first = (first == "0") ? "" : first;
        auto second = value.substr(dotPosition+1, value.length());
        if (second.length() > numDecimals)
        {
            second = second.substr(0, numDecimals);
        }
        else
        {
            int zeroesCt = int(numDecimals - second.length());
            zeroesCt = std::max(zeroesCt, 0);
            std::string zeroes = std::string(zeroesCt, '0');
            second += zeroes;
        }
        return {std::move(tokenIdentifier), 0, BigUInt(first + second), numDecimals};
    }
    std::string zeroes = std::string(numDecimals, '0');
    return  {std::move(tokenIdentifier), 0, BigUInt(value+ zeroes), numDecimals};

   // BigUInt amountBigInt = internal::bigUIntFromVal(value, numDecimals);

}

TokenPayment TokenPayment::fungibleFromBigUInt(std::string tokenIdentifier, BigUInt value, uint32_t numDecimals)
{
    return {std::move(tokenIdentifier), 0, std::move(value), numDecimals};
}


TokenPayment TokenPayment::nonFungible(std::string tokenIdentifier, uint64_t nonce)
{
    return {std::move(tokenIdentifier), nonce, BigUInt("1"), 0};
}

TokenPayment TokenPayment::metaESDTFromAmount(std::string tokenIdentifier, uint64_t nonce, double value, uint32_t numDecimals)
{
    BigUInt amountBigInt = internal::bigUIntFromVal(value, numDecimals);
    return {std::move(tokenIdentifier), nonce, amountBigInt, numDecimals};
}

TokenPayment TokenPayment::metaESDTFromBigUInt(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
{
    return {std::move(tokenIdentifier), nonce, std::move(value), numDecimals};
}

std::string TokenPayment::toString() const
{
    return m_value.getValue();
}

std::string TokenPayment::toPrettyString() const
{
    BigUInt denominator = internal::bigUIntFromVal(1, m_numDecimals);
    std::pair<BigUInt, BigUInt> divMod = m_value.divmod(denominator);

    int zeroesCt = int(denominator.getValue().length() - divMod.second.getValue().length()) - 1;
    zeroesCt = std::max(zeroesCt, 0);
    std::string zeroes = std::string(zeroesCt, '0');
    std::string second = zeroes + divMod.second.getValue();

    return divMod.first.getValue() + "." + second + " " + m_tokenIdentifier;
}


