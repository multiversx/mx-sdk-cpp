#ifndef PEM_HANDLER_WRAPPER_H
#define PEM_HANDLER_WRAPPER_H

#include "iwrapper.h"
#include "utils/params.h"

namespace ih
{
namespace wrapper
{
class PemHandlerInputWrapper : public IWrapper
{
public:
    explicit PemHandlerInputWrapper(cxxopts::ParseResult const &inputData) :
            IWrapper(inputData)
    {}

    const std::string &getPemFilePath() const
    {
        return getInputData()["pem"].as<std::string>();
    }
};
}
}

#endif
