#ifndef ERD_TRANSACTION_BUILDER_INPUT_H
#define ERD_TRANSACTION_BUILDER_INPUT_H

#include "gas_estimator.h"
#include "account/address.h"
#include "internal/biguint.h"

struct TransactionBuilderInput
{
    uint64_t nonce;
    BigUInt value;
    Address sender;
    Address receiver;
    std::string data;
    uint64_t gasPrice;
    std::string chainID;
    GasEstimator gasEstimator;
    uint64_t gasLimit = DEFAULT_GAS_LIMIT;
};

#endif //ERD_TRANSACTION_BUILDER_INPUT_H
