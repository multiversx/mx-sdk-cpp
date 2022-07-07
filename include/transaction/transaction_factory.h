#ifndef ERD_TRANSACTION_FACTORY_H
#define ERD_TRANSACTION_FACTORY_H

#include "gas_estimator.h"
#include "token_payment.h"
#include "itransaction_builder.h"


#include "esdt.h" //TODO: NOPE LIKE THIS


class TransactionFactory
{
public:
    explicit TransactionFactory(const NetworkConfig &networkConfig);

    ITransactionBuilder &createEGLDTransfer(uint64_t nonce,
                                            BigUInt value,
                                            Address sender,
                                            Address receiver,
                                            uint64_t gasPrice,
                                            std::string data);

    ITransactionBuilder &createESDTIssue(uint64_t nonce,
                                         Address sender,
                                         uint64_t const &gasPrice,
                                         std::string const &token,
                                         std::string const &ticker,
                                         BigUInt const &initialSupply,
                                         uint32_t const &numOfDecimals,
                                         ESDTProperties const &esdtProperties = ESDT_ISSUANCE_DEFAULT_PROPERTIES);

    ITokenTransactionBuilder &createESDTTransfer(TokenPayment tokenPayment,
                                                 uint64_t nonce,
                                                 Address sender,
                                                 Address receiver,
                                                 uint64_t gasPrice);

    ITokenTransactionBuilder &createESDTNFTTransfer(TokenPayment tokenPayment,
                                                    uint64_t nonce,
                                                    Address sender,
                                                    Address destination,
                                                    uint64_t gasPrice);

    ITokenTransactionBuilder &createMultiESDTNFTTransfer(std::vector<TokenPayment> tokenPayments,
                                                         uint64_t nonce,
                                                         Address sender,
                                                         Address destination,
                                                         uint64_t gasPrice);

private:
    std::string m_chainID;
    GasEstimator m_gasEstimator;
};

#endif //ERD_TRANSACTION_FACTORY_H
