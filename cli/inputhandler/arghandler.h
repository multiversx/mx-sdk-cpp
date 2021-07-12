#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include "utils/errors.h"
#include "options.h"

namespace ih
{
enum RequestType
{
    invalid,
    help,
    loadPemFile,
    createSignedTransactionWithPemFile,
    issueESDT,
    transferESDT
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

    bool isCmd(std::string const& arg);

    bool isSubCmd(std::string const& arg);

    std::string m_cmd;
    std::string m_subCmd;
    CLIOptions m_options;
    cxxopts::ParseResult m_result;
};
}

#endif // !ARGHANDLER_H
