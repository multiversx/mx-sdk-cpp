#ifndef MX_PAYLOAD_BUILDER_H
#define MX_PAYLOAD_BUILDER_H

// This file is an adaptation from https://github.com/multiversx/mx-sdk-js-core/blob/main/src/tokenTransferBuilders.ts

#include "esdt.h"
#include "account/address.h"
#include "transaction/token_payment.h"
#include "smartcontracts/contract_call.h"

class ESDTTransferPayloadBuilder
{
public:
    explicit ESDTTransferPayloadBuilder();

    ESDTTransferPayloadBuilder &setPayment(TokenPayment payment);

    ESDTTransferPayloadBuilder &withContractCall(ContractCall contractCall);

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

    ESDTNFTTransferPayloadBuilder &withContractCall(ContractCall contractCall);

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

    MultiESDTNFTTransferPayloadBuilder &withContractCall(ContractCall contractCall);

    std::string build() const;

private:
    std::vector<TokenPayment> m_payments;
    std::string m_destination;
    ContractCall m_contractCall;
};

class ESDTIssuePayloadBuilder
{
public:
    explicit ESDTIssuePayloadBuilder(std::string token);

    ESDTIssuePayloadBuilder &setTicker(std::string ticker);

    ESDTIssuePayloadBuilder &setInitialSupply(BigUInt initialSupply);

    ESDTIssuePayloadBuilder &setNumOfDecimals(uint32_t numOfDecimals);

    ESDTIssuePayloadBuilder &withProperties(ESDTProperties esdtProperties);

    std::string build() const;
private:
    std::string m_token;
    std::string m_ticker;
    BigUInt m_initialSupply;
    uint32_t m_numOfDecimals;
    ESDTProperties m_esdtProperties;
};

#endif //MX_PAYLOAD_BUILDER_H
