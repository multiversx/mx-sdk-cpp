#ifndef TRANSACTION_INPUT_WRAPPER_H
#define TRANSACTION_INPUT_WRAPPER_H

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

template <typename T>
inline T getUserInputOrDefault(cxxopts::ParseResult const &userInputs, std::string const &value, T const &defaultValue)
{
    return (userInputs.count(value) != 0) ?
           (userInputs[value].as<T>()) : defaultValue;
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
        return internal::getUserInputOrDefault<uint64_t>(getInputData(), "nonce", DEFAULT_NONCE);
    }

    std::string getValue() const
    {
        return internal::getUserInputOrDefault<std::string>(getInputData(), "value", DEFAULT_VALUE);
    }

    std::shared_ptr<Address> getReceiver() const
    {
        if (getInputData().count("receiver") != 0)
        {
            std::string const bech32Address= getInputData()["receiver"].as<std::string>();
            Address const receiver(bech32Address);
            return std::make_shared<Address>(receiver);
        }

        return DEFAULT_RECEIVER;
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
        return internal::getUserInputOrDefault<uint64_t>(getInputData(), "gas-price", DEFAULT_GAS_PRICE);
    }

    uint64_t getGasLimit() const
    {
        return internal::getUserInputOrDefault<uint64_t>(getInputData(), "gas-limit", DEFAULT_GAS_LIMIT);
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
        return DEFAULT_CHAIN_ID;
    }

    uint64_t getVersion() const
    {
        return internal::getUserInputOrDefault<uint64_t>(getInputData(), "version", DEFAULT_VERSION);
    }

    std::shared_ptr<uint32_t> getOptions() const
    {
        return internal::getUserInputIfExists<uint32_t>(getInputData(), "options");
    }
};
}
}

#endif
