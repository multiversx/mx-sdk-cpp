#include <utility>

#include "transaction/transaction_builder_esdt.h"


TransactionESDTBuilder::TransactionESDTBuilder(const TransactionBuilderInput &txInput, TokenPayment payment) :
        ITransactionBuilder(),
        m_txInput(txInput),
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
