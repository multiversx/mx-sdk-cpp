#include "smartcontracts/sc_arguments.h"

#include "hex.h"

SCArguments::SCArguments() : m_args()
{};

void SCArguments::add(std::string const &arg)
{
    std::string const val = util::stringToHex(arg);

    m_args.emplace_back(val);
}

void SCArguments::add(BigUInt const &arg)
{
    std::string const val = arg.getHexValue();

    m_args.emplace_back(val);
}

void SCArguments::add(Address const &arg)
{
    bytes const pk = arg.getPublicKey();

    std::string pkHex(pk.begin(), pk.end());
    pkHex = util::stringToHex(pkHex);

    m_args.emplace_back(pkHex);
}

bool SCArguments::empty() const
{
    return m_args.empty();
}

std::string SCArguments::asOnData() const
{
    std::string ret;

    for (auto const &arg : m_args)
    {
        ret += "@" + arg;
    }

    return ret;
}


