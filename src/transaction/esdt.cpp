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
}

void prepareTransactionForESDTTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function,
                                       SCArguments const &args)
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
        transaction.m_gasLimit = ESDT_GAS_LIMIT_NO_FUNCTION;
    }

    if (!args.empty())
    {
        data += args.asOnData();
    }

    transaction.m_value = DEFAULT_VALUE;
    transaction.m_data = std::make_shared<bytes>(data.begin(), data.end());
}


