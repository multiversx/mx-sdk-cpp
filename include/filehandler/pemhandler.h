#ifndef PEMHANDLER_H
#define PEMHANDLER_H

#include <string>
#include <vector>

#include "ifilehandler.h"
#include "account/address.h"
#include "internal/internal.h"

namespace ih
{
class PemFileReader : public IFileHandler
{
public:
    explicit PemFileReader(std::string const filePath);

    bool isFileValid() const override;

    Address getAddress() const;

    bytes getSeed() const;

    bytes getPrivateKey() const;

private:
    std::string getFileContent() const;

    bytes getKeyBytesFromContent(std::string const &content) const;

    bytes m_fileKeyBytes;
};
}

#endif
