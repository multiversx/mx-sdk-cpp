#include "transaction/messagesigner.h"
#include "cryptosignwrapper.h"

MessageSigner::MessageSigner(bytes const &seed) : Signer(seed) {}

std::string MessageSigner::getSignature(const std::string &message) const
{
    std::string const hashedMsg = computeERDPrefixedMsgHash(message);

    return Signer::getSignature(hashedMsg);
}

bool MessageSigner::verify(std::string const &signature, std::string const &message) const
{
    std::string const hashedMsg = computeERDPrefixedMsgHash(message);

    return Signer::verify(signature, hashedMsg);
}

bool MessageSigner::verify(std::string const &signature, std::string const &message, bytes const &publicKey)
{
    std::string const hashedMsg = computeERDPrefixedMsgHash(message);

    return Signer::verify(signature, hashedMsg, publicKey);
}

bool MessageSigner::verify(std::string const &signature, std::string const &message, Address const &address)
{
    std::string const hashedMsg = computeERDPrefixedMsgHash(message);

    return Signer::verify(signature, hashedMsg, address);
}

std::string MessageSigner::computeERDPrefixedMsgHash(std::string const &message)
{
    std::string const erdMsg = ERD_SIGNED_MSG_PREFIX + std::to_string(message.size()) + message;

    return wrapper::crypto::sha3Keccak(erdMsg);
}

