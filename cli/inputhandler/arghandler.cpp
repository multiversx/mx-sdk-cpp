#include "arghandler.h"
#include "utils/params.h"

#include <cstdint>

namespace ih
{

ArgHandler::ArgHandler() :
    m_optionsTx("transaction", "Manage transactions"),
    m_optionsPem("pem", "Manage pem files")
{
    initOptions();
}

std::string ArgHandler::parseCmd(int const &argc, char *const argv[])
{
    return (argc > 1) ? (std::string(argv[1])) : (std::string());
}

std::string ArgHandler::parseSubCmd(int const &argc, char *const argv[])
{
    return (argc > 2) ? (std::string(argv[2])) : (std::string());
}

bool ArgHandler::isCmd(std::string const& arg)
{
    return arg == m_cmd;
}

bool ArgHandler::isSubCmd(std::string const& arg)
{
    return arg == m_subCmd;
}

void ArgHandler::checkEmptyValues(std::vector<cxxopts::KeyValue> const &arguments)
{
    for (auto const &arg : arguments)
    {
        if (arg.value().empty())
        {
            throw std::invalid_argument(ERROR_MSG_EMPTY_VALUE + arg.key());
        }
    }
}

bool ArgHandler::canParse(int const &argc, char *const argv[], cxxopts::Options &options)
{
    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& e)
    {
        throw;
    }

    if (result.unmatched().size() != 2)
    {
        throw std::invalid_argument("Invalid arguments");
    }

    checkEmptyValues(result.arguments());

    m_result = result;

    return true;
}

ParseResult ArgHandler::parse(int const &argc, char *const argv[])
{
    RequestType reqType = invalid;
    std::string helpMsg;

    m_cmd = parseCmd(argc, argv);
    m_subCmd = parseSubCmd(argc, argv);

    if (isCmd("help") && argc == 2)
    {
        helpMsg = m_optionsTx.help() + "\n" + m_optionsPem.help();

        reqType = help;
    }
    else if (isCmd("pem") && isSubCmd("help") && argc == 3)
    {
        helpMsg = m_optionsPem.help();
        reqType = help;
    }
    else if (isCmd("transaction") && isSubCmd("help") && argc == 3)
    {
        helpMsg = m_optionsTx.help();
        reqType = help;
    }
    else if (isCmd("pem") && isSubCmd("load") &&
        canParse(argc, argv,m_optionsPem))
    {
        reqType = loadPemFile;
    }
    else if (isCmd("transaction") && isSubCmd("new") &&
             canParse(argc, argv, m_optionsTx))
    {
        reqType = createSignedTransactionWithPemFile;
    }

    return ParseResult {reqType, helpMsg, m_result};
}


void ArgHandler::initOptionsTx()
{
    m_optionsTx.add_options("new") // transaction new group
            ("nonce", "Transaction nonce", cxxopts::value<uint64_t>())
            ("value", "Transaction value", cxxopts::value<std::string>())
            ("receiver", "Receiver's address", cxxopts::value<std::string>())
            ("receiver-username", "Receiver's username", cxxopts::value<std::string>()->default_value(""))
            ("sender-username", "Sender's username", cxxopts::value<std::string>()->default_value(""))
            ("gas-price", "Transaction gas price", cxxopts::value<uint64_t>())
            ("gas-limit", "Transaction gas limit", cxxopts::value<uint64_t>())
            ("data", "Transaction data/payload", cxxopts::value<std::string>()->default_value(""))
            ("chainID", "Chain identifier", cxxopts::value<std::string>()->default_value("T"))
            ("version", "Transaction version", cxxopts::value<uint64_t>()->default_value("1"))
            ("options", "Transaction options", cxxopts::value<uint32_t>()->default_value("0"))
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
