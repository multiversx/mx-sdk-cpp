#include "options.h"

CLIOptions::CLIOptions() :
        m_optionsTx("erdcpp transaction new", "Create signed transactions\n[command]: transaction\n[subcommand]: new"),
        m_optionsNetwork("erdcpp network set", "Set network\n[command]: network\n[subcommand]: set"),
        m_optionsESDT()
{
    initOptions();
}

cxxopts::Options CLIOptions::transaction() const
{
    return m_optionsTx;
}


cxxopts::Options CLIOptions::network() const
{
    return m_optionsNetwork;
}

OptionsESDT CLIOptions::esdt() const
{
    return m_optionsESDT;
}

std::string CLIOptions::help() const
{
    return transaction().help() + "\n" +
           esdt().help() + "\n" +
           m_optionsNetwork.help();
}

void CLIOptions::initOptions()
{
    initOptionsTx();
    initOptionsNetwork();
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
            ("version", "Transaction version", cxxopts::value<uint64_t>()->default_value("1"))
            ("options", "Transaction options", cxxopts::value<uint32_t>()->default_value("0"))
            ("key", "File containing the private key (pem or keyfile)", cxxopts::value<std::string>())
            ("password", "Password for key file, not applicable for pem", cxxopts::value<std::string>()->default_value(""))
            ("outfile", "Json file where the output will be stored", cxxopts::value<std::string>());
}

void CLIOptions::initOptionsNetwork()
{
    m_optionsNetwork.add_options("set") // network config set
            ("config", "Set network config used to interact with ERDCPP CLI. Valid: mainnet, testnet, devnet, local (not case sensitive)", cxxopts::value<std::string>());
}
