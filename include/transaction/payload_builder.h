#ifndef ERD_PAYLOAD_BUILDER_H
#define ERD_PAYLOAD_BUILDER_H

// This file is an adaptation from https://github.com/ElrondNetwork/elrond-sdk-erdjs/blob/main/src/tokenTransferBuilders.ts

#include "account/address.h"
#include "transaction/token_payment.h"

class ESDTTransferPayloadBuilder
{
public:
    explicit ESDTTransferPayloadBuilder();

    ESDTTransferPayloadBuilder &setPayment(TokenPayment payment);

    std::string build() const;

private:
    TokenPayment m_payment;
};

class ESDTNFTTransferPayloadBuilder
{
public:
    explicit ESDTNFTTransferPayloadBuilder();

    ESDTNFTTransferPayloadBuilder &setPayment(TokenPayment payment);

    ESDTNFTTransferPayloadBuilder &setDestination(Address const &address);

    std::string build() const;

private:
    TokenPayment m_payment;
    std::string m_destination;
};

#endif //ERD_PAYLOAD_BUILDER_H
