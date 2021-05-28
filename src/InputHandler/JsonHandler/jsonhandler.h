#pragma warning(disable:4996)
#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <map>
#include <string>
#include "../../Utils/ext.h"
#include "../PemHandler/pemhandler.h"
#include "../InputWrappers/jsonhandler_input_wrapper.h"
#include <sodium.h>
#include "../transaction.h"


namespace ih
{
  class JsonFileHandler : public IFileHandler
  {
  public:
    explicit JsonFileHandler(wrapper::JsonHandlerInputWrapper const jsonInputWrapper);
    
    bool isFileValid() const override;

    void writeOutputFile(Transaction const& transaction);

  private:

    wrapper::JsonHandlerInputWrapper m_inputData;
  };
}

#endif