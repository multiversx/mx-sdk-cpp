#include "jsonhandler.h"
#include <iostream>
#include <fstream>

namespace ih
{
JsonFileHandler::JsonFileHandler(wrapper::JsonHandlerInputWrapper const jsonInputWrapper) :
        IFileHandler(jsonInputWrapper.getOutputFile()),
        m_inputData(jsonInputWrapper)
{}

bool JsonFileHandler::isFileValid() const
{
    return IFileHandler::fileExists() && IFileHandler::isFileExtensionValid("json");
}

void JsonFileHandler::writeOutputFile(Transaction const &transaction)
{
    std::ofstream myFile(m_inputData.getOutputFile());

    if (isFileValid())
    {
        myFile << transaction.getSerializedTransaction();;
        myFile.close();
        std::cerr << "Succesfully created transaction!\n";
    }
    else std::cerr << "Unable to open json file\n";
}

}
