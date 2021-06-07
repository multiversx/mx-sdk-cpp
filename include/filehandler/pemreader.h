#ifndef PEM_HANDLER_H
#define PEM_HANDLER_H

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
    explicit PemFileReader(std::string const &filePath);

    Address getAddress() const;

    bytes getSeed() const;

    bytes getPrivateKey() const;

protected:
    bool isFileValid() const override;

private:
    std::string getFileContent() const;

    bytes getKeyBytesFromContent(std::string const &content) const;

    bytes m_fileKeyBytes;
};
}

#endif
