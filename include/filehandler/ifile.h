#ifndef I_FILE_H
#define I_FILE_H

#include <string>

namespace ih
{
class IFile
{
public:
    explicit IFile(std::string path);

    const std::string &getFilePath() const;

protected:
    virtual bool checkFile() const = 0;

    bool fileExists() const;

    bool isFileExtension(std::string const &ext) const;

private:
    std::string getFileExtension() const;

    std::string const m_filePath;

};
}
#endif
