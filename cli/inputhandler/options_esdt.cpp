#include "options_esdt.h"

OptionsESDT::OptionsESDT() :
        m_optionsIssue("mxcpp esdt issue", "Issue ESDT\n[command]: esdt\n[subcommand]: issue"),
        m_optionsTransfer("mxcpp esdt transfer", "Transfer ESDT\n[command]: esdt\n[subcommand]: transfer")
{
    initOptions();
}

cxxopts::Options OptionsESDT::issue() const
{
    return m_optionsIssue;
}

cxxopts::Options OptionsESDT::transfer() const
{
    return m_optionsTransfer;
}

std::string OptionsESDT::help() const
{
    return m_optionsTransfer.help() + "\n" +
           m_optionsIssue.help();
}

void OptionsESDT::initOptions()
{
    initOptionsIssue();
    initOptionsTransfer();
}

void OptionsESDT::initOptionsIssue()
{
    m_optionsIssue.add_options("issue") // esdt issue group
            ("token", "Token id", cxxopts::value<std::string>())
            ("ticker", "Token ticker", cxxopts::value<std::string>())
            ("supply", "Initial supply", cxxopts::value<std::string>())
            ("dec", "Number of decimals", cxxopts::value<uint32_t>())
            ("gas-price", "Gas price", cxxopts::value<uint64_t>())
            ("key", "Sender's private key (pem or keyfile)", cxxopts::value<std::string>())
            ("password", "Password for key file, not applicable for pem", cxxopts::value<std::string>()->default_value(""))
            ("nonce", "Sender account's nonce", cxxopts::value<uint64_t>())
            ("can-freeze", "[Can freeze] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-wipe", "[Can wipe] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-pause", "[Can pause] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-mint", "[Can mint] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-burn", "[Can burn] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-change-owner", "[Can change owner] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-upgrade", "[Can upgrade] property (default: false)", cxxopts::value<bool>()->default_value("false"))
            ("can-add-roles", "[Can add special roles] property (default: false)", cxxopts::value<bool>()->default_value("false"));
}

void OptionsESDT::initOptionsTransfer()
{
    m_optionsTransfer.add_options("transfer") // esdt transfer group
            ("nonce", "Sender account's nonce", cxxopts::value<uint64_t>())
            ("gas-price", "Gas price", cxxopts::value<uint64_t>())
            ("gas-limit", "Gas limit", cxxopts::value<uint64_t>())
            ("receiver", "Receiver's address", cxxopts::value<std::string>())
            ("token", "Token id", cxxopts::value<std::string>())
            ("value", "ESDT value to transfer", cxxopts::value<std::string>())
            ("function", "Smart contract function", cxxopts::value<std::string>()->default_value(""))
            ("args", "Smart contract function args, hex encoded, comma separated, e.g.: 0a,3f6f", cxxopts::value<std::vector<std::string>>()->default_value(""))
            ("key", "Sender's private key (pem or keyfile)", cxxopts::value<std::string>())
            ("password", "Password for key file, not applicable for pem", cxxopts::value<std::string>()->default_value(""));
}

