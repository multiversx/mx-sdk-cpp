#ifndef PEMHANDLER_H
#define PEMHANDLER_H

#include <string>
#include <vector>

#include "ifilehandler.h"
#include "wrappers/pemhandler_input_wrapper.h"
#include "account/address.h"
#include "internal/internal.h"

namespace ih
{
class PemFileHandler : public IFileHandler
{
public:
    explicit PemFileHandler(wrapper::PemHandlerInputWrapper const inputWrapper);

    bool isFileValid() const override;

    Address getAddress() const;

    bytes getSeed() const;

    bytes getPrivateKey() const;

    void printFileContent() const;

private:
    std::string getFileContent() const;

    bytes getKeyBytesFromFile() const;

    std::string m_fileContent;
    wrapper::PemHandlerInputWrapper m_inputData;
};
}

#endif