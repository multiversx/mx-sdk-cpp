#include "transaction/transaction.h"
#include "params.h"
#include "hex.h"
#include "base64.h"

Transaction::Transaction(
        uint64_t const &nonce,
        std::string const &value,
        Address const &receiver,
        Address const &sender,
        uint64_t const &gasPrice,
        uint64_t const &gasLimit,
        std::string const &data,
        std::string const &chainID,
        uint64_t const &version)
{
    m_jsonTransaction[STR_JSON_NONCE] = nonce;
    m_jsonTransaction[STR_JSON_VALUE] = value;
    m_jsonTransaction[STR_JSON_RECEIVER] = receiver.getBech32Address();
    m_jsonTransaction[STR_JSON_SENDER] = sender.getBech32Address();
    m_jsonTransaction[STR_JSON_GAS_PRICE] = gasPrice;
    m_jsonTransaction[STR_JSON_GAS_LIMIT] = gasLimit;
    if (!data.empty())
    {
        m_jsonTransaction[STR_JSON_DATA] = util::base64::encode(data);
    }
    m_jsonTransaction[STR_JSON_CHAIN_ID] = chainID;
    m_jsonTransaction[STR_JSON_VERSION] = version;
}

void Transaction::applySignature(Signer const &signer)
{
    std::string const jsonSerialized = getSerializedTransaction();
    std::string const signature = signer.getSignature(jsonSerialized);

    m_jsonTransaction[STR_JSON_SIGNATURE] = util::stringToHex(signature);
}

std::string Transaction::getSerializedTransaction() const
{
    return m_jsonTransaction.dump();
}
