#include <utility>

#include "transaction/transaction_factory.h"
#include "transaction/transaction_builders.h"
#include "transaction/transaction_builder_input.h"

TransactionFactory::TransactionFactory(const NetworkConfig &networkConfig) :
        m_gasEstimator(networkConfig), m_chainID(networkConfig.chainId)
{}

ITransactionBuilder &TransactionFactory::createEGLDTransfer(uint64_t nonce,
                                                            BigUInt value,
                                                            Address sender,
                                                            Address receiver,
                                                            uint64_t gasPrice,
                                                            std::string data)
{
    static TransactionEGLDTransferBuilder builder({nonce,
                                                   std::move(value),
                                                   std::move(sender),
                                                   std::move(receiver),
                                                   std::move(data),
                                                   gasPrice,
                                                   m_chainID,
                                                   m_gasEstimator});
    return builder;
}

ITokenTransactionBuilder &TransactionFactory::createESDTTransfer(TokenPayment tokenPayment,
                                                                 uint64_t nonce,
                                                                 Address sender,
                                                                 Address receiver,
                                                                 uint64_t gasPrice)
{

    static TransactionESDTBuilder builder({nonce,
                                           BigUInt(0),
                                           std::move(sender),
                                           std::move(receiver),
                                           "",
                                           gasPrice,
                                           m_chainID,
                                           m_gasEstimator},
                                          std::move(tokenPayment));
    return builder;
}

ITokenTransactionBuilder &TransactionFactory::createESDTNFTTransfer(TokenPayment tokenPayment, uint64_t nonce, Address sender, Address destination, uint64_t gasPrice)
{
    static TransactionESDTNFTBuilder builder({nonce,
                                              BigUInt(0),
                                              std::move(sender),
                                              std::move(destination),
                                              "",
                                              gasPrice,
                                              m_chainID,
                                              m_gasEstimator},
                                             std::move(tokenPayment));

    return builder;
}
