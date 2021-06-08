#ifndef ERD_TRANSACTION_H
#define ERD_TRANSACTION_H

#include "json/json.hpp"

#include "account/address.h"
#include "transaction/signer.h"

#include <cstdint>
#include <string>

#define DEFAULT_DATA std::string()
#define DEFAULT_SIGNATURE std::string()

#define ON true
#define OFF false

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
            std::string const &chainID,
            uint64_t const &version,
            std::string data = DEFAULT_DATA,
            std::string signature = DEFAULT_SIGNATURE);

    void applySignature(Signer const &signer);

    std::string getSerialized() const;

private:

    template<typename T>
    bool emptyField(std::pair<T,bool> const &field) const;

    template<typename T>
    void setFieldIfNotDefault(std::pair<T,bool> &field, T const &value, T const &defaultValue);

    template<typename T>
    void setField(std::pair<T,bool> &field, T const &value, bool state);

    uint64_t m_nonce;
    std::string m_value;
    Address m_receiver;
    Address m_sender;
    uint64_t m_gasPrice;
    uint64_t m_gasLimit;
    std::pair<std::string,bool> m_data;
    std::string m_chainID;
    uint64_t m_version;
    std::pair<std::string,bool> m_signature;
};


#endif //ERD_TRANSACTION_H
