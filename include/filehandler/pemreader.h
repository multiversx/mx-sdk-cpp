#ifndef PEM_HANDLER_H
#define PEM_HANDLER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "isecretkey.h"
#include "account/address.h"
#include "internal/internal.h"

class PemFileReader : protected IFile, public ISecretKey
{
public:
    explicit PemFileReader(std::string const &filePath);

    Address getAddress() const override;

    bytes getSeed() const override;

private:
    std::string getFileContent() const;

    bytes getKeyBytesFromContent(std::string const &content) const;

    bytes m_fileKeyBytes;
};


#endif
