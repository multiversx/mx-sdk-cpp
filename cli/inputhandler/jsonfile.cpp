#include "jsonfile.h"
#include "utils/errors.h"

#include <iostream>
#include <fstream>

namespace ih
{
JsonFile::JsonFile(std::string const &filePath) :
        IFile(filePath)
{
    try
    {
        JsonFile::checkFile();
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

bool JsonFile::checkFile() const
{
    if (!IFile::fileExists()) throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST);
    if (!IFile::isFileExtension("json")) throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);

    return true;
}

void JsonFile::writeDataToFile(std::string const &data)
{
    std::ofstream myFile(IFile::getFilePath());

    if (checkFile())
    {
        myFile << data;
        myFile.close();
        std::cerr << "Data written successfully!\n";
    }
    else std::cerr << "Unable to open json file\n";
}

}
