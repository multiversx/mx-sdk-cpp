#ifndef ERD_PAYLOAD_BUILDER_H
#define ERD_PAYLOAD_BUILDER_H

// This file is an adaptation from https://github.com/ElrondNetwork/elrond-sdk-erdjs/blob/main/src/tokenTransferBuilders.ts

#include "account/address.h"
#include "transaction/token_payment.h"
#include "smartcontracts/contract_call.h"

class ESDTTransferPayloadBuilder
{
public:
    explicit ESDTTransferPayloadBuilder();

    ESDTTransferPayloadBuilder &setPayment(TokenPayment payment);

    ESDTTransferPayloadBuilder &setContractCall(ContractCall contractCall);

    std::string build() const;

private:
    TokenPayment m_payment;
    ContractCall m_contractCall;
};

class ESDTNFTTransferPayloadBuilder
{
public:
    explicit ESDTNFTTransferPayloadBuilder();

    ESDTNFTTransferPayloadBuilder &setPayment(TokenPayment payment);

    ESDTNFTTransferPayloadBuilder &setDestination(Address const &address);

    ESDTNFTTransferPayloadBuilder &setContractCall(ContractCall contractCall);

    std::string build() const;

private:
    TokenPayment m_payment;
    std::string m_destination;
    ContractCall m_contractCall;
};

class MultiESDTNFTTransferPayloadBuilder
{
public:
    explicit MultiESDTNFTTransferPayloadBuilder();

    MultiESDTNFTTransferPayloadBuilder &setPayments(std::vector<TokenPayment> payments);

    MultiESDTNFTTransferPayloadBuilder &setDestination(Address const &address);

    MultiESDTNFTTransferPayloadBuilder &setContractCall(ContractCall contractCall);

    std::string build() const;

private:
    std::vector<TokenPayment> m_payments;
    std::string m_destination;
    ContractCall m_contractCall;
};

#endif //ERD_PAYLOAD_BUILDER_H
