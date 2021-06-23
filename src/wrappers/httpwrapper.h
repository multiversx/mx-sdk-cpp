#ifndef ERD_WRAPPER_HTTP_H
#define ERD_WRAPPER_HTTP_H

#include "http/httplib.h"

#define STATUS_CODE_DEFAULT -1
#define STATUS_CODE_OK 200

namespace wrapper
{
namespace http
{

struct Result
{
    int status;
    bool error;
    std::string body;
    std::string statusMessage;
};

class Client
{
public:
    explicit Client(std::string const &url) : m_client(url.c_str())
    {}

    Result get(std::string const &path)
    {
        auto const res = m_client.Get(path.c_str());

        return wrappedResult(res);
    }

    Result post(std::string const &path, std::string const &message)
    {
        char const *contentType = "text/plain";
        auto const res = m_client.Post(path.c_str(), message, contentType);

        return wrappedResult(res);
    }

private:

    Result wrappedResult(httplib::Result const &res)
    {
        int status = STATUS_CODE_DEFAULT;
        bool err = false;
        std::string body;

        if (res)
        {
            status = res->status;
            body = res->body;
        }
        else
        {
            err = error(res);
        }

        return Result{status, err, body, getStatusMessage(status)};
    }

    bool error(httplib::Result const &res) const
    {
        return res.error() != httplib::Error::Success;
    }

    std::string getStatusMessage(int const &status) const
    {
        return std::string(httplib::detail::status_message(status));
    }

    httplib::Client m_client;
};

} // http
} // wrapper
#endif
