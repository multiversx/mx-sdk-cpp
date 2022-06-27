#include <utility>

#include "transaction/token_payment.h"

namespace internal
{
BigUInt bigUIntFromVal(double value, uint32_t numDecimals)
{

    auto v = value * std::pow(10, numDecimals);
    BigUInt val = BigUInt(std::to_string(uint64_t(v)));
   // std::string zeroes = std::string(numDecimals, '0');
  //  BigUInt multiplier = BigUInt("1" + zeroes);

    return val;// * multiplier;
}
}

TokenPayment::TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
        : m_tokenIdentifier(std::move(tokenIdentifier)),
          m_nonce(nonce),
          m_value(std::move(value)),
          m_numDecimals(numDecimals)
{}

TokenPayment TokenPayment::fungibleFromAmount(std::string tokenIdentifier, double value, uint32_t numDecimals)
{
    BigUInt amountBigInt = internal::bigUIntFromVal(value, numDecimals);
    return {std::move(tokenIdentifier), 0, amountBigInt, numDecimals};
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


