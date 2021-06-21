#ifndef ERD_WRAPPER_HTTP_H
#define ERD_WRAPPER_HTTP_H

#include "http/httplib.h"

#define STATUS_CODE_OK 200
#define STATUS_CODE_BAD_REQUEST 400
#define STATUS_CODE_DEFAULT -1
#define STATUS_MSG_OK "Ok"

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
        int status = STATUS_CODE_DEFAULT;
        bool err = false;
        std::string body;

        auto const res = m_client.Get(path.c_str());

        if (res)
        {
            status = res->status;

            if (status == STATUS_CODE_OK)
            {
                body = res->body;
            }
        }
        else
        {
            err = error(res);
        }

        return Result{status, err, body, getStatusMessage(status)};
    }

    Result post(std::string const &path, std::string const &message)
    {
        int status = STATUS_CODE_DEFAULT;
        bool err = false;
        std::string body;

        auto const res = m_client.Post(path.c_str(), message, "text/plain");

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

private:

    bool error(httplib::Result const &res) const
    {
        return res.error() != httplib::Error::Success;
    }

    std::string getStatusMessage(int const &status) const
    {
        return (status == STATUS_CODE_OK) ? STATUS_MSG_OK : std::string(httplib::detail::status_message(status));
    }

    httplib::Client m_client;
};

} // http
} // wrapper
#endif
