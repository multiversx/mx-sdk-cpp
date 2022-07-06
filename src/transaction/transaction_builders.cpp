#include <utility>

#include "transaction/transaction_builders.h"
#include "transaction/payload_builder.h"

namespace
{
std::shared_ptr<bytes> stringToBytesPtr(std::string const &in)
{
    bytes strToBytes(in.begin(), in.end());
    return std::make_shared<bytes>(strToBytes);
}
}

TransactionEGLDTransferBuilder::TransactionEGLDTransferBuilder(TransactionBuilderInput txInput) :
        ITransactionBuilder(),
        m_txInput(std::move(txInput))
{}

Transaction TransactionEGLDTransferBuilder::build()
{
    uint64_t gasLimit = m_txInput.gasEstimator.forEGLDTransfer(m_txInput.data.size());

    return Transaction(
            m_txInput.nonce,
            m_txInput.value.getValue(),
            m_txInput.receiver,
            m_txInput.sender,
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            stringToBytesPtr(m_txInput.data),
            DEFAULT_SIGNATURE,
            m_txInput.chainID,
            m_version,
            m_options);
}

TransactionESDTBuilder::TransactionESDTBuilder(TransactionBuilderInput txInput, TokenPayment payment) :
        ITokenTransactionBuilder(),
        m_txInput(std::move(txInput)),
        m_tokenPayment(std::move(payment))
{}

Transaction TransactionESDTBuilder::build()
{
    std::string payload = ESDTTransferPayloadBuilder()
            .setPayment(m_tokenPayment)
            .withContractCall(m_contractCall)
            .build();
    uint64_t gasLimit = m_txInput.gasEstimator.forESDTTransfer(payload.size());

    return Transaction(
            m_txInput.nonce,
            DEFAULT_VALUE,
            m_txInput.receiver,
            m_txInput.sender,
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            stringToBytesPtr(payload),
            DEFAULT_SIGNATURE,
            m_txInput.chainID,
            m_version,
            m_options);
}

TransactionESDTNFTBuilder::TransactionESDTNFTBuilder(TransactionBuilderInput txInput, TokenPayment payment) :
        ITokenTransactionBuilder(),
        m_txInput(std::move(txInput)),
        m_tokenPayment(std::move(payment))
{}

Transaction TransactionESDTNFTBuilder::build()
{
    std::string payload = ESDTNFTTransferPayloadBuilder()
            .setPayment(m_tokenPayment)
            .setDestination(m_txInput.receiver)
            .withContractCall(m_contractCall)
            .build();
    uint64_t gasLimit = m_txInput.gasEstimator.forESDTNFTTransfer(payload.size());

    return Transaction(
            m_txInput.nonce,
            DEFAULT_VALUE,
            m_txInput.sender,
            m_txInput.sender, // sender = receiver
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            stringToBytesPtr(payload),
            DEFAULT_SIGNATURE,
            m_txInput.chainID,
            m_version,
            m_options);
}
