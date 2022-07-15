#include "hex.h"
#include "transaction/esdt.h"

namespace internal
{

std::string ESDTPropertyField(std::string const &property, bool const &val)
{
    std::string const value = val ? "true" : "false";

    return "@" + util::stringToHex(property) + "@" + util::stringToHex(value);
}

}

bool operator==(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return (lhs.canFreeze                == rhs.canFreeze) &&
           (lhs.canWipe                  == rhs.canWipe) &&
           (lhs.canPause                 == rhs.canPause) &&
           (lhs.canMint                  == rhs.canMint) &&
           (lhs.canBurn                  == rhs.canBurn) &&
           (lhs.canChangeOwner           == rhs.canChangeOwner) &&
           (lhs.canUpgrade               == rhs.canUpgrade) &&
           (lhs.canAddSpecialRoles       == rhs.canAddSpecialRoles) &&
           (lhs.canTransferNFTCreateRole == rhs.canTransferNFTCreateRole);
}

bool operator!=(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return !(lhs == rhs);
}

void prepareTransactionForESDTTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function,
                                       SCArguments const &args)
{
    std::string data = ESDT_TRANSFER_PREFIX +
                       "@" + util::stringToHex(token) +
                       "@" + BigUInt(transaction.m_value).getHexValue();

    if (!function.empty())
    {
        data += "@" + util::stringToHex(function);
    }
    else
    {
        transaction.m_gasLimit = ESDT_TRANSFER_GAS_LIMIT_NO_FUNCTION;
    }

    if (!args.empty())
    {
        data += args.asOnData();
    }

    transaction.m_value = DEFAULT_VALUE;
    transaction.m_data = std::make_shared<bytes>(data.begin(), data.end());
}

void prepareTransactionForESDTIssuance(Transaction &transaction,
                                      std::string const &token,
                                      std::string const &ticker,
                                      std::string const &initialSupply,
                                      std::string const &noOfDecimals,
                                      ESDTProperties const &esdtProperties)
{
    transaction.m_value = ESDT_ISSUANCE_VALUE;
    transaction.m_gasLimit = ESDT_ISSUANCE_GAS_LIMIT;

    std::string data = ESDT_ISSUANCE_PREFIX +
                       "@" + util::stringToHex(token) +
                       "@" + util::stringToHex(ticker) +
                       "@" + BigUInt(initialSupply).getHexValue() +
                       "@" + BigUInt(noOfDecimals).getHexValue();

    if (esdtProperties != ESDT_ISSUANCE_DEFAULT_PROPERTIES)
    {
        data += internal::ESDTPropertyField("canFreeze", esdtProperties.canFreeze) +
                internal::ESDTPropertyField("canWipe", esdtProperties.canWipe) +
                internal::ESDTPropertyField("canPause", esdtProperties.canPause) +
                internal::ESDTPropertyField("canMint", esdtProperties.canMint) +
                internal::ESDTPropertyField("canBurn", esdtProperties.canBurn) +
                internal::ESDTPropertyField("canChangeOwner", esdtProperties.canChangeOwner) +
                internal::ESDTPropertyField("canUpgrade", esdtProperties.canUpgrade) +
                internal::ESDTPropertyField("canAddSpecialRoles", esdtProperties.canAddSpecialRoles);
    }

    transaction.m_data = std::make_shared<bytes>(data.begin(), data.end());
}

