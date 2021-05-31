#ifndef JSONHANDLER_WRAPPER_H
#define JSONHANDLER_WRAPPER_H

#include <string>
#include <map>
#include "iwrapper.h"
#include "utils/params.h"

namespace ih
{
    namespace wrapper
    {
        class JsonHandlerInputWrapper : public IWrapper
        {
        public:
            JsonHandlerInputWrapper(std::map<uint32_t, std::string> const &inputData) :
                    IWrapper(inputData)
            {
                m_containsData = (getInputData().find(ARGS_TX_IDX_DATA) != getInputData().end()) ?
                                 (true) : (false);
            }

            bool isDataEmpty() const
            {
                return m_containsData;
            }

            uint64_t getNonce() const
            {
                return std::stoul(getInputData().at(ARGS_TX_IDX_NONCE));
            }

            std::string getValue() const
            {
                return getInputData().at(ARGS_TX_IDX_VALUE);
            }

            std::string getReceiver() const
            {
                return getInputData().at(ARGS_TX_IDX_RECEIVER);
            }

            uint64_t getGasPrice() const
            {
                return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_PRICE));
            }

            uint64_t getGasLimit() const
            {
                return std::stoul(getInputData().at(ARGS_TX_IDX_GAS_LIMIT));
            }

            std::string getData() const
            {
                return (isDataEmpty()) ? ("") : getInputData().at(ARGS_TX_IDX_DATA);
            }

            std::string getChainId() const
            {
                return JSON_TX_DEFAULT_CHAIN_ID;;
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