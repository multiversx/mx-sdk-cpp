#ifndef MX_TRANSACTION_FACTORY_H
#define MX_TRANSACTION_FACTORY_H

#include "esdt.h"
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
                                                            std::string data = std::string());

    std::unique_ptr<ITransactionBuilder> createESDTIssue(uint64_t nonce,
                                                         Address sender,
                                                         uint64_t const &gasPrice,
                                                         std::string const &token,
                                                         std::string const &ticker,
                                                         BigUInt const &initialSupply,
                                                         uint32_t const &numOfDecimals,
                                                         ESDTProperties const &esdtProperties = ESDT_ISSUANCE_DEFAULT_PROPERTIES);

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

    std::unique_ptr<ITokenTransactionBuilder> createMultiESDTNFTTransfer(std::vector<TokenPayment> tokenPayments,
                                                                         uint64_t nonce,
                                                                         Address sender,
                                                                         Address destination,
                                                                         uint64_t gasPrice);

private:
    std::string m_chainID;
    GasEstimator m_gasEstimator;
};

#endif //MX_TRANSACTION_FACTORY_H
