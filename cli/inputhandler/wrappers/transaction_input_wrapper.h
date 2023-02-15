#ifndef MX_CLI_TRANSACTION_INPUT_WRAPPER_H
#define MX_CLI_TRANSACTION_INPUT_WRAPPER_H

#include "iwrapper.h"
#include "account/address.h"
#include "internal/biguint.h"

namespace ih
{
namespace wrapper
{
class TransactionInputWrapper : public IWrapper
{
public:
    explicit TransactionInputWrapper(cxxopts::ParseResult const  &inputData);

    uint64_t getNonce() const;

    BigUInt getValue() const;

    std::shared_ptr<Address> getReceiver() const;

    std::shared_ptr<bytes> getReceiverName() const;

    std::shared_ptr<bytes> getSenderName() const;

    uint64_t getGasPrice() const;

    uint64_t getGasLimit() const;

    std::shared_ptr<bytes> getData() const;

    uint64_t getVersion() const;

    std::shared_ptr<uint32_t> getOptions() const;
};
}
}

#endif
