#include "transaction_input_wrapper.h"

#include "internal/internal.h"
#include "transaction/transaction.h"

namespace internal
{
template<class T>
inline std::shared_ptr<T> getUserInputIfExists(cxxopts::ParseResult const &userInputs,
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
inline std::shared_ptr<bytes> getUserInputIfExists(cxxopts::ParseResult const &userInputs,
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

template<typename T>
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

TransactionInputWrapper::TransactionInputWrapper(cxxopts::ParseResult const &inputData) : IWrapper(inputData)
{}

uint64_t TransactionInputWrapper::getNonce() const
{
    return internal::getUserInputOrDefault<uint64_t>(getInputData(), "nonce", DEFAULT_NONCE);
}

BigUInt TransactionInputWrapper::getValue() const
{
    return BigUInt(internal::getUserInputOrDefault<std::string>(getInputData(), "value", DEFAULT_VALUE.getValue()));
}

std::shared_ptr<Address> TransactionInputWrapper::getReceiver() const
{
    if (getInputData().count("receiver") != 0)
    {
        std::string const bech32Address = getInputData()["receiver"].as<std::string>();
        Address const receiver(bech32Address);
        return std::make_shared<Address>(receiver);
    }

    return DEFAULT_RECEIVER;
}

std::shared_ptr<bytes> TransactionInputWrapper::getReceiverName() const
{
    return internal::getUserInputIfExists<bytes>(getInputData(), "receiver-name");
}

std::shared_ptr<bytes> TransactionInputWrapper::getSenderName() const
{
    return internal::getUserInputIfExists<bytes>(getInputData(), "sender-name");
}

uint64_t TransactionInputWrapper::getGasPrice() const
{
    return internal::getUserInputOrDefault<uint64_t>(getInputData(), "gas-price", DEFAULT_GAS_PRICE);
}

uint64_t TransactionInputWrapper::getGasLimit() const
{
    return internal::getUserInputOrDefault<uint64_t>(getInputData(), "gas-limit", DEFAULT_GAS_LIMIT);
}

std::shared_ptr<bytes> TransactionInputWrapper::getData() const
{
    return internal::getUserInputIfExists<bytes>(getInputData(), "data");
}

uint64_t TransactionInputWrapper::getVersion() const
{
    return internal::getUserInputOrDefault<uint64_t>(getInputData(), "version", DEFAULT_VERSION);
}

std::shared_ptr<uint32_t> TransactionInputWrapper::getOptions() const
{
    return internal::getUserInputIfExists<uint32_t>(getInputData(), "options");
}

}
}


