#ifndef JSONHANDLER_WRAPPER_H
#define JSONHANDLER_WRAPPER_H

#include "iwrapper.h"
#include "utils/params.h"
#include "account/address.h"
#include "internal/internal.h"
#include "transaction/transaction.h"

namespace internal
{
template<class T>
inline std::shared_ptr<T> getUserInputIfExists(cxxopts::ParseResult const & userInputs,
                                               std::string const &arg)
{

    if (userInputs.count(arg))
    {
        auto const input = userInputs[arg].as<uint64_t>();
        return std::make_shared<T>(input);
    }
    else return nullptr;
}

template<>
inline std::shared_ptr<bytes> getUserInputIfExists(cxxopts::ParseResult const & userInputs,
                                                   std::string const &arg)
{
    if (userInputs.count(arg))
    {
        auto const userInput = userInputs[arg].as<std::string>();
        bytes const input(userInput.begin(),userInput.end());
        return std::make_shared<bytes>(input);
    }
    else return nullptr;
}
}

namespace ih
{
namespace wrapper
{
class TransactionInputWrapper : public IWrapper
{
public:
    explicit TransactionInputWrapper(cxxopts::ParseResult const  &inputData) :
            IWrapper(inputData)
    {}

    uint64_t getNonce() const
    {
        return getInputData()["nonce"].as<uint64_t>();
    }

    std::string getValue() const
    {
        return getInputData()["value"].as<std::string>();
    }

    Address getReceiver() const
    {
        std::string const address = getInputData()["receiver"].as<std::string>();
        return Address(address);
    }

    std::shared_ptr<bytes> getReceiverName() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), "receiver-name");
    }

    std::shared_ptr<bytes> getSenderName() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), "sender-name");
    }

    uint64_t getGasPrice() const
    {
        return getInputData()["gas-price"].as<uint64_t>();
    }

    uint64_t getGasLimit() const
    {
        return getInputData()["gas-limit"].as<uint64_t>();
    }

    std::shared_ptr<bytes> getData() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), "data");
    }

    std::shared_ptr<std::string> getSignature() const
    {
        return DEFAULT_SIGNATURE;
    }

    std::string getChainId() const
    {
        return getInputData()["chainID"].as<std::string>();
    }

    uint64_t getVersion() const
    {
        return DEFAULT_VERSION;
    }

    std::string getInputFile() const
    {
        return getInputData()["key"].as<std::string>();
    }

    std::string getOutputFile() const
    {
        return getInputData()["outfile"].as<std::string>();
    }

    std::shared_ptr<uint32_t> getOptions() const
    {
        return internal::getUserInputIfExists<uint32_t>(getInputData(), "options");
    }
};
}
}

#endif
