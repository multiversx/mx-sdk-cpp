#include "hex.h"
#include "transaction/esdt.h"

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
        transaction.m_gasLimit = ESDT_GAS_LIMIT_NO_FUNCTION;
    }

    if (!args.empty())
    {
        data += args.asOnData();
    }

    transaction.m_value = DEFAULT_VALUE;
    transaction.m_data = std::make_shared<bytes>(data.begin(), data.end());
}


