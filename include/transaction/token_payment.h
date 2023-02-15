#ifndef MX_TOKEN_PAYMENT_H
#define MX_TOKEN_PAYMENT_H

#include <string>
#include "internal/biguint.h"

class TokenPayment
{
public:
    static TokenPayment fungibleFromAmount(std::string tokenIdentifier, std::string amount, uint32_t numDecimals);

    static TokenPayment fungibleFromBigUInt(std::string tokenIdentifier, BigUInt value, uint32_t numDecimals = 0);

    static TokenPayment semiFungible(std::string tokenIdentifier, uint64_t nonce, BigUInt quantity);

    static TokenPayment nonFungible(std::string tokenIdentifier, uint64_t nonce);

    static TokenPayment metaESDTFromAmount(std::string tokenIdentifier, uint64_t nonce, std::string amount, uint32_t numDecimals);

    static TokenPayment metaESDTFromBigUInt(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals = 0);

    BigUInt value() const;

    uint64_t nonce() const;

    std::string tokenIdentifier() const;

    std::string toString() const;

    std::string toPrettyString() const;

private:
    TokenPayment(std::string tokenIdentifier, uint64_t nonce, BigUInt value, uint32_t numDecimals);

    std::string m_tokenIdentifier;
    BigUInt m_value;
    uint64_t m_nonce;
    uint32_t m_numDecimals;
};

#endif //MX_TOKEN_PAYMENT_H
