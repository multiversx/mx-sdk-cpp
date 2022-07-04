#ifndef ERD_TRANSACTION_FACTORY_H
#define ERD_TRANSACTION_FACTORY_H

#include "gas_estimator.h"
#include "transaction.h"
#include "payload_builder.h"
#include "filehandler/isecretkey.h"

struct TransactionBuilderInput
{
    TransactionBuilderInput(
            uint64_t nonce,
            Address sender,
            Address receiver,
            uint64_t gasPrice,
            std::string chainID,
            GasEstimator gasEstimator) :
            nonce(nonce),
            sender(sender),
            receiver(receiver),
            gasPrice(gasPrice),
            chainID(chainID),
            gasEstimator(gasEstimator)
    {}

    TransactionBuilderInput(TransactionBuilderInput const &rhs) :
            nonce(rhs.nonce),
            sender(rhs.sender),
            receiver(rhs.receiver),
            gasPrice(rhs.gasPrice),
            chainID(rhs.chainID),
            gasEstimator(rhs.gasEstimator)
    {}

    uint64_t nonce;
    Address sender;
    Address receiver;
    uint64_t gasPrice;
    std::string chainID;
    GasEstimator gasEstimator;
};

struct ESDTTransactionBuilderInput : TransactionBuilderInput
{
    ESDTTransactionBuilderInput(TransactionBuilderInput baseInput, TokenPayment tokenPayment) :
            TransactionBuilderInput(baseInput),
            tokenPayment(tokenPayment)
    {}

    TokenPayment tokenPayment;
};

class ITransactionBuilder
{
public:
    virtual Transaction build() = 0;

    Transaction buildSigned(const bytes &seed)
    {
        Signer signer(seed);
        Transaction tx = this->build();
        tx.sign(signer);

        return tx;
    }

    Transaction buildSigned(const ISecretKey &wallet)
    {
        bytes seed = wallet.getSeed();
        Signer signer(seed);
        Transaction tx = this->build();
        tx.sign(signer);

        return tx;
    }
};

//class TransactionESDTBuilder;
class TransactionFactory;

class TransactionESDTBuilder : public ITransactionBuilder
{
    friend class TransactionFactory;

public:

    TransactionESDTBuilder &withContractCall(ContractCall contractCall)
    {
        m_contractCall = contractCall;
        return *this;
    }


    Transaction build() override
    {
        std::string payload = ESDTTransferPayloadBuilder()
                .setPayment(m_tokenPayment)
                .withContractCall(m_contractCall)
                .build();

        uint64_t gasLimit = m_txInput.gasEstimator.forESDTTransfer(payload.size());

        return Transaction(
                m_txInput.nonce,
                "0",
                m_txInput.receiver,
                m_txInput.sender,
                nullptr,
                nullptr,
                m_txInput.gasPrice,
                gasLimit,
                std::make_shared<bytes>(bytes(payload.begin(), payload.end())),
                nullptr,
                m_txInput.chainID,
                DEFAULT_VERSION,
                DEFAULT_OPTIONS);
    }


private:
    explicit TransactionESDTBuilder(ESDTTransactionBuilderInput input) :
            m_txInput(input), m_tokenPayment(input.tokenPayment), m_contractCall("")
    {}

    ESDTTransactionBuilderInput m_txInput;
    TokenPayment m_tokenPayment;
    ContractCall m_contractCall;
};


class TransactionFactory
{


public:
    TransactionFactory(NetworkConfig networkConfig) :
            m_gasEstimator(networkConfig), m_chainID(networkConfig.chainId)
    {}

    TransactionESDTBuilder createESDTTransfer(
            TokenPayment tokenPayment,
            uint64_t nonce,
            Address receiver,
            Address sender,
            uint64_t gasPrice)
    {
        TransactionBuilderInput baseInput(nonce, sender, receiver, gasPrice, m_chainID, m_gasEstimator);
        return TransactionESDTBuilder({baseInput, tokenPayment});
    }

private:
    std::string m_chainID;
    GasEstimator m_gasEstimator;
};


#endif //ERD_TRANSACTION_FACTORY_H
