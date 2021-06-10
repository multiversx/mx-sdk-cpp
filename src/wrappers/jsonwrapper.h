#ifndef ERD_WRAPPER_JSON_H
#define ERD_WRAPPER_JSON_H

#include <string>

#include "internal/internal.h"
#include "json/json.hpp"
//TODO: Ugly include, must take care of CMake here. Also, move implementation in CPP
#include "../utils/errors.h"
#include "../utils/base64.h"


namespace internal
{
template <class T>
inline void set(nlohmann::ordered_json &json, std::string const &key, T const &value)
{
    json[key] = value;
}

template<>
inline void set<bytes>(nlohmann::ordered_json &json, std::string const &key, bytes const &value)
{
    std::string val(value.begin(),value.end());
    json[key] = util::base64::encode(val);
}

template<class T>
inline T at(nlohmann::ordered_json const &json, std::string const &key)
{
    return json.at(key);
}

template<>
inline bytes at<bytes>(nlohmann::ordered_json const &json, std::string const &key)
{
    std::string val = json.at(key);
    return bytes(val.begin(), val.end());
}

}

namespace wrapper
{
namespace json
{
class OrderedJson
{
public:
    explicit OrderedJson() = default;

    template <typename T>
    void set(std::string const &key, T const &value)
    {
        internal::set(m_json, key, value);
    }

    bool contains(std::string const &key) const
    {
        return m_json.contains(key);
    }

    template <typename T>
    T at(std::string const &key) const
    {
        try
        {
            return internal::at<T>(m_json, key);
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_JSON_KEY_NOT_FOUND + key);
        }
    }

    std::string serialize() const
    {
        return m_json.dump();
    }

    void deserialize(std::string const& serialized)
    {
        try
        {
            m_json = nlohmann::ordered_json::parse(serialized);
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_JSON);
        }
    }

private:
    nlohmann::ordered_json m_json;
};
}
}

#endif
