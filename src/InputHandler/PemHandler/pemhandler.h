#ifndef PEMHANDLER_H
#define PEMHANDLER_H

#include <string>
#include <vector>
#include "../IFileHandler/ifilehandler.h"
#include "../InputWrappers/pemhandler_input_wrapper.h"

namespace ih
{
  class PemFileHandler : public IFileHandler
  {
    std::string const hrp = "erd";

  public:
    explicit PemFileHandler(wrapper::PemHandlerInputWrapper const inputWrapper);

    bool isFileValid() const override;

    void getSeed(unsigned char* seed) const;

    void getPublicKey(unsigned char* pk) const;

    void getPrivateKey(unsigned char* pk) const;

    std::string getSegwitAddress() const;

    void printFileContent() const;

  private:

    std::string getFileContent() const;

    std::vector<char> getKeyBytesFromFile() const;

    std::string m_fileContent;
    wrapper::PemHandlerInputWrapper m_inputData;
  };
}

#endif