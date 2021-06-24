#ifndef ERD_PROXY_DATA_TRANSACTION_H
#define ERD_PROXY_DATA_TRANSACTION_H

#include <string>

struct TransactionHash
{
    std::string hash;
};

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

