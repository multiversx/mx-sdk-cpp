#ifndef ERD_ITRANSACTION_BUILDER_H
#define ERD_ITRANSACTION_BUILDER_H

#include "transaction.h"
#include "filehandler/isecretkey.h"
#include "smartcontracts/contract_call.h"

class ITransactionBuilder
{
public:
    explicit ITransactionBuilder();

    virtual Transaction build() = 0;

    ITransactionBuilder &withContractCall(ContractCall contractCall);

    Transaction buildSigned(const bytes &seed);

    Transaction buildSigned(const ISecretKey &wallet);

protected:
    ContractCall m_contractCall;
};

#endif //ERD_ITRANSACTION_BUILDER_H
