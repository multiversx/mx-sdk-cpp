#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include "cliparser/cxxopts.hpp"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    cxxopts::Options pem() const;

    std::string help() const;

    std::string helpPem() const;

    std::string helpTx() const;

private:
    void initOptions();

    void initOptionsTx();

    void initOptionsPem();

    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsPem;
};

#endif
