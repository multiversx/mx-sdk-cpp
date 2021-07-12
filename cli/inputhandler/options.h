#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include "options_esdt.h"

class CLIOptions
{
public:
    explicit CLIOptions();

    cxxopts::Options transaction() const;

    cxxopts::Options pem() const;

    OptionsESDT esdt() const;

    std::string help() const;

private:
    void initOptions();

    void initOptionsTx();

    void initOptionsPem();

    cxxopts::Options m_optionsTx;
    cxxopts::Options m_optionsPem;
    OptionsESDT m_optionsESDT;
};

#endif
