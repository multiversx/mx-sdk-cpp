#ifndef ERD_CLI_OPTIONS_H
#define ERD_CLI_OPTIONS_H

#include "options_esdt.h"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    cxxopts::Options network() const;

    OptionsESDT esdt() const;

    std::string help() const;

private:
    void initOptions();

    void initOptionsTx();

    void initOptionsNetwork();

    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsNetwork;
    OptionsESDT m_optionsESDT;
};

#endif
