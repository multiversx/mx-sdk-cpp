#ifndef JSONHANDLER_WRAPPER_H
#define JSONHANDLER_WRAPPER_H

#include <string>
#include <map>

#include "iwrapper.h"
#include "utils/params.h"
#include "account/address.h"
#include "internal/internal.h"

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
    }

    bool isDataEmpty() const
    {
        return !m_containsData;
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

    uint64_t getGasPrice() const
    {
        return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_PRICE));
    }

    uint64_t getGasLimit() const
    {
        return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_LIMIT));
    }

    bytes getData() const
    {
        if (!isDataEmpty())
        {
            std::string userData = getInputData().at(ARGS_TX_IDX_DATA);
            return bytes(userData.begin(),userData.end());
        }
        else return bytes();
    }

    std::string getChainId() const
    {
        return JSON_TX_DEFAULT_CHAIN_ID;
    }

    uint64_t getVersion() const
    {
        return JSON_TX_DEFAULT_VERSION;
    }

    std::string getInputFile() const
    {
        return getInputData().at(ARGS_TX_IDX_PEM_INPUT_FILE);
    }

    std::string getOutputFile() const
    {
        return getInputData().at(ARGS_TX_IDX_JSON_OUT_FILE);
    }

private:
    bool m_containsData;
};
}
}

#endif
