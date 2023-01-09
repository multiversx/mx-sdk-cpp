#include <utility>

// This file is an adaptation from https://github.com/multiversx/mx-sdk-erdjs/blob/main/src/gasEstimator.ts
#include "transaction/gas_estimator.h"

// Additional gas to account for eventual increases in gas requirements (thus avoid fast-breaking changes in clients of erd-cpp).
#define ADDITIONAL_GAS_FOR_ESDT_TRANSFER  100000

// Additional gas to account for extra blockchain operations (e.g. data movement (between accounts) for NFTs),
// and for eventual increases in gas requirements (thus avoid fast-breaking changes in clients of erd-cpp).
#define ADDITIONAL_GAS_FOR_ESDT_NFT_TRANSFER  800000

#define GAS_COST_ESDT_TRANSFER 200000
#define GAS_COST_ESDT_NFT_TRANSFER 200000
#define GAS_COST_ESDT_NFT_MULTI_TRANSFER 200000

GasEstimator::GasEstimator(NetworkConfig networkConfig) : m_networkConfig(std::move(networkConfig))
{}

uint64_t GasEstimator::forEGLDTransfer(uint64_t dataLength) const
{
    return m_networkConfig.minGasLimit +
           m_networkConfig.gasPerDataByte * dataLength;
}

uint64_t GasEstimator::forESDTTransfer(uint64_t dataLength) const
{
    return m_networkConfig.minGasLimit +
           GAS_COST_ESDT_TRANSFER +
           m_networkConfig.gasPerDataByte * dataLength +
           ADDITIONAL_GAS_FOR_ESDT_TRANSFER;
}

uint64_t GasEstimator::forESDTNFTTransfer(uint64_t dataLength) const
{
    return m_networkConfig.minGasLimit +
           GAS_COST_ESDT_NFT_TRANSFER +
           m_networkConfig.gasPerDataByte * dataLength +
           ADDITIONAL_GAS_FOR_ESDT_NFT_TRANSFER;
}

uint64_t GasEstimator::forMultiESDTNFTTransfer(uint64_t dataLength, uint64_t numTransfers) const
{
    return m_networkConfig.minGasLimit +
           (GAS_COST_ESDT_NFT_MULTI_TRANSFER + ADDITIONAL_GAS_FOR_ESDT_NFT_TRANSFER) * numTransfers +
           m_networkConfig.gasPerDataByte * dataLength;
}
