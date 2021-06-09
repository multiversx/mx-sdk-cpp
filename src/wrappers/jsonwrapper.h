#ifndef ERD_WRAPPER_JSON_H
#define ERD_WRAPPER_JSON_H

#include <string>

#include "internal/internal.h"
#include "json/json.hpp"
//TODO: Ugly include, must take care of CMake here
#include "../utils/errors.h"

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
        m_json[key] = value;
    }

    bool contains(std::string const &key) const
    {
        return m_json.contains(key);
    }

    template <typename T>
    T at(std::string const &key) const
    {
        return m_json.at(key);
    }

    std::string getSerialized() const
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
