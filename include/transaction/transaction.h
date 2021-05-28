#ifndef ERDCPP_TRANSACTIONHANDLER_H
#define ERDCPP_TRANSACTIONHANDLER_H

#include "json/json.hpp"

#include "account/address.h"
#include "transaction/signer.h"

#include <stdint.h>
#include <string>


class Transaction
{
public:
  Transaction(
    uint64_t nonce,
    std::string value,
    Address receiver,
    Address sender,
    uint64_t gasPrice,
    uint64_t gasLimit,
    std::string data,
    std::string chainID,
    uint64_t version);

  void applySignature(Signer signer);

  std::string getSerializedTransaction() const;

private:

  nlohmann::ordered_json m_jsonTransaction;
};


#endif //ERDCPP_TRANSACTIONHANDLER_H
