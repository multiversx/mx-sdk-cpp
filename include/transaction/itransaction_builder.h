#ifndef ERD_ITRANSACTION_BUILDER_H
#define ERD_ITRANSACTION_BUILDER_H

#include "transaction.h"
#include "filehandler/isecretkey.h"

class ITransactionBuilder
{
public:
    virtual Transaction build() = 0;

    Transaction buildSigned(const bytes &seed);

    Transaction buildSigned(const ISecretKey &wallet);
};

#endif //ERD_ITRANSACTION_BUILDER_H
