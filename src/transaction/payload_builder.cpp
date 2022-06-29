#include "transaction/esdt.h"
#include "transaction/scarguments.h"
#include "transaction/payload_builder.h"

ESDTTransferPayloadBuilder::ESDTTransferPayloadBuilder() :
        m_payment(TokenPayment::fungibleFromAmount("", "0", 0))
{}

ESDTTransferPayloadBuilder &ESDTTransferPayloadBuilder::setPayment(TokenPayment payment)
{
    m_payment = std::move(payment);
    return *this;
}

std::string ESDTTransferPayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_payment.tokenIdentifier());
    args.add(m_payment.value());

    return ESDT_TRANSFER_PREFIX + args.asOnData();
}
