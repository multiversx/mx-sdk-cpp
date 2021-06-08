#pragma warning(disable:4996)
#ifndef TRANSACTION_HANDLER_H
#define TRANSACTION_HANDLER_H

#include "filehandler/ifile.h"
#include "wrappers/transaction_input_wrapper.h"
#include "transaction/transaction.h"

namespace ih
{
class JsonFileHandler : public IFile
{
public:
    explicit JsonFileHandler(std::string const &filePath);

    bool isFileValid() const override;

    void writeDataToFile(std::string const &data);
};
}

#endif
