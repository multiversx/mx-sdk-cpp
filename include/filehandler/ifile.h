#ifndef I_FILE_H
#define I_FILE_H

#include <string>

class IFile
{
public:
    explicit IFile(std::string path, std::string expectedExtension = "");

    const std::string &getFilePath() const;

protected:
    virtual void checkFile() const;

    bool fileExists() const;

    static std::string getFileExtension(std::string const &filePath);

private:
    std::string const m_filePath;
    std::string const m_expectedExtension;
};

#endif
