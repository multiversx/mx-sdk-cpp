#ifndef ERD_TRANSACTION_BUILDER_INPUT_H
#define ERD_TRANSACTION_BUILDER_INPUT_H

#include "internal/biguint.h"
#include "gas_estimator.h"
#include "account/address.h"

struct TransactionBuilderInput
{
    explicit TransactionBuilderInput(
            uint64_t nonce,
            BigUInt value,
            Address sender,
            Address receiver,
            std::string data,
            uint64_t gasPrice,
            std::string chainID,
            GasEstimator gasEstimator);

    TransactionBuilderInput(TransactionBuilderInput const &rhs) = default;

    uint64_t nonce;
    BigUInt value;
    Address sender;
    Address receiver;
    std::string data;
    uint64_t gasPrice;
    std::string chainID;
    GasEstimator gasEstimator;
};

#endif //ERD_TRANSACTION_BUILDER_INPUT_H
