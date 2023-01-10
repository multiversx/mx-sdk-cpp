#ifndef ERD_CLI_CONFIG_H
#define ERD_CLI_CONFIG_H

#include <string>
#include <map>

#define NETWORK_MAINNET std::string("Mainnet")
#define NETWORK_DEVNET std::string("Devnet")
#define NETWORK_TESTNET std::string("Testnet")
#define NETWORK_LOCAL std::string("Local")

enum Network
{
    Mainnet,
    Devnet,
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
    explicit CLIConfig(std::string const &tomlConfigPath = "mx-sdk-erdcpp/cli/config/config.toml");

    Config config() const;

    void setNetwork(Network const &network) const;

    std::string toString(Network const& network) const;

private:
    std::string m_tomlPath;
    std::map<Network, std::string> m_networkMap;
};

#endif
