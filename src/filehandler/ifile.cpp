#include "filehandler/ifile.h"
#include <fstream>
#include <utility>

namespace ih
{
IFile::IFile(std::string path) :
        m_filePath(std::move(path))
{}

const std::string &IFile::getFilePath() const
{
    return m_filePath;
}

bool IFile::fileExists() const
{
    std::ifstream f(m_filePath);
    return f.good() && f.is_open();
}

bool IFile::isFileExtension(std::string const &ext) const
{
    return getFileExtension() == ext;
}

std::string IFile::getFileExtension() const
{
    std::string ext;

    if (m_filePath.find_last_of('.') != std::string::npos)
        ext = m_filePath.substr(m_filePath.find_last_of('.') + 1);

    return ext;
}

}
