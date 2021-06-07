#ifndef PEMHANDLER_H
#define PEMHANDLER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "account/address.h"
#include "internal/internal.h"

namespace ih
{
class PemFileReader : public IFile
{
public:
    explicit PemFileReader(std::string const filePath);

    bool isFileValid() const override;

    Address getAddress() const;

    bytes getSeed() const;

    bytes getPrivateKey() const;

private:
    std::string getFileContent() const;

    bytes getKeyBytesFromFile() const;

    std::string m_fileContent;
};
}

#endif
