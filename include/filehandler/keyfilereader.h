#ifndef KEY_FILE_READER_H
#define KEY_FILE_READER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "isecretkey.h"
#include "account/address.h"
#include "internal/internal.h"

class KeyFileReader : protected IFile, public ISecretKey
{
public:
    explicit KeyFileReader(std::string const &filePath, std::string const &password);

    Address getAddress() const override;

    bytes getSeed() const override;

private:
    EncryptedData getFileContent() const;

    bytes m_secretKey;
};


#endif
