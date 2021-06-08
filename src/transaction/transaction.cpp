#include "transaction/transaction.h"
#include "jsonwrapper.h"
#include "params.h"
#include "hex.h"
#include "base64.h"
#include "errors.h"

#define ON true
#define OFF false

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
        setField(m_signature, DEFAULT_SIGNATURE, OFF);

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

void Transaction::deserialize(std::string const& serializedTransaction)
{
    wrapper::json::JsonOrdered json;

    json.deserialize(serializedTransaction);

    if (!json.contains(STR_JSON_NONCE)) throw std::invalid_argument(ERROR_MSG_NONCE);
    if (!json.contains(STR_JSON_VALUE)) throw std::invalid_argument(ERROR_MSG_VALUE);
    if (!json.contains(STR_JSON_RECEIVER)) throw std::invalid_argument(ERROR_MSG_RECEIVER);
    if (!json.contains(STR_JSON_SENDER)) throw std::invalid_argument(ERROR_MSG_SENDER);
    if (!json.contains(STR_JSON_GAS_PRICE)) throw std::invalid_argument(ERROR_MSG_GAS_PRICE);
    if (!json.contains(STR_JSON_GAS_LIMIT)) throw std::invalid_argument(ERROR_MSG_GAS_LIMIT);
    if (!json.contains(STR_JSON_CHAIN_ID)) throw std::invalid_argument(ERROR_MSG_CHAIN_ID);
    if (!json.contains(STR_JSON_VERSION)) throw std::invalid_argument(ERROR_MSG_VERSION);

    setNonce(json.at<uint64_t>(STR_JSON_NONCE));
    setValue(json.at<std::string>(STR_JSON_VALUE));
    setReceiver(Address(json.at<std::string>(STR_JSON_RECEIVER)));
    setSender(Address(json.at<std::string>(STR_JSON_SENDER)));
    setGasPrice(json.at<uint64_t>(STR_JSON_GAS_PRICE));
    setGasLimit(json.at<uint64_t>(STR_JSON_GAS_LIMIT));
    setChainId(json.at<std::string>(STR_JSON_CHAIN_ID));
    setVersion(json.at<uint64_t>(STR_JSON_VERSION));

    if (json.contains(STR_JSON_DATA))
    {
        setData(json.at<std::string>(STR_JSON_DATA));
    }
    if (json.contains(STR_JSON_SIGNATURE))
    {
        setSignature(json.at<std::string>(STR_JSON_SIGNATURE));
    }
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

template<typename T>
T Transaction::fieldValue(std::pair<T,bool> const &field) const
{
    return field.first;
}

const std::string &Transaction::getValue() const
{
    return m_value;
}

void Transaction::setValue(const std::string &value)
{
    m_value = value;
}

const Address &Transaction::getReceiver() const
{
    return m_receiver;
}

void Transaction::setReceiver(const Address &receiver)
{
    m_receiver = receiver;
}

const Address &Transaction::getSender() const
{
    return m_sender;
}

void Transaction::setSender(const Address &sender)
{
    m_sender = sender;
}

uint64_t Transaction::getGasPrice() const
{
    return m_gasPrice;
}

void Transaction::setGasPrice(uint64_t gasPrice)
{
    m_gasPrice = gasPrice;
}

uint64_t Transaction::getGasLimit() const
{
    return m_gasLimit;
}

void Transaction::setGasLimit(uint64_t gasLimit)
{
    m_gasLimit = gasLimit;
}

const std::string &Transaction::getChainId() const
{
    return m_chainID;
}

void Transaction::setChainId(const std::string &chainId)
{
    m_chainID = chainId;
}

uint64_t Transaction::getVersion() const
{
    return m_version;
}

void Transaction::setVersion(uint64_t version)
{
    m_version = version;
}

uint64_t Transaction::getNonce() const
{
    return m_nonce;
}

void Transaction::setNonce(uint64_t nonce)
{
    m_nonce = nonce;
}

void Transaction::setData(std::string const& data)
{
    setField(m_data,data,ON);
}
void Transaction::setSignature(std::string const& signature)
{
    setField(m_signature,signature,ON);
}

std::string Transaction::getData() const
{
    return (emptyField(m_data)) ? (DEFAULT_DATA) : (fieldValue(m_data));
}
std::string Transaction::getSignature() const
{
    return (emptyField(m_signature)) ? (DEFAULT_SIGNATURE) : (fieldValue(m_signature));
}

