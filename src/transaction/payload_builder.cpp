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

ESDTNFTTransferPayloadBuilder::ESDTNFTTransferPayloadBuilder() :
        m_payment(TokenPayment::fungibleFromAmount("", "0", 0))
{}

ESDTNFTTransferPayloadBuilder &ESDTNFTTransferPayloadBuilder::setPayment(TokenPayment payment)
{
    m_payment = std::move(payment);
    return *this;
}

ESDTNFTTransferPayloadBuilder &ESDTNFTTransferPayloadBuilder::setDestination(Address const &address)
{
    m_destination = address.getBech32Address();
    return *this;
}

std::string ESDTNFTTransferPayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_payment.tokenIdentifier());
    args.add(BigUInt(m_payment.nonce()));
    args.add(m_payment.value());
    args.add(Address(m_destination));

    return ESDT_NFT_TRANSFER_PREFIX + args.asOnData();
}
