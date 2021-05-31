#include "transactionhandler.h"
#include <iostream>
#include <fstream>

namespace ih
{
TransactionInputHandler::TransactionInputHandler(wrapper::TransactionInputWrapper const jsonInputWrapper) :
        IFileHandler(jsonInputWrapper.getOutputFile()),
        m_inputData(jsonInputWrapper)
{}

bool TransactionInputHandler::isFileValid() const
{
    return IFileHandler::fileExists() && IFileHandler::isFileExtension("json");
}

void TransactionInputHandler::writeTransactionToJsonFile(Transaction const &transaction)
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
