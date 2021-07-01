#include <gmpxx.h>

#include "transaction/esdt.h"
#include "hex.h"
#include "errors.h"

namespace internal
{
    inline std::string bigIntToHex(std::string const &val)
    {
        std::string ret;

        try
        {
            mpz_class number(val);
            number = abs(number);
            ret = number.get_str(16);
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_VALUE);
        }

        if (ret.size() % 2 != 0)
        {
            ret.insert(ret.begin(), '0');
        }

        return ret;
    }

    std::string ESDTPropertyField(std::string const &property, bool const &val)
    {
        std::string const value = val ? "true" : "false";

        return "@" + util::stringToHex(property) + "@" + util::stringToHex(value);
    }
}

bool operator==(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return (lhs.canFreeze          == rhs.canFreeze) &&
           (lhs.canWipe            == rhs.canWipe) &&
           (lhs.canPause           == rhs.canPause) &&
           (lhs.canMint            == rhs.canMint) &&
           (lhs.canBurn            == rhs.canBurn) &&
           (lhs.canChangeOwner     == rhs.canChangeOwner) &&
           (lhs.canUpgrade         == rhs.canUpgrade) &&
           (lhs.canAddSpecialRoles == rhs.canAddSpecialRoles);
}

bool operator!=(ESDTProperties const &lhs, ESDTProperties const &rhs)
{
    return !(lhs == rhs);
}

void prepareTransactionForESDTTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function,
                                       std::vector<std::string> const &params)
{
    std::string data = ESDT_TRANSFER_PREFIX +
                       "@" + util::stringToHex(token) +
                       "@" + internal::bigIntToHex(transaction.m_value);

    if (!function.empty())
    {
        data += "@" + util::stringToHex(function);
    }
    else
    {
        transaction.m_gasLimit = ESDT_TRANSFER_GAS_LIMIT_NO_FUNCTION;
    }

    if (!params.empty())
    {
        for (auto const &param : params)
        {
            data += "@" + util::stringToHex(param);
        }
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
                        "@" + util::stringToHex(initialSupply) +
                        "@" + util::stringToHex(noOfDecimals);

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
