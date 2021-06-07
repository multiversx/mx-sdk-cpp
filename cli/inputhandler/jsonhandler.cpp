#include "jsonhandler.h"
#include <iostream>
#include <fstream>

namespace ih
{
JsonFileHandler::JsonFileHandler(std::string const &filePath) :
        IFile(filePath)
{}

bool JsonFileHandler::isFileValid() const
{
    return IFile::fileExists() && IFile::isFileExtension("json");
}

void JsonFileHandler::writeDataToFile(std::string const &data)
{
    std::ofstream myFile(IFile::getFilePath());

    if (isFileValid())
    {
        myFile << data;
        myFile.close();
        std::cerr << "Data written successfully!\n";
    }
    else std::cerr << "Unable to open json file\n";
}

}
