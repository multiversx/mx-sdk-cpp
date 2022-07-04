#ifndef ERD_TRANSACTION_FACTORY_H
#define ERD_TRANSACTION_FACTORY_H

#include "gas_estimator.h"
#include "transaction.h"
#include "payload_builder.h"
#include "filehandler/isecretkey.h"

class TransactionESDTBuilder;

class TransactionFactory
{


public:

    TransactionFactory(NetworkConfig networkConfig) : m_gasEstimator(networkConfig)
    {
    }

    TransactionESDTBuilder createESDTTransfer(TokenPayment tokenPayment)
    {
        return TransactionESDTBuilder(tokenPayment,m_gasEstimator);
    }

private:
    GasEstimator m_gasEstimator;
};

class TransactionESDTBuilder
{
    friend class TransactionFactory;

public:
    TransactionESDTBuilder(TokenPayment tokenPayment, GasEstimator gasEstimator) :
            m_tokenPayment(tokenPayment), m_gasEstimator(gasEstimator), m_contractCall("")
    {

    }

    TransactionESDTBuilder &withContractCall(ContractCall contractCall)
    {

    }

    TransactionESDTBuilder &sign(bytes seed)
    {

    }

    TransactionESDTBuilder &sign(ISecretKey wallet)
    {

    }

    Transaction build()
    {
        std::string payload = ESDTTransferPayloadBuilder().setPayment(m_tokenPayment).build();
        uint64_t gasLimit = m_gasEstimator.forESDTTransfer(payload.size());
    }

    TokenPayment m_tokenPayment;
    GasEstimator m_gasEstimator;
    ContractCall m_contractCall;
};

#endif //ERD_TRANSACTION_FACTORY_H
