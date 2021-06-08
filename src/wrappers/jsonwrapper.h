#ifndef ERD_WRAPPER_JSON_H
#define ERD_WRAPPER_JSON_H

#include <string>

#include "internal/internal.h"
#include "json/json.hpp"

namespace wrapper
{
namespace json
{
class JsonOrdered
{
public:
    explicit JsonOrdered()
    {}

    template <typename T>
    void set(std::string const &key, T const &value)
    {
        j[key] = value;
    }

    template <typename T>
    T at(std::string const &key) const
    {
        return j.at(key);
    }

    std::string getSerialized() const
    {
        return j.dump();
    }

    void deserialize(std::string const& serialized)
    {
        j = nlohmann::ordered_json::parse(serialized);
        //j = (serialized)_json;
    }

private:
    nlohmann::ordered_json j;
};
}
}

#endif
