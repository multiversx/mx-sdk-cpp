#ifndef ERD_CLI_ARG_HANDLER_H
#define ERD_CLI_ARG_HANDLER_H

#include "utils/errors.h"
#include "options.h"

namespace ih
{
enum RequestType
{
    invalid,
    help,
    createSignedTransaction,
    issueESDT,
    transferESDT,
    setNetworkConfig
};

struct ArgParsedResult
{
    RequestType requestType;
    std::string help;
    cxxopts::ParseResult result;
};

class ArgHandler
{
public:

    explicit ArgHandler();

    ArgParsedResult parse(int const &argc, char *const argv[]);

private:

    bool canParse(int const &argc, char *const argv[], cxxopts::Options options);

    bool isCmd(std::string const& arg) const;

    bool isSubCmd(std::string const& arg) const;

    bool isCmdHelp() const;

    bool isSubCmdHelp() const;

    std::string m_cmd;
    std::string m_subCmd;
    CLIOptions m_options;
    cxxopts::ParseResult m_result;
};
}

#endif // !ARGHANDLER_H
