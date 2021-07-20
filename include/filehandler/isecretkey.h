#ifndef I_SECRET_KEY_H
#define I_SECRET_KEY_H

#include "account/address.h"
#include "internal/internal.h"

class ISecretKey
{
public:
    explicit ISecretKey() = default;

    virtual Address getAddress() const = 0;

    virtual bytes getSeed() const = 0;
};

#endif
