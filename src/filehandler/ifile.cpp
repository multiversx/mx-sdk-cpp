#include "filehandler/ifile.h"
#include "common.h"

#include <fstream>
#include <utility>

IFile::IFile(std::string path, std::string expectedExtension) :
        m_filePath(std::move(path)),
        m_expectedExtension(std::move(expectedExtension))
{
    try
    {
        IFile::checkFile();
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

const std::string &IFile::getFilePath() const
{
    return m_filePath;
}

void IFile::checkFile() const
{
    if (!IFile::fileExists())
    {
        throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST + getFilePath());
    }
    if (!m_expectedExtension.empty())
    {
        std::string const fileExtension = getFileExtension(getFilePath());
        util::checkParam(fileExtension, m_expectedExtension, ERROR_MSG_FILE_EXTENSION_INVALID);
    }
}

bool IFile::fileExists() const
{
    std::ifstream f(m_filePath);
    return f.good() && f.is_open();
}

std::string IFile::getFileExtension(const std::string &filePath)
{
    std::string ext;
    auto const dotPosition = filePath.find_last_of('.');

    if (dotPosition != std::string::npos)
        ext = filePath.substr(dotPosition + 1);

    return ext;
}

