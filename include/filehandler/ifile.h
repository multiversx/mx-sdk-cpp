#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>

namespace ih
{
class IFile
{
public:
    explicit IFile(std::string const &path);

    virtual bool isFileValid() const = 0;

    const std::string &getFilePath() const;

protected:

    bool fileExists() const;

    bool isFileExtension(std::string const ext) const;

private:
    std::string getFileExtension() const;

    std::string m_filePath;

};
}
#endif
