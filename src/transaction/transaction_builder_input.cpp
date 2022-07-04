#include <utility>

#include "transaction/transaction_builder_input.h"

TransactionBuilderInput::TransactionBuilderInput(
        uint64_t nonce,
        BigUInt value,
        Address sender,
        Address receiver,
        std::string data,
        uint64_t gasPrice,
        std::string chainID,
        GasEstimator gasEstimator) :
        nonce(nonce),
        value(std::move(value)),
        sender(std::move(sender)),
        receiver(std::move(receiver)),
        data(std::move(data)),
        gasPrice(gasPrice),
        chainID(std::move(chainID)),
        gasEstimator(std::move(gasEstimator))
{}
