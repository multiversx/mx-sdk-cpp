#include "transaction/transaction.h"

#include <utility>
#include "jsonwrapper.h"
#include "params.h"
#include "hex.h"
#include "base64.h"
#include "errors.h"

namespace internal
{
    template <typename T>
    void setValueIfNotNull(wrapper::json::OrderedJson &json, std::string const& key, std::shared_ptr<T> const &val)
    {
        if (val != nullptr)
            json.set(key, *val);
    }
}

Transaction::Transaction(
        uint64_t const &nonce,
        std::string value,
        Address const &receiver,
        Address const &sender,
        std::shared_ptr<bytes> receiverUserName,
        std::shared_ptr<bytes> senderUserName,
        uint64_t const &gasPrice,
        uint64_t const &gasLimit,
        std::shared_ptr<bytes> data,
        std::shared_ptr<std::string> signature,
        std::string chainID,
        uint64_t const &version,
        std::shared_ptr<uint32_t> options) :
        m_nonce(nonce),
        m_value(std::move(value)),
        m_receiver(new Address(receiver)),
        m_sender(new Address(sender)),
        m_receiverUserName(std::move(receiverUserName)),
        m_senderUserName(std::move(senderUserName)),
        m_gasPrice(gasPrice),
        m_gasLimit(gasLimit),
        m_data(std::move(data)),
        m_signature(std::move(signature)),
        m_chainID(std::move(chainID)),
        m_version(version),
        m_options(std::move(options))
{}

Transaction::Transaction() :
        m_nonce(DEFAULT_NONCE),
        m_value(DEFAULT_VALUE),
        m_receiver(DEFAULT_RECEIVER),
        m_sender(DEFAULT_SENDER),
        m_receiverUserName(DEFAULT_RECEIVER_NAME),
        m_senderUserName(DEFAULT_SENDER_NAME),
        m_gasPrice(DEFAULT_GAS_PRICE),
        m_gasLimit(DEFAULT_GAS_LIMIT),
        m_data(DEFAULT_DATA),
        m_signature(DEFAULT_SIGNATURE),
        m_chainID(DEFAULT_CHAIN_ID),
        m_version(DEFAULT_VERSION),
        m_options(DEFAULT_OPTIONS)
{}

void Transaction::applySignature(Signer const &signer)
{
    if (m_signature != nullptr)
        m_signature = nullptr;

    std::string const jsonSerialized = serialize();
    std::string signature = util::stringToHex(signer.getSignature(jsonSerialized));

    m_signature.reset(new std::string(signature));
}

std::string Transaction::serialize() const
{
    if (m_receiver == nullptr) throw std::invalid_argument(ERROR_MSG_RECEIVER);
    if (m_sender == nullptr) throw std::invalid_argument(ERROR_MSG_SENDER);

    wrapper::json::OrderedJson json;

    json.set(TX_NONCE, m_nonce);
    json.set(TX_VALUE, m_value);
    json.set(TX_RECEIVER, (*m_receiver).getBech32Address());
    json.set(TX_SENDER, (*m_sender).getBech32Address());
    internal::setValueIfNotNull(json, TX_RECEIVER_NAME, m_receiverUserName);
    internal::setValueIfNotNull(json, TX_SENDER_NAME, m_senderUserName);
    json.set(TX_GAS_PRICE, m_gasPrice);
    json.set(TX_GAS_LIMIT, m_gasLimit);
    internal::setValueIfNotNull(json, TX_DATA, m_data);
    internal::setValueIfNotNull(json, TX_SIGNATURE, m_signature);
    json.set(TX_CHAIN_ID, m_chainID);
    json.set(TX_VERSION, m_version);
    internal::setValueIfNotNull(json, TX_OPTIONS, m_options);

    return json.serialize();
}

void Transaction::deserialize(std::string const &serializedTransaction)
{
    wrapper::json::OrderedJson json;

    try
    {
        json.deserialize(serializedTransaction);
    }
    catch (std::invalid_argument const &err)
    {
        throw err;
    }

    if (!json.contains(TX_NONCE)) throw std::invalid_argument(ERROR_MSG_NONCE);
    if (!json.contains(TX_VALUE)) throw std::invalid_argument(ERROR_MSG_VALUE);
    if (!json.contains(TX_RECEIVER)) throw std::invalid_argument(ERROR_MSG_RECEIVER);
    if (!json.contains(TX_SENDER)) throw std::invalid_argument(ERROR_MSG_SENDER);
    if (!json.contains(TX_GAS_PRICE)) throw std::invalid_argument(ERROR_MSG_GAS_PRICE);
    if (!json.contains(TX_GAS_LIMIT)) throw std::invalid_argument(ERROR_MSG_GAS_LIMIT);
    if (!json.contains(TX_CHAIN_ID)) throw std::invalid_argument(ERROR_MSG_CHAIN_ID);
    if (!json.contains(TX_VERSION)) throw std::invalid_argument(ERROR_MSG_VERSION);

    m_nonce = json.at<uint64_t>(TX_NONCE);
    m_value = json.at<std::string>(TX_VALUE);
    m_receiver.reset(new Address(json.at<std::string>(TX_RECEIVER)));
    m_sender.reset(new Address(json.at<std::string>(TX_SENDER)));
    m_gasPrice = json.at<uint64_t>(TX_GAS_PRICE);
    m_gasLimit = json.at<uint64_t>(TX_GAS_LIMIT);
    m_chainID = json.at<std::string>(TX_CHAIN_ID);
    m_version = json.at<uint64_t>(TX_VERSION);

    if (json.contains(TX_DATA))
    {
        m_data = std::make_shared<bytes>(json.at<bytes>(TX_DATA));
    }
    if (json.contains(TX_SIGNATURE))
    {
        m_signature = std::make_shared<std::string>(json.at<std::string>(TX_SIGNATURE));
    }
    if (json.contains(TX_RECEIVER_NAME))
    {
        m_receiverUserName = std::make_shared<bytes>(json.at<bytes>(TX_RECEIVER_NAME));
    }
    if (json.contains(TX_SENDER_NAME))
    {
        m_senderUserName = std::make_shared<bytes>(json.at<bytes>(TX_SENDER_NAME));
    }
    if (json.contains(TX_OPTIONS))
    {
        m_options = std::make_shared<uint32_t>(json.at<uint32_t>(TX_OPTIONS));
    }
}
