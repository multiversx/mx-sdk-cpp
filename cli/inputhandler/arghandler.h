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

    void checkEmptyValues(std::vector<cxxopts::KeyValue>const &arguments);

    std::string parseCmd(int const &argc, char *const argv[]);

    std::string parseSubCmd(int const &argc, char *const argv[]);

    bool isCmd(std::string const& arg);

    bool isSubCmd(std::string const& arg);

    std::string m_cmd;
    std::string m_subCmd;
    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsPem;
    cxxopts::ParseResult m_result;
};
}

#endif // !ARGHANDLER_H
