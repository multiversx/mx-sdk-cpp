#include "arghandler.h"
#include "utils/params.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <iostream>



namespace internal
{
// Generic template function to check for user input value.
template<typename T>
bool isUserInputValid(T &arg)
{
    return true;
}

// Specialization template function to check for user input value.
// Expects input to be a non-empty string.
template<>
bool isUserInputValid(std::string const &arg)
{
    return (!arg.empty());
}
}






namespace ih
{

RequestedCmd::RequestedCmd(std::map<uint32_t, std::string> const &userInputs,
                           RequestType const &reqType, errorCode const &errCode) :
        m_userInputs(userInputs),
        m_requestType(reqType),
        m_errCode(errCode)
{}

const std::map<uint32_t, std::string> &RequestedCmd::getUserInputs() const
{
    return m_userInputs;
}

const RequestType &RequestedCmd::getRequestType() const
{
    return m_requestType;
}

const errorCode &RequestedCmd::getErrorCode() const
{
    return m_errCode;
}


ArgHandler::ArgHandler() :
    m_errCode(ERROR_NONE),
    m_optionsTx("transaction", "Manage transactions"),
    m_optionsPem("pem", "Manage pem files")
{
    initOptions();
}

bool ArgHandler::isCmd(std::string const& arg)
{
    return arg == m_cmd;
}

bool ArgHandler::isSubCmd(std::string const& arg)
{
    return arg == m_subCmd;
}

bool ArgHandler::canParse(int const &argc, char *const argv[], cxxopts::Options &options)
{
    bool ret = false;
    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& e)
    {
        throw;
    }

    auto arguments = result.arguments();

    for (auto const &arg : arguments)
    {
        if (arg.value().empty())
        {
            throw std::invalid_argument("Empty " + arg.key());
        }
    }

    if (result.unmatched().size() != 2) throw std::invalid_argument("Invalid number of arguments");

    m_result = result;

    return ret = true;
}

ParseResult ArgHandler::parse(int const &argc, char *const argv[])
{
    RequestType reqType = invalid;

    if(argc > 1)
    {
        m_cmd = std::string(argv[1]);
        if (argc > 2)
        {
            m_subCmd = std::string(argv[2]);
        }
    }

    if (isCmd("pem") && isSubCmd("load") &&
        canParse(argc,argv,m_optionsPem))
    {
        reqType = loadPemFile;
    }
    else if (isCmd("transaction") && isSubCmd("new") &&
             canParse(argc, argv, m_optionsTx))
    {
        reqType = createSignedTransactionWithPemFile;
    }

    return ParseResult {reqType, cxxopts::ParseResult()};
}


void ArgHandler::initOptionsTx()
{
    m_optionsTx.add_options("new") // transaction new group
            ("nonce", "Transaction nonce", cxxopts::value<uint64_t>())
            ("value", "Transaction value", cxxopts::value<std::string>())
            ("receiver", "Receiver's address", cxxopts::value<std::string>())
            ("receiver-username", "Receiver's username (default: )", cxxopts::value<std::string>()->default_value(""))
            ("sender-username", "Sender's username (default: )", cxxopts::value<std::string>()->default_value(""))
            ("gas-price", "Transaction gas price", cxxopts::value<uint64_t>())
            ("gas-limit", "Transaction gas limit", cxxopts::value<uint64_t>())
            ("data", "Transaction data/payload (default: )", cxxopts::value<std::string>()->default_value(""))
            ("chainID", "Chain identifier (default: T)", cxxopts::value<std::string>()->default_value("T"))
            ("version", "Transaction version (default: 1)", cxxopts::value<std::string>()->default_value("1"))
            ("options", "Transaction options (default: )", cxxopts::value<std::string>()->default_value(""))
            ("pem", "Pem keyfile", cxxopts::value<std::string>())
            ("outfile", "Json file where the output will be stored", cxxopts::value<std::string>());
}

void ArgHandler::initOptionsPem()
{
    m_optionsPem.add_options("load") // pem load group
            ("f,file", "Load a pem file to check if valid", cxxopts::value<std::string>());
}


void ArgHandler::initOptions()
{
    initOptionsTx();
    initOptionsPem();
}

}
