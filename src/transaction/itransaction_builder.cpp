#include "transaction/itransaction_builder.h"

ITransactionBuilder &ITransactionBuilder::withContractCall(ContractCall contractCall)
{
    m_contractCall = std::move(contractCall);
    return *this;
}

Transaction ITransactionBuilder::buildSigned(const bytes &seed)
{
    Signer signer(seed);
    Transaction tx = this->build();
    tx.sign(signer);

    return tx;
}

Transaction ITransactionBuilder::buildSigned(const ISecretKey &wallet)
{
    bytes seed = wallet.getSeed();
    Signer signer(seed);
    Transaction tx = this->build();
    tx.sign(signer);

    return tx;
}

ITransactionBuilder::ITransactionBuilder() : m_contractCall("")
{}
