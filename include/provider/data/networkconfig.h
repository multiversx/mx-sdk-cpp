#ifndef ERD_NETWORKCONFIG_H
#define ERD_NETWORKCONFIG_H

#include <string>

/// A "NetworkConfig" object holds the network parameters relevant to creating transactions (e.g. minimum gas limit, minimum gas price).
/// The following fields can (should) be fetched from https://api.elrond.com/network/config.
struct NetworkConfig
{
    std::string chainId;
    uint32_t gasPerDataByte;
    uint32_t minGasLimit;
    uint64_t minGasPrice;
};

bool operator==(NetworkConfig const &lhs, NetworkConfig const &rhs);

bool operator!=(NetworkConfig const &lhs, NetworkConfig const &rhs);

#define DEFAULT_MAINNET_NETWORK_CONFIG NetworkConfig { \
.chainId = "1",                                        \
.gasPerDataByte = 1500,                                \
.minGasLimit = 50000,                                  \
.minGasPrice = 1000000000                              \
};

#define DEFAULT_TESTNET_NETWORK_CONFIG NetworkConfig { \
.chainId = "T",                                        \
.gasPerDataByte = 1500,                                \
.minGasLimit = 50000,                                  \
.minGasPrice = 1000000000                              \
};

#endif //ERD_NETWORKCONFIG_H
