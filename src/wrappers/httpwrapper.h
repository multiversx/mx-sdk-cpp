#ifndef ERD_WRAPPER_HTTP_H
#define ERD_WRAPPER_HTTP_H

#include <string>
#include "http/httplib.h"


namespace http
{

struct Result
{
    int status;
    bool error;
    std::string body;
};

class Client
{
public:
    explicit Client(std::string const &url): m_client(url.c_str()){}

    Result get(std::string const &path)
    {
        std::string body;
        int status = -1;
        bool error = false;

        m_client.Get(path.c_str());

        if (auto res = m_client.Get(path.c_str()))
        {
            status = res->status;

            if (res->status == 200)
            {
                body = res->body;
            }
        }
        else
        {
            error = (res.error() != httplib::Error::Success);
        }

        return Result{status, error, body};
    }

    Result post(std::string const &path, std::string const &message)
    {
        auto res = m_client.Post(path.c_str(), message.c_str(), "text/plain");

        return Result {res->status, res.error() != httplib::Error::Success, res->body};
    }

    std::string getStatusMessage(int const &status)
    {
        return  std::string(httplib::detail::status_message(status));
    }

private:
    httplib::Client m_client;
};
}

#endif
