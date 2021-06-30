#ifndef ERD_PROXY_API_RESPONSE_H
#define ERD_PROXY_API_RESPONSE_H

#include <stdexcept>

#include "json/json.hpp"
#include "../utils/errors.h"

namespace utility
{
void requireAttribute(nlohmann::json const &data, std::string const &attribute)
{
    if (!data.contains(attribute))
    {
        throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + attribute);
    }
}
}

class ErdGenericApiResponse
{
public:
    explicit ErdGenericApiResponse(std::string const &rawData)
    {
        try
        {
            m_response = nlohmann::json::parse(rawData);
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_JSON_SERIALIZED + rawData);
        }
    }

    void checkSuccessfulOperation() const
    {
        std::string const error = getError();
        std::string const code = getCode();

        bool const success = error.empty() && (code.find("success")!= std::string::npos);

        if (!success)
        {
            throw std::runtime_error(ERROR_MSG_HTTP_REQUEST_FAILED + code + ". " + ERROR_MSG_REASON + error);
        }
    }

    std::string getError() const
    {
        std::string ret;

        if (m_response.contains("error"))
        {
            if(m_response["error"] != "")
            {
                ret = m_response["error"];
            }
        }

       return ret;
    }

    std::string getCode() const
    {
        utility::requireAttribute(m_response, "code");

        return m_response["code"];
    }

    template <class T>
    T getData() const
    {
        utility::requireAttribute(m_response, "data");

        return m_response["data"];
    }

private:

    nlohmann::json m_response;
};

#endif
