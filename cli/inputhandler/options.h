#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include "cliparser/cxxopts.hpp"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    std::string help() const;

    std::string helpTx() const;

private:
    void initOptions();

    void initOptionsTx();

    cxxopts::Options m_optionsTx;
};

#endif
