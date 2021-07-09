#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include "cliparser/cxxopts.hpp"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    cxxopts::Options pem() const;

    cxxopts::Options esdt() const;

    std::string help() const;

private:
    void initOptions();

    void initOptionsTx();

    void initOptionsPem();

    void initOptionsESDT();

    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsPem;
    cxxopts::Options m_optionsESDT;
};

#endif
