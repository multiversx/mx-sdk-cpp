#ifndef I_WRAPPER_H
#define I_WRAPPER_H

#include <cliparser/cxxopts.hpp>

namespace ih
{
namespace wrapper
{
class IWrapper
{
public:
    explicit IWrapper(cxxopts::ParseResult const  &inputData) :
            m_inputData(inputData)
    {}

    const cxxopts::ParseResult &getInputData() const
    {
        return m_inputData;
    }

private:
    cxxopts::ParseResult const m_inputData;
};
}
}

#endif
