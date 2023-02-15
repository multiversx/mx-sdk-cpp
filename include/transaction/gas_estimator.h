#ifndef MX_GAS_ESTIMATOR_H
#define MX_GAS_ESTIMATOR_H

#include "provider/data/networkconfig.h"

class GasEstimator
{
public:
    explicit GasEstimator(NetworkConfig networkConfig);

    uint64_t forEGLDTransfer(uint64_t dataLength) const;

    uint64_t forESDTTransfer(uint64_t dataLength) const;

    uint64_t forESDTNFTTransfer(uint64_t dataLength) const;

    uint64_t forMultiESDTNFTTransfer(uint64_t dataLength, uint64_t numTransfers) const;

private:
    NetworkConfig m_networkConfig;
};

#endif //MX_GAS_ESTIMATOR_H
