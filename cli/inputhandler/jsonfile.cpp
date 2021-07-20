#include "jsonfile.h"
#include "utils/errors.h"

#include <iostream>
#include <fstream>

namespace ih
{
JsonFile::JsonFile(std::string const &filePath) :
        IFile(filePath, "json")
{}


void JsonFile::writeDataToFile(std::string const &data)
{
    std::ofstream myFile(IFile::getFilePath());

    if (IFile::fileExists())
    {
        myFile << data;
        myFile.close();
        std::cerr << "Data written successfully!\n";
    }
    else std::cerr << "Unable to open json file\n";
}

}
