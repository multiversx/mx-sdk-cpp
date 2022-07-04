#include <utility>

#include "transaction/transaction_builders.h"
#include "transaction/payload_builder.h"

TransactionEGLDTransferBuilder::TransactionEGLDTransferBuilder(TransactionBuilderInput txInput) :
        m_txInput(std::move(txInput))
{}

Transaction TransactionEGLDTransferBuilder::build()
{
    uint64_t gasLimit = m_txInput.gasEstimator.forEGLDTransfer(m_txInput.data.size());
    bytes const payloadBytes = bytes(m_txInput.data.begin(), m_txInput.data.end());
    return Transaction(
            m_txInput.nonce,
            m_txInput.value.getValue(),
            m_txInput.receiver,
            m_txInput.sender,
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            std::make_shared<bytes>(payloadBytes),
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
    bytes const payloadBytes = bytes(payload.begin(), payload.end());
    uint64_t gasLimit = m_txInput.gasEstimator.forESDTTransfer(payloadBytes.size());

    return Transaction(
            m_txInput.nonce,
            DEFAULT_VALUE,
            m_txInput.receiver,
            m_txInput.sender,
            DEFAULT_SENDER_NAME,
            DEFAULT_RECEIVER_NAME,
            m_txInput.gasPrice,
            gasLimit,
            std::make_shared<bytes>(payloadBytes),
            DEFAULT_SIGNATURE,
            m_txInput.chainID,
            m_version,
            m_options);
}
