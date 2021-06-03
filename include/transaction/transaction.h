#ifndef ERD_TRANSACTION_H
#define ERD_TRANSACTION_H

#include "json/json.hpp"

#include "account/address.h"
#include "transaction/signer.h"

#include <cstdint>
#include <string>

class Transaction
{
public:
    Transaction(
            uint64_t const &nonce,
            std::string const &value,
            Address const &receiver,
            Address const &sender,
            uint64_t const &gasPrice,
            uint64_t const &gasLimit,
            std::string const &data,
            std::string const &chainID,
            uint64_t const &version);

    void applySignature(Signer const &signer);

    std::string getSerializedTransaction() const;

private:
    // TODO: Create wrapper for json
    nlohmann::ordered_json m_jsonTransaction;
};


#endif //ERD_TRANSACTION_H
