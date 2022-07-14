#ifndef ERD_TRANSACTION_FACTORY_H
#define ERD_TRANSACTION_FACTORY_H

#include "gas_estimator.h"
#include "token_payment.h"
#include "itransaction_builder.h"

class TransactionFactory
{
public:
    explicit TransactionFactory(const NetworkConfig &networkConfig);

    std::unique_ptr<ITransactionBuilder> createEGLDTransfer(uint64_t nonce,
                                                            BigUInt value,
                                                            Address sender,
                                                            Address receiver,
                                                            uint64_t gasPrice,
                                                            std::string data);

    std::unique_ptr<ITokenTransactionBuilder> createESDTTransfer(TokenPayment tokenPayment,
                                                                 uint64_t nonce,
                                                                 Address sender,
                                                                 Address receiver,
                                                                 uint64_t gasPrice);

    std::unique_ptr<ITokenTransactionBuilder> createESDTNFTTransfer(TokenPayment tokenPayment,
                                                                    uint64_t nonce,
                                                                    Address sender,
                                                                    Address destination,
                                                                    uint64_t gasPrice);

private:
    std::string m_chainID;
    GasEstimator m_gasEstimator;
};

#endif //ERD_TRANSACTION_FACTORY_H
