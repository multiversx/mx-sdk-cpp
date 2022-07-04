#include <utility>

#include "transaction/transaction_builder_input.h"

TransactionBuilderInput::TransactionBuilderInput(
        uint64_t nonce,
        Address sender,
        Address receiver,
        uint64_t gasPrice,
        std::string chainID,
        GasEstimator gasEstimator) :
        nonce(nonce),
        sender(std::move(sender)),
        receiver(std::move(receiver)),
        gasPrice(gasPrice),
        chainID(std::move(chainID)),
        gasEstimator(std::move(gasEstimator))
{}
