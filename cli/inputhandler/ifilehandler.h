#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>

namespace ih
{
  class IFileHandler
  {
  public:
    explicit IFileHandler(std::string const& path);

    virtual bool isFileValid() const;

    const std::string &getFilePath() const;

  protected:

    bool fileExists() const;

    bool isFileExtensionValid(std::string const ext) const;

  private:
    std::string getFileExtension() const;

    std::string m_filePath;

  };
}
#endif
