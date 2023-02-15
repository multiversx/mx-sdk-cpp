#ifndef MX_CLI_SECRET_KEY_PROVIDER_H
#define MX_CLI_SECRET_KEY_PROVIDER_H

#include "inputhandler/options.h"
#include "filehandler/isecretkey.h"

class SecretKeyProvider : public ISecretKey
{
public:
    explicit SecretKeyProvider(cxxopts::ParseResult const &input);

    bytes getSeed() const override;

    Address getAddress() const override;

private:
    std::shared_ptr<ISecretKey> m_keyFile;
};


#endif
