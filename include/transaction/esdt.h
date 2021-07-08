#ifndef ERD_ESDT_H
#define ERD_ESDT_H

#include "transaction.h"
#include "scarguments.h"

#define NO_FUNCTION std::string()
#define NO_PARAMETERS SCArguments()

#define ESDT_GAS_LIMIT_NO_FUNCTION 500000
#define ESDT_TRANSFER_PREFIX std::string("ESDTTransfer")

void prepareTransactionForESDTTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function = NO_FUNCTION,
                                       SCArguments const &args = NO_PARAMETERS);

#endif //ERD_ESDT_H
