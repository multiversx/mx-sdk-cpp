#include "proxy/proxyprovider.h"

#include <utility>

#include "httpwrapper.h"
#include "json/json.hpp"
#include "errors.h"

ProxyProvider::ProxyProvider(std::string url):m_url(std::move(url)){}

Account ProxyProvider::getAccount(Address const &address)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/address/" + address.getBech32Address());

    if (result.error)
    {
        throw std::runtime_error(result.statusMessage);
    }
    if (result.status != STATUS_CODE_OK)
    {
        throw std::runtime_error("Request failed with status: " + result.statusMessage + ". Proxy response: " + result.body);
    }

    nlohmann::json response;
    std::string balance;
    uint64_t nonce;

    try
    {
        response = nlohmann::json::parse(result.body);

        balance = response["data"]["account"]["balance"];
        nonce = response["data"]["account"]["nonce"];
    }
    catch (...)
    {
        throw std::runtime_error(ERROR_MSG_JSON_SERIALIZED + " : " + result.body);
    }

    return Account(address, balance, nonce);
}

std::string ProxyProvider::send(Transaction const &transaction)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.post("/transaction/send", transaction.serialize(), CONTENT_TYPE_JSON);

    if (result.error)
    {
        throw std::runtime_error(result.statusMessage);
    }
    if (result.status != STATUS_CODE_OK)
    {
        throw std::runtime_error("Request failed with status: " + result.statusMessage + ". Proxy response: " + result.body);
    }

    nlohmann::json response;
    std::string txHash;

    try
    {
        response = nlohmann::json::parse(result.body);
        txHash = response["data"]["txHash"];
    }
    catch (...)
    {
        throw std::runtime_error(ERROR_MSG_JSON_SERIALIZED + " : " + result.body);
    }

    return txHash;
}

bool ProxyProvider::transactionExecuted(std::string const &txHash)
{
    wrapper::http::Client client(m_url);
    wrapper::http::Result const result = client.get("/transaction/" + txHash + "/status");

    if (result.error)
    {
        throw std::runtime_error(result.statusMessage);
    }
    if (result.status != STATUS_CODE_OK)
    {
        throw std::runtime_error("Request failed with status: " + result.statusMessage + ". Proxy response: " + result.body);
    }

    nlohmann::json response;
    std::string txStatus;

    try
    {
        response = nlohmann::json::parse(result.body);
        txStatus = response["data"]["status"];
    }
    catch (...)
    {
        throw std::runtime_error(ERROR_MSG_JSON_SERIALIZED + " : " + result.body);
    }

    return txStatus == "success"; //this.status == "executed" || this.status == "success" || this.status == "successful"
}