#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include <string>
#include <vector>
#include <map>

#include "utils/errors.h"
//#include "options.h"
#include "cliparser/cxxopts.hpp"




namespace ih
{
enum RequestType
{
    invalid,
    help,
    loadPemFile,
    createSignedTransactionWithPemFile
};

class RequestedCmd
{
public:
    RequestedCmd(std::map<uint32_t, std::string> const &userInputs,
                 RequestType const &reqType, errorCode const &errCode);

    const std::map<uint32_t, std::string> &getUserInputs() const;

    const RequestType &getRequestType() const;

    const errorCode &getErrorCode() const;

private:
    std::map<uint32_t, std::string> const m_userInputs;
    RequestType const m_requestType;
    errorCode const m_errCode;
};

struct ParseResult
{
    RequestType requestType;
    cxxopts::ParseResult result;
};

class ArgHandler
{
public:

    explicit ArgHandler();

    ParseResult parse(int const &argc, char *const argv[]);

private:

    void initOptions();

    void initOptionsTx();

    void initOptionsPem();

    bool canParse(int const &argc, char *const argv[], cxxopts::Options &options);

    bool isCmd(std::string const& arg);

    bool isSubCmd(std::string const& arg);

    std::string m_cmd;
    std::string m_subCmd;
    std::vector<std::string> m_arguments;
    errorCode m_errCode;
    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsPem;
    cxxopts::ParseResult m_result;
};
}

#endif // !ARGHANDLER_H
