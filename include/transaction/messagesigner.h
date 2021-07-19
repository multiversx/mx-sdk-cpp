#ifndef ERD_MESSAGE_SIGNER_H
#define ERD_MESSAGE_SIGNER_H

#include "transaction/signer.h"
#include "internal/internal.h"
#include "account/address.h"

#define ERD_SIGNED_MSG_PREFIX std::string("\x17""Elrond Signed Message:\n")

class MessageSigner : protected Signer
{
public:
    explicit MessageSigner(bytes const &seed);

    std::string getSignature(std::string const &message) const override;

    bool verify(std::string const &signature, std::string const &message) const override;

    static bool verify(std::string const &signature, std::string const &message, bytes const &publicKey);

    static bool verify(std::string const &signature, std::string const &message, Address const &address);

    static std::string computeHash(std::string const &message);
};


#endif //ERD_MESSAGE_SIGNER_H
