#ifndef ERD_TOKEN_PAYMENT_H
#define ERD_TOKEN_PAYMENT_H

#include <string>
#include <utility>
#include "scarguments.h"
#include "bigint/integer.h"
#include <cmath>

class TokenPayment
{
public:
    static TokenPayment fungibleFromAmount(std::string tokenIdentifier, size_t value, uint32_t numDecimals);

    static TokenPayment fungibleFromBigUInt(std::string tokenIdentifier, BigUInt value, uint32_t numDecimals = 0);

    static TokenPayment nonFungible(std::string tokenIdentifier, uint64_t nonce);

    static TokenPayment metaESDTFromAmount(std::string tokenIdentifier, uint64_t nonce, size_t value, uint32_t numDecimals);

    static TokenPayment metaESDTFromBigUInt(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals);

    std::string toString() const;

    std::string toPrettyString() const;

private:
    std::string m_tokenIdentifier;
    BigUInt m_value;
    uint64_t m_nonce;
    uint32_t m_numDecimals;


    TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals);
};

#endif //ERD_TOKEN_PAYMENT_H
