#include "provider/proxyprovider.h"
#include "apiresponse.h"
#include "httpwrapper.h"

namespace internal
{
    ErdGenericApiResponse parse(wrapper::http::Result const &res)
    {
        if (res.error)
        {
            throw std::runtime_error(res.statusMessage);
        }

        return ErdGenericApiResponse(res.body);
    }

    nlohmann::json getPayLoad(wrapper::http::Result const &res)
    {
        ErdGenericApiResponse response = parse(res);
        response.checkSuccessfulOperation();

        return response.getData<nlohmann::json>();
    }
}

ProxyProvider::ProxyProvider(std::string url):
    m_url(std::move(url)) {}

Account ProxyProvider::getAccount(Address const &address)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address());

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "account");
    utility::requireAttribute(data["account"], "balance");
    utility::requireAttribute(data["account"], "nonce");

    std::string const balance = data["account"]["balance"];
    uint64_t const nonce = data["account"]["nonce"];

    return Account(address, balance, nonce);
}

TransactionHash ProxyProvider::send(Transaction const &transaction)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.post("/transaction/send", transaction.serialize(), wrapper::http::applicationJson);

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "txHash");

    std::string const txHash = data["txHash"];

    return TransactionHash{txHash};
}

TransactionStatus ProxyProvider::getTransactionStatus(std::string const &txHash)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/transaction/" + txHash + "/status");

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "status");

    std::string const txStatus = data["status"];

    return TransactionStatus(txStatus);
}

std::string ProxyProvider::getESDTTokenBalance(Address const &address, std::string const &token) const
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address() + "/esdt/" + token);

    auto data = internal::getDataIfValid(result);

    if (!data.contains("tokenData"))
        throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "tokenData");
    if (!data["tokenData"].contains("balance"))
        throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "balance");

    std::string balance = data["tokenData"]["balance"];

    return balance;
}

std::map<std::string, std::string> ProxyProvider::getAllESDTTokenBalances(Address const &address) const
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address() + "/esdt");

    auto data = internal::getDataIfValid(result);

    if (!data.contains("esdts"))
        throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "esdts");

    auto esdts = data["esdts"];

    std::map<std::string, std::string> ret;
    std::string esdt;
    std::string balance;

    for (const auto& it : esdts.items())
    {
        esdt = it.key();

        if (!it.value().contains("balance"))
            throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "balance for token " + esdt);

        balance = it.value()["balance"];

        ret[esdt] = balance;
    }

    return ret;
}