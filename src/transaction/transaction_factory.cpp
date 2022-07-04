#include "transaction/transaction_factory.h"

 TransactionFactory::TransactionFactory(const NetworkConfig& networkConfig) :
        m_gasEstimator(networkConfig), m_chainID(networkConfig.chainId)
{}

TransactionESDTBuilder TransactionFactory::createESDTTransfer(
        TokenPayment tokenPayment,
        uint64_t nonce,
        Address receiver,
        Address sender,
        uint64_t gasPrice)
{
    TransactionBuilderInput baseInput(nonce, std::move(sender), std::move(receiver), gasPrice, m_chainID, m_gasEstimator);
    return TransactionESDTBuilder(baseInput, std::move(tokenPayment));
}