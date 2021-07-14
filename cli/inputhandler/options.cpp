#include "options.h"

CLIOptions::CLIOptions() :
        m_optionsTx("erdcpp transaction [subcommand]", "Manage transactions\n[command]: transaction\n[subcommands]: new"),
        m_optionsESDT()
{
    initOptions();
}

cxxopts::Options CLIOptions::transaction() const
{
    return m_optionsTx;
}

OptionsESDT CLIOptions::esdt() const
{
    return m_optionsESDT;
}

std::string CLIOptions::help() const
{
    return transaction().help() + "\n" +
           esdt().help();
}

void CLIOptions::initOptions()
{
    initOptionsTx();
}

void CLIOptions::initOptionsTx()
{
    m_optionsTx.add_options("new") // transaction new group
            ("nonce", "Transaction nonce", cxxopts::value<uint64_t>())
            ("value", "Transaction value", cxxopts::value<std::string>())
            ("receiver", "Receiver's address", cxxopts::value<std::string>())
            ("receiver-name", "Receiver's username", cxxopts::value<std::string>()->default_value(""))
            ("sender-name", "Sender's username", cxxopts::value<std::string>()->default_value(""))
            ("gas-price", "Transaction gas price", cxxopts::value<uint64_t>())
            ("gas-limit", "Transaction gas limit", cxxopts::value<uint64_t>())
            ("data", "Transaction data/payload", cxxopts::value<std::string>()->default_value(""))
            ("chainID", "Chain identifier", cxxopts::value<std::string>()->default_value("T"))
            ("version", "Transaction version", cxxopts::value<uint64_t>()->default_value("1"))
            ("options", "Transaction options", cxxopts::value<uint32_t>()->default_value("0"))
            ("pem", "Pem keyfile", cxxopts::value<std::string>())
            ("outfile", "Json file where the output will be stored", cxxopts::value<std::string>());
}

