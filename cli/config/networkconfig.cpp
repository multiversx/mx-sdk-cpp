#include "networkconfig.h"
#include "toml/cpptoml.h"
#include "utils/params.h"

#include <iostream>

#define BEGIN  "\x1B[33m"
#define END  "\x1B[0m"
#define YELLOW(x) BEGIN x END

NetworkConfig::NetworkConfig(std::string tomlConfigPath):
    m_tomlPath(tomlConfigPath) {}

Config NetworkConfig::config() const
{
    Config ret;

    try
    {
        auto const data = cpptoml::parse_file(m_tomlPath);
        auto const networkConfig = *data->get_qualified_as<std::string>("Config.NetworkConfig");
        auto const config = data->get_table(networkConfig);

        ret.chainID = *config->get_as<std::string>("ChainID");
        ret.proxyUrl = *config->get_as<std::string>("ProxyUrl");
        ret.issueESDTSCAddress = *config->get_as<std::string>("IssueESDTSCAddress");
    }
    catch (...)
    {
        std::cerr<< YELLOW("Warning: Could not load configuration, using default config.");

        std::cerr<< "\nChain ID: " << NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        std::cerr<< "\nProxy url: " << NETWORK_CONFIG_DEFAULT_PROXY_URL;
        std::cerr<< "\nESDT Issue SC Address: " << NETWORK_CONFIG_DEFAULT_ESDT_ISSUE_SC_ADDRESS << "\n";

        ret.chainID = NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        ret.proxyUrl = NETWORK_CONFIG_DEFAULT_PROXY_URL;
        ret.issueESDTSCAddress = NETWORK_CONFIG_DEFAULT_ESDT_ISSUE_SC_ADDRESS;
    }

    return ret;
}

