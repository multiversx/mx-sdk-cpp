#ifndef ERD_PAYLOAD_BUILDER_H
#define ERD_PAYLOAD_BUILDER_H

// This file is an adaptation from https://github.com/ElrondNetwork/elrond-sdk-erdjs/blob/main/src/tokenTransferBuilders.ts

#include "transaction/token_payment.h"

class ESDTTransferPayloadBuilder
{
public:
    explicit ESDTTransferPayloadBuilder();

    ESDTTransferPayloadBuilder &setPayment(TokenPayment payment);

    std::string build( ) const;

private:
    TokenPayment m_payment;
};

#endif //ERD_PAYLOAD_BUILDER_H
