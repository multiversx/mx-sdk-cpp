#ifndef JSON_FILE_H
#define JSON_FILE_H

#include "filehandler/ifile.h"
#include "wrappers/transaction_input_wrapper.h"
#include "transaction/transaction.h"

namespace ih
{
class JsonFile : public IFile
{
public:
    explicit JsonFile(std::string const &filePath);

    void writeDataToFile(std::string const &data);
};
}

#endif
