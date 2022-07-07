#include <utility>

#include "hex.h"
#include "transaction/esdt.h"
#include "transaction/transaction_factory.h"
#include "transaction/transaction_builders.h"
#include "transaction/transaction_builder_input.h"

namespace internal
{

std::string ESDTPropertyField(std::string const &property, bool const &val)
{
    std::string const value = val ? "true" : "false";
    return "@" + util::stringToHex(property) + "@" + util::stringToHex(value);
}

std::string ESDTPropertiesAsOnData(ESDTProperties const &esdtProperties)
{
    return ESDTPropertyField("canFreeze", esdtProperties.canFreeze) +
           ESDTPropertyField("canWipe", esdtProperties.canWipe) +
           ESDTPropertyField("canPause", esdtProperties.canPause) +
           ESDTPropertyField("canMint", esdtProperties.canMint) +
           ESDTPropertyField("canBurn", esdtProperties.canBurn) +
           ESDTPropertyField("canChangeOwner", esdtProperties.canChangeOwner) +
           ESDTPropertyField("canUpgrade", esdtProperties.canUpgrade) +
           ESDTPropertyField("canAddSpecialRoles", esdtProperties.canAddSpecialRoles);
}

}

TransactionFactory::TransactionFactory(const NetworkConfig &networkConfig) :
        m_gasEstimator(networkConfig), m_chainID(networkConfig.chainId)
{}

ITransactionBuilder &TransactionFactory::createEGLDTransfer(uint64_t nonce,
                                                            BigUInt value,
                                                            Address sender,
                                                            Address receiver,
                                                            uint64_t gasPrice,
                                                            std::string data)
{
    static TransactionEGLDTransferBuilder builder({nonce,
                                                   std::move(value),
                                                   std::move(sender),
                                                   std::move(receiver),
                                                   std::move(data),
                                                   gasPrice,
                                                   m_chainID,
                                                   m_gasEstimator});
    return builder;
}

ITransactionBuilder &TransactionFactory::createESDTIssue(uint64_t nonce,
                                                         Address sender,
                                                         uint64_t const &gasPrice,
                                                         std::string const &token,
                                                         std::string const &ticker,
                                                         BigUInt const &initialSupply,
                                                         uint32_t const &numOfDecimals,
                                                         ESDTProperties const &esdtProperties)
{
    SCArguments args;
    args.add(token);
    args.add(ticker);
    args.add(initialSupply);
    args.add(BigUInt(numOfDecimals));

    std::string data = ESDT_ISSUANCE_PREFIX + args.asOnData();
    data += (esdtProperties != ESDT_ISSUANCE_DEFAULT_PROPERTIES) ?
            data += internal::ESDTPropertiesAsOnData(esdtProperties) : "";

    static TransactionEGLDTransferBuilder builder({.nonce = nonce,
                                                          .value =  BigUInt(ESDT_ISSUANCE_VALUE),
                                                          .sender = std::move(sender),
                                                          .receiver = ESDT_ISSUANCE_ADDRESS_SYSTEM_SC,
                                                          .data =  std::move(data),
                                                          .gasPrice = gasPrice,
                                                          .chainID = m_chainID,
                                                          .gasEstimator =  m_gasEstimator,
                                                          .gasLimit = ESDT_ISSUANCE_GAS_LIMIT});
    return builder;
}

ITokenTransactionBuilder &TransactionFactory::createESDTTransfer(TokenPayment tokenPayment,
                                                                 uint64_t nonce,
                                                                 Address sender,
                                                                 Address receiver,
                                                                 uint64_t gasPrice)
{

    static TransactionESDTBuilder builder({nonce,
                                           BigUInt(0),
                                           std::move(sender),
                                           std::move(receiver),
                                           "",
                                           gasPrice,
                                           m_chainID,
                                           m_gasEstimator},
                                          std::move(tokenPayment));
    return builder;
}

ITokenTransactionBuilder &TransactionFactory::createESDTNFTTransfer(TokenPayment tokenPayment,
                                                                    uint64_t nonce,
                                                                    Address sender,
                                                                    Address destination,
                                                                    uint64_t gasPrice)
{
    static TransactionESDTNFTBuilder builder({nonce,
                                              BigUInt(0),
                                              std::move(sender),
                                              std::move(destination),
                                              "",
                                              gasPrice,
                                              m_chainID,
                                              m_gasEstimator},
                                             std::move(tokenPayment));

    return builder;
}

ITokenTransactionBuilder &TransactionFactory::createMultiESDTNFTTransfer(std::vector<TokenPayment> tokenPayments,
                                                                         uint64_t nonce,
                                                                         Address sender,
                                                                         Address destination,
                                                                         uint64_t gasPrice)
{
    static TransactionMultiESDTNFTBuilder builder({nonce,
                                                   BigUInt(0),
                                                   std::move(sender),
                                                   std::move(destination),
                                                   "",
                                                   gasPrice,
                                                   m_chainID,
                                                   m_gasEstimator},
                                                  std::move(tokenPayments));

    return builder;
}
