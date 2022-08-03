#include "arghandler.h"
#include "utils/params.h"

namespace
{
std::string parseCmd(int const &argc, char *const argv[])
{
    return (argc > 1) ? (std::string(argv[1])) : (std::string());
}

std::string parseSubCmd(int const &argc, char *const argv[])
{
    return (argc > 2) ? (std::string(argv[2])) : (std::string());
}

void checkEmptyValues(std::vector<cxxopts::KeyValue> const &arguments)
{
    for (auto const &arg : arguments)
    {
        if (arg.value().empty())
        {
            throw std::invalid_argument(ERROR_MSG_EMPTY_VALUE + arg.key());
        }
    }
}
}

namespace ih
{

ArgHandler::ArgHandler() : m_options() {}

bool ArgHandler::isCmd(std::string const& arg) const
{
    return arg == m_cmd;
}

bool ArgHandler::isSubCmd(std::string const& arg) const
{
    return arg == m_subCmd;
}

bool ArgHandler::isCmdHelp() const
{
    return isCmd("help") || isCmd("--help") || isCmd("-h");
}

bool ArgHandler::isSubCmdHelp() const
{
    return isSubCmd("help") || isSubCmd("--help") || isSubCmd("-h");
}

bool ArgHandler::canParse(int const &argc, char *const argv[], cxxopts::Options options)
{
    try
    {
        m_result = options.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& e)
    {
        throw;
    }

    if (m_result.unmatched().size() != 2)
    {
        throw std::invalid_argument("Invalid number of arguments");
    }

    checkEmptyValues(m_result.arguments());

    return true;
}

ArgParsedResult ArgHandler::parse(int const &argc, char *const argv[])
{
    RequestType reqType = invalid;
    std::string helpMsg;

    m_cmd = parseCmd(argc, argv);
    m_subCmd = parseSubCmd(argc, argv);

    if (isCmdHelp() && argc == 2)
    {
        helpMsg = m_options.help();
        reqType = help;
    }
    else if (isCmd("transaction") && isSubCmdHelp() && argc == 3)
    {
        helpMsg = m_options.transaction().help();
        reqType = help;
    }
    else if (isCmd("esdt") && isSubCmdHelp() && argc == 3)
    {
        helpMsg = m_options.esdt().help();
        reqType = help;
    }
    else if (isCmd("network") && isSubCmdHelp() && argc == 3)
    {
        helpMsg = m_options.network().help();
        reqType = help;
    }
    else if (isCmd("transaction") && isSubCmd("new") &&
             canParse(argc, argv, m_options.transaction()))
    {
        reqType = createSignedTransaction;
    }
    else if (isCmd("esdt") && isSubCmd("issue") &&
            canParse(argc, argv, m_options.esdt().issue()))
    {
        reqType = issueESDT;
    }
    else if (isCmd("esdt") && isSubCmd("transfer") &&
             canParse(argc, argv, m_options.esdt().transfer()))
    {
        reqType = transferESDT;
    }
    else if (isCmd("network") && isSubCmd("set") &&
             canParse(argc, argv, m_options.network()))
    {
        reqType = setNetworkConfig;
    }

    return ArgParsedResult {reqType, helpMsg, m_result};
}

}
