#ifndef PEMHANDLER_WRAPPER_H
#define PEMHANDLER_WRAPPER_H

#include <string>
#include <map>
#include "iwrapper.h"
#include "utils/params.h"

namespace ih
{
namespace wrapper
{
class PemHandlerInputWrapper : public IWrapper
{
public:
    PemHandlerInputWrapper(std::map<uint32_t, std::string> const &inputData) :
            IWrapper(inputData)
    {}

    const std::string &getPemFilePath() const
    {
        return getInputData().at(ARGS_TX_IDX_PEM_INPUT_FILE);
    }
};
}
}

#endif