#ifndef ERD_TRANSACTION_BUILDER_ESDT_H
#define ERD_TRANSACTION_BUILDER_ESDT_H

#include "token_payment.h"
#include "itransaction_builder.h"
#include "transaction/transaction.h"
#include "transaction_builder_input.h"

class TransactionFactory;

class TransactionEGLDTransferBuilder : public ITransactionBuilder
{
    friend class TransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionEGLDTransferBuilder(TransactionBuilderInput txInput);

    TransactionBuilderInput m_txInput;
};


class TransactionESDTBuilder : public ITokenTransactionBuilder
{
    friend class TransactionFactory;

public:
    Transaction build() override;

private:
    explicit TransactionESDTBuilder(TransactionBuilderInput txInput, TokenPayment payment);

    TransactionBuilderInput m_txInput;
    TokenPayment m_tokenPayment;
};


#endif //ERD_TRANSACTION_BUILDER_ESDT_H
