#ifndef ERD_PAYLOAD_BUILDER_H
#define ERD_PAYLOAD_BUILDER_H

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
