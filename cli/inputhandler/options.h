#ifndef ERD_CLI_OPTIONS_H
#define ERD_CLI_OPTIONS_H

#include "options_esdt.h"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    OptionsESDT esdt() const;

    std::string help() const;

private:
    void initOptions();

    void initOptionsTx();

    cxxopts::Options m_optionsTx;
    OptionsESDT m_optionsESDT;
};

#endif
