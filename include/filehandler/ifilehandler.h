#ifndef I_FILE_HANDLER_H
#define I_FILE_HANDLER_H

#include <string>

namespace ih
{
class IFileHandler
{
public:
    explicit IFileHandler(std::string const &path);

    const std::string &getFilePath() const;

protected:
    virtual bool isFileValid() const = 0;

    bool fileExists() const;

    bool isFileExtension(std::string const &ext) const;

private:
    std::string getFileExtension() const;

    std::string const m_filePath;

};
}
#endif
