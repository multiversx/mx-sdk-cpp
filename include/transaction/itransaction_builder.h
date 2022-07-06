#ifndef ERD_ITRANSACTION_BUILDER_H
#define ERD_ITRANSACTION_BUILDER_H

#include "transaction.h"
#include "filehandler/isecretkey.h"
#include "smartcontracts/contract_call.h"

class ITransactionBuilder
{
public:
    explicit ITransactionBuilder();

    ITransactionBuilder &withVersion(uint64_t version);

    ITransactionBuilder &withOptions(uint32_t options);

    virtual Transaction build() = 0;

    Transaction buildSigned(const bytes &seed);

protected:
    uint64_t m_version;
    std::shared_ptr<uint32_t> m_options;
};

class ITokenTransactionBuilder : public ITransactionBuilder
{
public:
    explicit ITokenTransactionBuilder();

    ITokenTransactionBuilder &withContractCall(ContractCall contractCall);

protected:
    ContractCall m_contractCall;
};

#endif //ERD_ITRANSACTION_BUILDER_H
