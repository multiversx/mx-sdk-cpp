#ifndef ERD_TRANSACTION_H
#define ERD_TRANSACTION_H

#include "json/json.hpp"

#include "account/address.h"
#include "transaction/signer.h"

#include <cstdint>
#include <string>

#define DEFAULT_DATA std::string()
#define DEFAULT_SIGNATURE std::string()

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

    const std::string &getValue() const;
    const Address &getReceiver() const;
    const Address &getSender() const;
    uint64_t getGasPrice() const;
    uint64_t getGasLimit() const;
    const std::string &getChainId() const;
    uint64_t getVersion() const;
    uint64_t getNonce() const;
    std::string getData() const;
    std::string getSignature() const;

    void setNonce(uint64_t nonce);
    void setReceiver(const Address &receiver);
    void setValue(const std::string &value);
    void setSender(const Address &sender);
    void setGasPrice(uint64_t gasPrice);
    void setGasLimit(uint64_t gasLimit);
    void setChainId(const std::string &chainId);
    void setVersion(uint64_t version);
    void setData(std::string const& data);
    void setSignature(std::string const& signature);

    void applySignature(Signer const &signer);

    std::string getSerialized() const;

    void deserialize(std::string const& serializedTransaction);

private:
    template<typename T>
    T fieldValue(std::pair<T,bool> const &field) const;

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
    std::string m_chainID;
    uint64_t m_version;
    std::pair<std::string,bool> m_data;
    std::pair<std::string,bool> m_signature;
};


#endif //ERD_TRANSACTION_H
