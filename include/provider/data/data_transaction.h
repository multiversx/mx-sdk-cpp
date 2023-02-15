#ifndef MX_PROXY_DATA_TRANSACTION_H
#define MX_PROXY_DATA_TRANSACTION_H

#include <string>

// Also see MX-SDK-JS implementation:
// https://github.com/multiversx/mx-sdk-js-core/blob/5c10d7d69dbc424464909398d0874684b59cb2c4/src/transaction.ts#L455
class TransactionStatus
{
public:
    explicit TransactionStatus(std::string status);

    bool operator ==(TransactionStatus const& txStatus);

    bool isPending() const;

    bool isExecuted() const;

    bool isSuccessful() const;

    bool isFailed() const;

    bool isInvalid() const;

private:
    std::string m_status;
};

#endif

