#include "transaction/transaction.h"
#include "jsonwrapper.h"
#include "params.h"
#include "hex.h"
#include "base64.h"
#include "errors.h"

//TODO: Allow creating transactions without nonce/gasPrice
Transaction::Transaction(
        uint64_t const &nonce,
        std::string const &value,
        Address const &receiver,
        Address const &sender,
        uint64_t const &gasPrice,
        uint64_t const &gasLimit,
        std::string const &chainID,
        uint64_t const &version,
        std::string data,
        std::string signature):
        m_nonce(nonce), m_value(value), m_receiver(receiver),
        m_sender(sender), m_gasPrice(gasPrice), m_gasLimit(gasLimit),
        m_chainID(chainID), m_version(version)
{
    setFieldIfNotDefault(m_data, data, DEFAULT_DATA);
    setFieldIfNotDefault(m_signature,signature,DEFAULT_SIGNATURE);
}

void Transaction::applySignature(Signer const &signer)
{
    if(!emptyField(m_signature))
        throw std::invalid_argument(ERROR_MSG_SIGNATURE_ALREADY_DEFINED);

    std::string const jsonSerialized = getSerialized();
    std::string const signature = util::stringToHex(signer.getSignature(jsonSerialized));

    setField(m_signature, signature, ON);
}

std::string Transaction::getSerialized() const
{
    wrapper::json::JsonOrdered json;

    json.set(STR_JSON_NONCE, m_nonce);
    json.set(STR_JSON_VALUE, m_value);
    json.set(STR_JSON_RECEIVER, m_receiver.getBech32Address());
    json.set(STR_JSON_SENDER, m_sender.getBech32Address());
    json.set(STR_JSON_GAS_PRICE, m_gasPrice);
    json.set(STR_JSON_GAS_LIMIT, m_gasLimit);
    if (!emptyField(m_data))
    {
        json.set(STR_JSON_DATA, util::base64::encode(m_data.first));
    }
    json.set(STR_JSON_CHAIN_ID, m_chainID);
    json.set(STR_JSON_VERSION, m_version);
    if (!emptyField(m_signature))
    {
        json.set(STR_JSON_SIGNATURE,m_signature.first);
    }

    return json.getSerialized();
}

template<typename T>
void Transaction::setFieldIfNotDefault(std::pair<T,bool> &field, T const &value, T const &defaultValue)
{
    if(value != defaultValue)
    {
        setField(field, value, ON);
    }
    else
    {
        field.second = OFF;
    }

}

template<typename T>
void Transaction::setField(std::pair<T,bool> &field, T const &value, bool state)
{
    field.first = value;
    field.second = state;
}

template<typename T>
bool Transaction::emptyField(std::pair<T,bool> const &field) const
{
    return (field.second == OFF);
}