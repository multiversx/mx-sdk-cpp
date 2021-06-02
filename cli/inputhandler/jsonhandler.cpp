#include "jsonhandler.h"
#include <iostream>
#include <fstream>

namespace ih
{
JsonFileHandler::JsonFileHandler(std::string const &filePath) :
        IFileHandler(filePath)
{}

bool JsonFileHandler::isFileValid() const
{
    return IFileHandler::fileExists() && IFileHandler::isFileExtension("json");
}

void JsonFileHandler::writeDataToFile(std::string const &data)
{
    std::ofstream myFile(IFileHandler::getFilePath());

    if (isFileValid())
    {
        myFile << data;
        myFile.close();
        std::cerr << "Data written successfully!\n";
    }
    else std::cerr << "Unable to open json file\n";
}

}
