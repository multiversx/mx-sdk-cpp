#include "cli_utility.h"

namespace cli
{
namespace utility
{

Transaction createTransaction(cxxopts::ParseResult const &result, Config const &config)
{
    ih::wrapper::TransactionInputWrapper const txWrapper(result);
    Transaction tx;

    tx.m_nonce = txWrapper.getNonce();
    tx.m_value = txWrapper.getValue();
    tx.m_receiver = txWrapper.getReceiver();
    tx.m_sender = DEFAULT_SENDER;
    tx.m_receiverUserName = txWrapper.getReceiverName();
    tx.m_senderUserName = txWrapper.getSenderName();
    tx.m_gasPrice = txWrapper.getGasPrice();
    tx.m_gasLimit = txWrapper.getGasLimit();
    tx.m_data = txWrapper.getData();
    tx.m_signature = DEFAULT_SIGNATURE;
    tx.m_chainID = config.chainID;
    tx.m_version = txWrapper.getVersion();
    tx.m_options = txWrapper.getOptions();

    return tx;
}

void signTransaction(Transaction &transaction, std::shared_ptr<ISecretKey> const &keyFile)
{
    auto const senderSeed = keyFile->getSeed();
    auto const senderAddress = keyFile->getAddress();

    Signer signer(senderSeed);

    transaction.m_sender = std::make_shared<Address>(senderAddress);
    transaction.sign(signer);
}

}
}


