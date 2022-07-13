#include "cliconfig.h"
#include "toml/cpptoml.h"
#include "utils/cfg.h"

#include <iostream>

#define BEGIN  "\x1B[33m"
#define END  "\x1B[0m"
#define YELLOW(x) BEGIN x END

namespace
{
inline std::string fileDoesNotContain(std::string const& filePath)
{
    return "File " + filePath + " does not contain ";
}
}

std::string getCanonicRootPath(std::string const &path)
{
    // Get absolute path to executable
    std::string canonicPath = std::string(canonicalize_file_name("/proc/self/exe"));

    // Remove everything in path until elrond-sdk-erdcpp directory and concatenate it with the path
    // Use rfind because github action runs into elrond-sdk-erdcpp/elrond-sdk-erdcpp folder
    auto const pos = canonicPath.rfind("elrond-sdk-erdcpp");
    canonicPath = canonicPath.substr(0, pos);
    return canonicPath + path;
}

CLIConfig::CLIConfig(std::string const &tomlConfigPath) :
        m_tomlPath(getCanonicRootPath(tomlConfigPath)),
        m_networkMap({{Mainnet, NETWORK_MAINNET},
                      {Devnet,  NETWORK_DEVNET},
                      {Testnet, NETWORK_TESTNET},
                      {Local,   NETWORK_LOCAL}})
{}

Config CLIConfig::config() const
{
    Config ret;

    try
    {
        auto const data = cpptoml::parse_file(m_tomlPath);
        auto const networkConfig = *data->get_qualified_as<std::string>("Config.CLIConfig");
        if (networkConfig.empty())
        {
            throw std::runtime_error(fileDoesNotContain(m_tomlPath) + "[Config].CLIConfig table");
        }

        auto const config = data->get_table(networkConfig);
        if (!config->contains("ChainID") || !config->contains("ProxyUrl"))
        {
            throw std::runtime_error(fileDoesNotContain(m_tomlPath) + "ChainID/ProxyUrl field in [" + networkConfig +"] table");
        }

        ret.chainID = *config->get_as<std::string>("ChainID");
        ret.proxyUrl = *config->get_as<std::string>("ProxyUrl");
    }
    catch (std::exception const &exception)
    {
        std::cerr << YELLOW("Warning: Could not load configuration, got exception: <" + std::string(exception.what()) + ">, using default config.");
        std::cerr << "\nChain ID: " << NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        std::cerr << "\nProxy url: " << NETWORK_CONFIG_DEFAULT_PROXY_URL;

        ret.chainID = NETWORK_CONFIG_DEFAULT_CHAIN_ID;
        ret.proxyUrl = NETWORK_CONFIG_DEFAULT_PROXY_URL;
    }

    return ret;
}

void CLIConfig::setNetwork(Network const &network) const
{
    std::ifstream inFile(m_tomlPath);
    std::stringstream strStream;

    strStream << inFile.rdbuf();
    std::string currLine;
    bool found = false;
    while (std::getline(strStream, currLine))
    {
        if (currLine.find("CLIConfig") != std::string::npos)
        {
            std::string newLine = std::string("CLIConfig = ") + "\"" + m_networkMap.at(network) + "\"";
            std::string fileContent = strStream.str();
            fileContent.replace(fileContent.find(currLine), currLine.length(), newLine);

            // Rewrite file with new content
            std::ofstream ofs;
            ofs.open(m_tomlPath, std::ofstream::out | std::ofstream::trunc);
            ofs << fileContent;
            ofs.close();

            found = true;
            break;
        }
    }

    if (!found)
    {
        throw std::runtime_error("Could not find CLIConfig table in " + m_tomlPath);
    }
}

std::string CLIConfig::toString(const Network &network) const
{
    return m_networkMap.at(network);
}

