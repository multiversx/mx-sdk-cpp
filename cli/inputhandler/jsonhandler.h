#pragma warning(disable:4996)
#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include "ifilehandler.h"
#include "wrappers/jsonhandler_input_wrapper.h"
#include "transaction/transaction.h"

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