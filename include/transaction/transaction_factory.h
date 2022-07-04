#ifndef ERD_TRANSACTION_FACTORY_H
#define ERD_TRANSACTION_FACTORY_H

#include "gas_estimator.h"
#include "transaction.h"
#include "payload_builder.h"
#include "filehandler/isecretkey.h"
#include "itransaction_builder.h"
#include "transaction_builder_input.h"

#include "transaction_builder_esdt.h"

class TransactionFactory
{
public:
    explicit TransactionFactory(const NetworkConfig &networkConfig);

    ITransactionBuilder &createESDTTransfer(TokenPayment tokenPayment,
                                            uint64_t nonce,
                                            Address receiver,
                                            Address sender,
                                            uint64_t gasPrice);

private:
    std::string m_chainID;
    GasEstimator m_gasEstimator;
};

#endif //ERD_TRANSACTION_FACTORY_H
