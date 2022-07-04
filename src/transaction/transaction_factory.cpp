#include <utility>

#include "transaction/transaction_factory.h"

TransactionFactory::TransactionFactory(const NetworkConfig &networkConfig) :
        m_gasEstimator(networkConfig), m_chainID(networkConfig.chainId)
{}

ITransactionBuilder &
TransactionFactory::createEGLDTransfer(uint64_t nonce, BigUInt value, Address sender, Address receiver, uint64_t gasPrice, std::string data)
{
    TransactionBuilderInput baseInput(nonce,
                                      std::move(value),
                                      std::move(sender),
                                      std::move(receiver),
                                      std::move(data),
                                      gasPrice,
                                      m_chainID,
                                      m_gasEstimator);

    static TransactionEGLDTransferBuilder ret(baseInput);
    return ret;
}

ITokenTransactionBuilder &TransactionFactory::createESDTTransfer(
        TokenPayment tokenPayment,
        uint64_t nonce,
        Address receiver,
        Address sender,
        uint64_t gasPrice)
{
    TransactionBuilderInput baseInput(nonce,
                                      BigUInt(0),
                                      std::move(sender),
                                      std::move(receiver),
                                      "",
                                      gasPrice,
                                      m_chainID,
                                      m_gasEstimator);

    static TransactionESDTBuilder ret(baseInput, std::move(tokenPayment));
    return ret;
}
