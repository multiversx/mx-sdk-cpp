#include <gmpxx.h>

#include "transaction/esdt.h"
#include "hex.h"

namespace internal
{
    inline std::string bigIntToHex(std::string const &val)
    {
        mpz_class a(val);
        return a.get_str(16);
    }
}

void prepareTransactionForEsdtTransfer(Transaction &transaction,
                                       std::string const &token,
                                       std::string const &function,
                                       std::vector<std::string> const &params)
{
    std::string data = ESDT_TRANSFER_PREFIX +
                       "@" + util::stringToHex(token) +
                       "@" + internal::bigIntToHex(transaction.m_value);

    if (!function.empty())
    {
        data += "@" + function;
    }
    else
    {
        transaction.m_gasLimit = ESDT_GAS_LIMIT_NO_FUNCTION;
    }

    if (!params.empty())
    {
        for (auto const &param : params)
        {
            data += "@" + param;
        }
    }

    transaction.m_value = DEFAULT_VALUE;
    transaction.m_data = std::make_shared<bytes>(data.begin(), data.end());
}


