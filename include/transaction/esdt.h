#ifndef ERD_ESDT_H
#define ERD_ESDT_H

#include "transaction.h"

#define NO_FUNCTION std::string()
#define NO_PARAMETERS std::vector<std::string>()

#define ESDT_GAS_LIMIT_NO_FUNCTION 500000
#define ESDT_TRANSFER_PREFIX std::string("ESDTTransfer")

void prepareTransactionForEsdtTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function = NO_FUNCTION,
                                       std::vector<std::string> const &params = NO_PARAMETERS);

#endif //ERD_ESDT_H
