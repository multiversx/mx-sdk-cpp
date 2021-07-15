#ifndef KEY_FILE_READER_H
#define KEY_FILE_READER_H

#include <string>
#include <vector>

#include "ifile.h"
#include "account/address.h"
#include "internal/internal.h"

class KeyFileReader : public IFile
{
public:
    explicit KeyFileReader(std::string const &filePath, std::string const &password);

    Address getAddress() const;

    bytes getSeed() const;

protected:
    void checkFile() const override;

private:
    EncryptedData getFileContent() const;

    bytes m_secretKey;
};


#endif
