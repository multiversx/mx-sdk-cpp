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

    ITransactionBuilder &withVersion(uint64_t version);

    ITransactionBuilder &withOptions(uint32_t options);

    ITransactionBuilder &withContractCall(ContractCall contractCall);

    Transaction buildSigned(const bytes &seed);

    Transaction buildSigned(const ISecretKey &wallet);

protected:
    ContractCall m_contractCall;
    uint64_t m_version;
    std::shared_ptr<uint32_t> m_options;
};

#endif //ERD_ITRANSACTION_BUILDER_H
