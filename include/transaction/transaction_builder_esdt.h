#ifndef ERD_TRANSACTION_BUILDER_ESDT_H
#define ERD_TRANSACTION_BUILDER_ESDT_H

#include "transaction_builder_input.h"
#include "token_payment.h"
#include "itransaction_builder.h"
#include "transaction/payload_builder.h"
#include "transaction/transaction.h"

class TransactionFactory;

class TransactionESDTBuilder : public ITransactionBuilder
{
    friend class TransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionESDTBuilder(const TransactionBuilderInput &txInput, TokenPayment payment);

    TransactionBuilderInput m_txInput;
    TokenPayment m_tokenPayment;
};


#endif //ERD_TRANSACTION_BUILDER_ESDT_H
