#pragma warning(disable:4996)
#ifndef TRANSACTION_HANDLER_H
#define TRANSACTION_HANDLER_H

#include "filehandler/ifilehandler.h"
#include "wrappers/transaction_input_wrapper.h"
#include "transaction/transaction.h"

namespace ih
{
class TransactionInputHandler : public IFileHandler
{
public:
    explicit TransactionInputHandler(wrapper::TransactionInputWrapper const jsonInputWrapper);

    bool isFileValid() const override;

    void writeTransactionToJsonFile(Transaction const &transaction);

private:

    wrapper::TransactionInputWrapper m_inputData;
};
}

#endif