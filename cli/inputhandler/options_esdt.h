#ifndef CLI_OPTIONS_ESDT_H
#define CLI_OPTIONS_ESDT_H

#include "cliparser/cxxopts.hpp"

class OptionsESDT
{
public:
    explicit OptionsESDT();

    cxxopts::Options issue() const;

    cxxopts::Options transfer() const;

    std::string help() const;

private:
    void initOptions();

    void initOptionsIssue();

    void initOptionsTransfer();

    cxxopts::Options m_optionsIssue;
    cxxopts::Options m_optionsTransfer;
};

#endif
