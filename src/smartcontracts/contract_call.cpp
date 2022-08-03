#include <utility>

#include "hex.h"
#include "smartcontracts/contract_call.h"

ContractCall::ContractCall(std::string function)
        : m_function(std::move(function))
{}

ContractCall::ContractCall(std::string function, SCArguments args) :
        m_function(std::move(function)), m_args(std::move(args))
{}

void ContractCall::setArgs(SCArguments args)
{
    m_args = std::move(args);
}

std::string ContractCall::asOnData() const
{
    return (m_function.empty()) ?
           ("") : ("@" + util::stringToHex(m_function) + m_args.asOnData());
}
