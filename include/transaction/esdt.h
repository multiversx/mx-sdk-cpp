#ifndef MX_ESDT_H
#define MX_ESDT_H

#include "transaction.h"
#include "smartcontracts/sc_arguments.h"

#define ESDT_TRANSFER_PREFIX std::string("ESDTTransfer")
#define ESDT_NFT_TRANSFER_PREFIX std::string("ESDTNFTTransfer")
#define MULTI_ESDT_NFT_TRANSFER_PREFIX std::string("MultiESDTNFTTransfer")

#define ESDT_ISSUANCE_PREFIX std::string("issue")
#define ESDT_ISSUANCE_VALUE "50000000000000000" //(0.05 EGLD)
#define ESDT_ISSUANCE_ADDRESS Address("erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u")
#define ESDT_ISSUANCE_GAS_LIMIT 60000000
#define ESDT_ISSUANCE_DEFAULT_PROPERTIES ESDTProperties{}

struct ESDTProperties
{
    bool canFreeze = false;
    bool canWipe = false;
    bool canPause = false;
    bool canMint = false;
    bool canBurn = false;
    bool canChangeOwner = false;
    bool canUpgrade = false;
    bool canAddSpecialRoles = false;
    bool canTransferNFTCreateRole = false;
};

bool operator==(ESDTProperties const &lhs, ESDTProperties const &rhs);

bool operator!=(ESDTProperties const &lhs, ESDTProperties const &rhs);

#endif //MX_ESDT_H
