#include "transaction.h"
#include "../Utils/params.h"
#include "../Utils/hex.h"
#include "../Utils/base64.h"
#include <sodium.h>

Transaction::Transaction(
    uint64_t nonce,
    std::string value,
    Address receiver,
    Address sender,
    uint64_t gasPrice,
    uint64_t gasLimit,
    std::string data,
    std::string chainID,
    uint64_t version)
{
  m_jsonTransaction[STR_JSON_NONCE] = nonce;
  m_jsonTransaction[STR_JSON_VALUE] = value;
  m_jsonTransaction[STR_JSON_RECEIVER] = receiver.getBech32Address();
  m_jsonTransaction[STR_JSON_SENDER] = sender.getBech32Address();
  m_jsonTransaction[STR_JSON_GAS_PRICE] = gasPrice;
  m_jsonTransaction[STR_JSON_GAS_LIMIT] = gasLimit;
  if (!data.empty())
  {
    m_jsonTransaction[STR_JSON_DATA] = util::base64_encode(data);
  }
  m_jsonTransaction[STR_JSON_CHAIN_ID] = chainID;
  m_jsonTransaction[STR_JSON_VERSION] = version;
}

void Transaction::applySignature(Signer signer)
{
  std::string const jsonSerialized = getSerializedTransaction();
  std::string const signature = signer.getSignature(jsonSerialized);

  m_jsonTransaction[STR_JSON_SIGNATURE] = util::stringToHex(signature);
}

std::string Transaction::getSerializedTransaction() const
{
  return m_jsonTransaction.dump();
}
