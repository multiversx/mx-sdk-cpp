#ifndef ERD_CLI_NETWORK_CONFIG_H
#define ERD_CLI_NETWORK_CONFIG_H

#include <string>

struct Config
{
    std::string chainID;
    std::string proxyUrl;
    std::string issueESDTSCAddress;
};

class NetworkConfig
{
public:
    explicit NetworkConfig(std::string tomlConfigPath = "config/config.toml");

    Config config() const;

private:
    std::string m_tomlPath;
};

#endif
