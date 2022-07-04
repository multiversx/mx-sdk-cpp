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

    uint64_t gasLimit = m_txInput.gasEstimator.forESDTTransfer(payload.size());

    return Transaction(
            m_txInput.nonce,
            "0",
            m_txInput.receiver,
            m_txInput.sender,
            nullptr,
            nullptr,
            m_txInput.gasPrice,
            gasLimit,
            std::make_shared<bytes>(bytes(payload.begin(), payload.end())),
            nullptr,
            m_txInput.chainID,
            DEFAULT_VERSION,
            DEFAULT_OPTIONS);
}
