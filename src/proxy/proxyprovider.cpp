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