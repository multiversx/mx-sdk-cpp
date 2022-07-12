#ifndef ERD_CLI_CONFIG_H
#define ERD_CLI_CONFIG_H

#include <string>

enum Network
{
    Devnet,
    Mainnet,
    Testnet,
    Local,
};

struct Config
{
    std::string chainID;
    std::string proxyUrl;
};

class CLIConfig
{
public:
    explicit CLIConfig(std::string const &tomlConfigPath = "elrond-sdk-erdcpp/cli/config/config.toml");

    Config config() const;

    void setNetwork(Network const &network) const;

private:
    std::string m_tomlPath;
};

#endif
