#ifndef PEMHANDLER_H
#define PEMHANDLER_H

#include <string>
#include <vector>
#include "../IFileHandler/ifilehandler.h"
#include "../InputWrappers/pemhandler_input_wrapper.h"
#include "../address.h"

namespace ih
{
  class PemFileHandler : public IFileHandler
  {
  public:
    explicit PemFileHandler(wrapper::PemHandlerInputWrapper const inputWrapper);

    bool isFileValid() const override;

    Address getAddress() const;

    void getSeed(unsigned char* seed) const;

    void getPrivateKey(unsigned char* pk) const;

    void printFileContent() const;

  private:

    std::string getFileContent() const;

    std::vector<char> getKeyBytesFromFile() const;

    std::string m_fileContent;
    wrapper::PemHandlerInputWrapper m_inputData;
  };
}

#endif