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
        std::shared_ptr<std::string> data,
        std::string const &chainID,
        uint64_t const &version,
        std::shared_ptr<std::string> signature):
        m_nonce(nonce), m_value(value), m_receiver(receiver),
        m_sender(sender), m_gasPrice(gasPrice), m_gasLimit(gasLimit),
        m_chainID(chainID),m_version(version)
{
    m_data.reset(new std::string(*data));
    m_signature.reset(new std::string(*data));
}

void Transaction::applySignature(Signer const &signer)
{
    if(m_signature != nullptr)
        m_signature = nullptr;

    std::string const jsonSerialized = getSerialized();
    std::string signature = util::stringToHex(signer.getSignature(jsonSerialized));

    m_signature.reset(new std::string (signature));
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
    if (m_data != nullptr)
    {
        json.set(STR_JSON_DATA, util::base64::encode(*m_data));
    }
    json.set(STR_JSON_CHAIN_ID, m_chainID);
    json.set(STR_JSON_VERSION, m_version);
    if (m_signature != nullptr)
    {
        json.set(STR_JSON_SIGNATURE,*m_signature);
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

    m_nonce = json.at<uint64_t>(STR_JSON_NONCE);
    m_value = json.at<std::string>(STR_JSON_VALUE);
    m_receiver = Address(json.at<std::string>(STR_JSON_RECEIVER));
    m_sender = Address(json.at<std::string>(STR_JSON_SENDER));
    m_gasPrice = json.at<uint64_t>(STR_JSON_GAS_PRICE);
    m_gasLimit = json.at<uint64_t>(STR_JSON_GAS_LIMIT);
    m_chainID = json.at<std::string>(STR_JSON_CHAIN_ID);
    m_version = json.at<uint64_t>(STR_JSON_VERSION);

    if (json.contains(STR_JSON_DATA))
    {
        m_data = std::make_unique<std::string>(json.at<std::string>(STR_JSON_DATA));
    }
    if (json.contains(STR_JSON_SIGNATURE))
    {
        m_signature = std::make_unique<std::string>(json.at<std::string>(STR_JSON_SIGNATURE));
    }
}
