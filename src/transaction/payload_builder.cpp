#include <utility>

#include "hex.h"
#include "transaction/esdt.h"
#include "smartcontracts/sc_arguments.h"
#include "transaction/payload_builder.h"

namespace
{

std::string ESDTPropertyField(std::string const &property, bool const &val)
{
    std::string const value = val ? "true" : "false";
    return "@" + util::stringToHex(property) + "@" + util::stringToHex(value);
}

std::string ESDTPropertiesAsOnData(ESDTProperties const &esdtProperties)
{
    return ESDTPropertyField("canFreeze", esdtProperties.canFreeze) +
           ESDTPropertyField("canWipe", esdtProperties.canWipe) +
           ESDTPropertyField("canPause", esdtProperties.canPause) +
           ESDTPropertyField("canMint", esdtProperties.canMint) +
           ESDTPropertyField("canBurn", esdtProperties.canBurn) +
           ESDTPropertyField("canChangeOwner", esdtProperties.canChangeOwner) +
           ESDTPropertyField("canUpgrade", esdtProperties.canUpgrade) +
           ESDTPropertyField("canAddSpecialRoles", esdtProperties.canAddSpecialRoles);
}

}

ESDTTransferPayloadBuilder::ESDTTransferPayloadBuilder() :
        m_payment(TokenPayment::fungibleFromAmount("", "0", 0)),
        m_contractCall("")
{}

ESDTTransferPayloadBuilder &ESDTTransferPayloadBuilder::setPayment(TokenPayment payment)
{
    m_payment = std::move(payment);
    return *this;
}

ESDTTransferPayloadBuilder &ESDTTransferPayloadBuilder::withContractCall(ContractCall contractCall)
{
    m_contractCall = std::move(contractCall);
    return *this;
}

std::string ESDTTransferPayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_payment.tokenIdentifier());
    args.add(m_payment.value());

    return ESDT_TRANSFER_PREFIX + args.asOnData() + m_contractCall.asOnData();
}

ESDTNFTTransferPayloadBuilder::ESDTNFTTransferPayloadBuilder() :
        m_payment(TokenPayment::fungibleFromAmount("", "0", 0)),
        m_contractCall("")
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

ESDTNFTTransferPayloadBuilder &ESDTNFTTransferPayloadBuilder::withContractCall(ContractCall contractCall)
{
    m_contractCall = std::move(contractCall);
    return *this;
}

std::string ESDTNFTTransferPayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_payment.tokenIdentifier());
    args.add(BigUInt(m_payment.nonce()));
    args.add(m_payment.value());
    args.add(Address(m_destination));

    return ESDT_NFT_TRANSFER_PREFIX + args.asOnData() + m_contractCall.asOnData();
}


MultiESDTNFTTransferPayloadBuilder::MultiESDTNFTTransferPayloadBuilder() :
        m_contractCall("")
{}

MultiESDTNFTTransferPayloadBuilder &MultiESDTNFTTransferPayloadBuilder::setPayments(std::vector<TokenPayment> payments)
{
    m_payments = std::move(payments);
    return *this;
}

MultiESDTNFTTransferPayloadBuilder &MultiESDTNFTTransferPayloadBuilder::setDestination(const Address &address)
{
    m_destination = address.getBech32Address();
    return *this;
}


MultiESDTNFTTransferPayloadBuilder &MultiESDTNFTTransferPayloadBuilder::withContractCall(ContractCall contractCall)
{
    m_contractCall = std::move(contractCall);
    return *this;
}

std::string MultiESDTNFTTransferPayloadBuilder::build() const
{
    SCArguments args;
    args.add(Address(m_destination));
    args.add(BigUInt(m_payments.size()));

    for (TokenPayment const &payment: m_payments)
    {
        args.add(payment.tokenIdentifier());
        args.add(BigUInt(payment.nonce()));
        args.add(payment.value());
    }

    return MULTI_ESDT_NFT_TRANSFER_PREFIX + args.asOnData() + m_contractCall.asOnData();
}

ESDTIssuePayloadBuilder::ESDTIssuePayloadBuilder(std::string token) :
        m_token(std::move(token)),
        m_initialSupply(0),
        m_numOfDecimals(0)
{}

ESDTIssuePayloadBuilder &ESDTIssuePayloadBuilder::setTicker(std::string ticker)
{
    m_ticker = std::move(ticker);
    return *this;
}

ESDTIssuePayloadBuilder &ESDTIssuePayloadBuilder::setInitialSupply(BigUInt initialSupply)
{
    m_initialSupply = std::move(initialSupply);
    return *this;
}

ESDTIssuePayloadBuilder &ESDTIssuePayloadBuilder::setNumOfDecimals(uint32_t numOfDecimals)
{
    m_numOfDecimals = numOfDecimals;
    return *this;
}

ESDTIssuePayloadBuilder &ESDTIssuePayloadBuilder::withProperties(ESDTProperties esdtProperties)
{
    m_esdtProperties = esdtProperties;
    return *this;
}

std::string ESDTIssuePayloadBuilder::build() const
{
    SCArguments args;
    args.add(m_token);
    args.add(m_ticker);
    args.add(m_initialSupply);
    args.add(BigUInt(m_numOfDecimals));

    std::string data = ESDT_ISSUANCE_PREFIX + args.asOnData();
    data += (m_esdtProperties != ESDT_ISSUANCE_DEFAULT_PROPERTIES) ? ESDTPropertiesAsOnData(m_esdtProperties) : "";

    return data;
}
