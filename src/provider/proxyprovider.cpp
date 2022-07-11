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

ProxyProvider::ProxyProvider(std::string url) :
        m_url(std::move(url))
{}

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

std::string ProxyProvider::send(Transaction const &transaction)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.post("/transaction/send", transaction.serialize(), wrapper::http::applicationJson);

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "txHash");
    return data["txHash"];
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

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "tokenData");
    utility::requireAttribute(data["tokenData"], "balance");

    std::string balance = data["tokenData"]["balance"];

    return balance;
}

std::map<std::string, std::string> ProxyProvider::getAllESDTTokenBalances(Address const &address) const
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address() + "/esdt");

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "esdts");

    auto esdts = data["esdts"];

    std::map<std::string, std::string> ret;
    std::string esdt;
    std::string balance;

    for (const auto &it: esdts.items())
    {
        utility::requireAttribute(it.value(), "balance");

        esdt = it.key();
        balance = it.value()["balance"];
        ret[esdt] = balance;
    }

    return ret;
}

NetworkConfig ProxyProvider::getNetworkConfig() const
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/network/config");

    auto data = internal::getPayLoad(result);

    utility::requireAttribute(data, "config");
    utility::requireAttribute(data["config"], "erd_chain_id");
    utility::requireAttribute(data["config"], "erd_gas_per_data_byte");
    utility::requireAttribute(data["config"], "erd_min_gas_limit");
    utility::requireAttribute(data["config"], "erd_min_gas_price");

    NetworkConfig cfg;
    cfg.chainId = data["config"]["erd_chain_id"];
    cfg.gasPerDataByte = data["config"]["erd_gas_per_data_byte"];
    cfg.minGasLimit = data["config"]["erd_min_gas_limit"];
    cfg.minGasPrice = data["config"]["erd_min_gas_price"];

    return cfg;
}

