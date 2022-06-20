#include "provider/data/networkconfig.h"

bool operator==(NetworkConfig const &lhs, NetworkConfig const &rhs)
{
    return (lhs.chainId == rhs.chainId) &&
           (lhs.gasPerDataByte == rhs.gasPerDataByte) &&
           (lhs.minGasLimit == rhs.minGasLimit) &&
           (lhs.minGasPrice == rhs.minGasPrice);
}

bool operator!=(NetworkConfig const &lhs, NetworkConfig const &rhs)
{
    return !(lhs == rhs);
}