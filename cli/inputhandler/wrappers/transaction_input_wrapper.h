#ifndef JSONHANDLER_WRAPPER_H
#define JSONHANDLER_WRAPPER_H

#include <string>
#include <map>

#include "iwrapper.h"
#include "utils/params.h"
#include "account/address.h"
#include "internal/internal.h"
#include "transaction/transaction.h"

namespace internal
{
template<class T>
inline std::shared_ptr<T> getUserInputIfExists(std::map<uint32_t, std::string> const &userInputs,
                                               unsigned int const &idx, bool const &exists)
{
    if (exists)
    {
        std::string const& userInput = userInputs.at(idx);
        auto const input = std::stoul(userInput);
        return std::make_shared<T>(input);
    }
    else return nullptr;
}

template<>
inline std::shared_ptr<bytes> getUserInputIfExists(std::map<uint32_t, std::string> const &userInputs,
                                                   unsigned int const &idx, bool const &exists)
{
    if (exists)
    {
        std::string const& userInput = userInputs.at(idx);
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
    explicit TransactionInputWrapper(std::map<uint32_t, std::string> const &inputData) :
            IWrapper(inputData)
    {
        m_containsData = (getInputData().find(ARGS_TX_IDX_DATA) != getInputData().end());
        m_containsReceiverName = (getInputData().find(ARGS_TX_IDX_RECEIVER_NAME) != getInputData().end());
        m_containsSenderName = (getInputData().find(ARGS_TX_IDX_SENDER_NAME) != getInputData().end());
        m_containsOptions = (getInputData().find(ARGS_TX_IDX_OPTIONS) != getInputData().end());
        m_containsChainID = (getInputData().find(ARGS_TX_IDX_CHAIN_ID) != getInputData().end());
    }

    uint64_t getNonce() const
    {
        return std::stoul(getInputData().at(ARGS_TX_IDX_NONCE));
    }

    std::string getValue() const
    {
        return getInputData().at(ARGS_TX_IDX_VALUE);
    }

    Address getReceiver() const
    {
        return Address(getInputData().at(ARGS_TX_IDX_RECEIVER));
    }

    std::shared_ptr<bytes> getReceiverName() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), ARGS_TX_IDX_RECEIVER_NAME, m_containsReceiverName);
    }

    std::shared_ptr<bytes> getSenderName() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), ARGS_TX_IDX_SENDER_NAME, m_containsSenderName);
    }

    uint64_t getGasPrice() const
    {
        return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_PRICE));
    }

    uint64_t getGasLimit() const
    {
        return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_LIMIT));
    }

    std::shared_ptr<bytes> getData() const
    {
        return internal::getUserInputIfExists<bytes>(getInputData(), ARGS_TX_IDX_DATA, m_containsData);
    }

    std::shared_ptr<std::string> getSignature() const
    {
        return DEFAULT_SIGNATURE;
    }

    std::string getChainId() const
    {
        return m_containsChainID ? (getInputData().at(ARGS_TX_IDX_CHAIN_ID)) : (DEFAULT_CHAIN_ID);
    }

    uint64_t getVersion() const
    {
        return DEFAULT_VERSION;
    }

    std::string getInputFile() const
    {
        return getInputData().at(ARGS_TX_IDX_PEM_INPUT_FILE);
    }

    std::string getOutputFile() const
    {
        return getInputData().at(ARGS_TX_IDX_JSON_OUT_FILE);
    }

    std::shared_ptr<uint32_t> getOptions() const
    {
        return internal::getUserInputIfExists<uint32_t>(getInputData(), ARGS_TX_IDX_OPTIONS, m_containsOptions);
    }

private:

    bool m_containsData;
    bool m_containsReceiverName;
    bool m_containsSenderName;
    bool m_containsOptions;
    bool m_containsChainID;
};
}
}

#endif
