#ifndef ERD_PROXY_API_RESPONSE_H
#define ERD_PROXY_API_RESPONSE_H

#include "httpwrapper.h"
#include "jsonwrapper.h"

class ErdGenericApiResponse
{
public:
    explicit ErdGenericApiResponse(std::string rawData)
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

    std::string getError() const
    {
        std::string ret;

        if (m_response.contains("error"))
            if(m_response["error"] != "")
                ret = m_response["error"];

       return ret;
    }

    std::string getCode() const
    {
        if (!m_response.contains("code"))
            throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "code");

        return m_response["code"];
    }

    template <class T>
    T getData() const
    {
        if (!m_response.contains("data"))
            throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + "data");

        return m_response["data"];
    }

private:
    nlohmann::json m_response;
};

#endif
