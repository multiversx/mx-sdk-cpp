#pragma warning(disable:4996)
#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <map>
#include <string>
#include "../../Utils/ext.h"
#include "../PemHandler/pemhandler.h"
#include "../InputWrappers/jsonhandler_input_wrapper.h"
#include <sodium.h>


namespace ih
{
  class JsonHandler : public IFileHandler
  {
  public:
    explicit JsonHandler(wrapper::PemHandlerInputWrapper const pemInputWrapper,
                         wrapper::JsonHandlerInputWrapper const jsonInputWrapper);

    //TODO: handle if pem/json input is not path
    bool isFileValid() const override;

    void writeOutputFile();

  private:

    std::string getSender();

    std::string getSignature(nlohmann::ordered_json unsignedTransaction);

    nlohmann::ordered_json createUnsignedTransaction();

    void signTransaction(nlohmann::ordered_json& transaction);


    wrapper::JsonHandlerInputWrapper m_inputData;
    ih::PemFileHandler m_pemHandler;
  };
}

#endif