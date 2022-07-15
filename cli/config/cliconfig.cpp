#include "cliconfig.h"
#include "toml/cpptoml.h"
#include "utils/cfg.h"

#include <iostream>

#define BEGIN  "\x1B[33m"
#define END  "\x1B[0m"
#define YELLOW(x) BEGIN x END

CLIConfig::CLIConfig(std::string tomlConfigPath):
    m_tomlPath(std::move(tomlConfigPath)) {}

Config CLIConfig::config() const
{
    Config ret;

    try
    {
        auto const data = cpptoml::parse_file(m_tomlPath);
        auto const networkConfig = *data->get_qualified_as<std::string>("Config.CLIConfig");
        auto const config = data->get_table(networkConfig);

        ret.chainID = *config->get_as<std::string>("ChainID");
        ret.proxyUrl = *config->get_as<std::string>("ProxyUrl");
    }
    catch (...)
    {
        std::cerr<< YELLOW("Warning: Could not load configuration, using default config.");

        std::cerr<< "\nChain ID: " << NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        std::cerr<< "\nProxy url: " << NETWORK_CONFIG_DEFAULT_PROXY_URL;

        ret.chainID = NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        ret.proxyUrl = NETWORK_CONFIG_DEFAULT_PROXY_URL;
    }

    return ret;
}

