#ifndef I_SECRET_KEY_PROVIDER_H
#define I_SECRET_KEY_PROVIDER_H

#include "account/address.h"
#include "internal/internal.h"

class ISecretKeyProvider
{
public:
    explicit ISecretKeyProvider() = default;

    virtual Address getAddress() const = 0;

    virtual bytes getSeed() const = 0;
};

#endif
