#ifndef ERD_TRANSACTION_H
#define ERD_TRANSACTION_H

#include "json/json.hpp"

#include "account/address.h"
#include "transaction/signer.h"

#include <cstdint>
#include <string>

#define DEFAULT_NONCE 0U
#define DEFAULT_VALUE std::string()
#define DEFAULT_RECEIVER nullptr
#define DEFAULT_SENDER nullptr
#define DEFAULT_GAS_PRICE 0U
#define DEFAULT_GAS_LIMIT 0U
#define DEFAULT_DATA nullptr
#define DEFAULT_CHAIN_ID std::string()
#define DEFAULT_VERSION 0U
#define DEFAULT_SIGNATURE nullptr

class Transaction
{
public:
    explicit Transaction(
            uint64_t const &nonce,
            std::string value,
            Address const &receiver,
            Address const &sender,
            uint64_t const &gasPrice,
            uint64_t const &gasLimit,
            std::shared_ptr<std::string> data,
            std::string chainID,
            uint64_t const &version,
            std::shared_ptr<std::string> signature);

    explicit Transaction();

    void applySignature(Signer const &signer);

    std::string getSerialized() const;

    void deserialize(std::string const& serializedTransaction);

    uint64_t m_nonce;
    std::string m_value;
    std::shared_ptr<Address> m_receiver;
    std::shared_ptr<Address> m_sender;
    uint64_t m_gasPrice;
    uint64_t m_gasLimit;
    std::string m_chainID;
    uint64_t m_version;
    std::shared_ptr<std::string> m_data;
    std::shared_ptr<std::string> m_signature;
};


#endif //ERD_TRANSACTION_H
