#ifndef TRANSACTION_INPUT_WRAPPER_H
#define TRANSACTION_INPUT_WRAPPER_H

#include "iwrapper.h"
#include "account/address.h"

namespace ih
{
namespace wrapper
{
class TransactionInputWrapper : public IWrapper
{
public:
    explicit TransactionInputWrapper(cxxopts::ParseResult const  &inputData);

    uint64_t getNonce() const;

    std::string getValue() const;

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
