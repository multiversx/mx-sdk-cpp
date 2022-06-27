#include "transaction/token_payment.h"

TokenPayment TokenPayment::fungibleFromAmount(std::string tokenIdentifier, size_t value, uint32_t numDecimals)
{
    std::string val = std::to_string(value);
    std::string zeroes = std::string(numDecimals, '0');

    BigUInt amountBigInt = BigUInt(val + zeroes);
    return {std::move(tokenIdentifier), 0, amountBigInt, numDecimals};
}

TokenPayment::TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals)
        : m_tokenIdentifier(std::move(tokenIdentifier)),
          m_nonce(nonce),
          m_value(std::move(value)),
          m_numDecimals(numDecimals)
{}
