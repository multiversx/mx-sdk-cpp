#ifndef ERD_WRAPPER_JSON_H
#define ERD_WRAPPER_JSON_H

#include <sstream>

#include "internal/internal.h"
#include "json/json.hpp"
#include "../utils/errors.h"
#include "../utils/base64.h"

namespace internal
{

template<class T>
inline void set(nlohmann::ordered_json &json, std::string const &key, T const &value)
{
    json[key] = value;
}

template<>
inline void set<bytes>(nlohmann::ordered_json &json, std::string const &key, bytes const &value)
{
    std::string const val(value.begin(), value.end());
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
    std::string const tmp = json.at(key);
    std::string const val = util::base64::decode(tmp);

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

    bool contains(std::string const &key) const
    {
        return m_json.contains(key);
    }

    bool empty() const
    {
        return m_json.empty();
    }

    template <typename T>
    void set(std::string const &key, T const &value)
    {
        try
        {
            if (!multipleKeys(key))
            {
                internal::set(m_json, key, value);
            }
            else
            {
                std::vector<std::string> const keys = getKeys(key);

                std::string currKey;
                nlohmann::ordered_json nestedJson;
                uint32_t const noOfNestedJsons = keys.size() - 1;

                // Example: set {key, value} = {[a][b][c], d}
                // [a][b][c] = d
                // json1[c] = d <=> {b:c}
                // json2[b] = json1 <=> {b:{c:d}}
                // json3[a] = json2 <=> {a:{b:{c:d}}}
                internal::set(nestedJson, keys.back(), value);

                for (int i = (noOfNestedJsons - 1); i >= 0; --i)
                {
                    currKey = keys[i];
                    nlohmann::ordered_json const tmpJson = nestedJson;

                    nestedJson.clear();
                    nestedJson[currKey] = tmpJson;
                }
                m_json = nestedJson;
            }
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_JSON_SET_KEY + key);
        }
    }

    template <typename T>
    T at(std::string const &key) const
    {
        try
        {
            if (!multipleKeys(key))
            {
                return internal::at<T>(m_json, key);
            }
            else
            {
                std::vector<std::string> const keys = getKeys(key);

                std::string currKey;
                auto const noOfNestedJsons = keys.size() - 1;

                // Example: get {key, value} = {[a][b][c], d}
                // [a][b][c] = d
                // json[a] = json[b] <=> {a:{b:{c:d}}}
                // json[b] = json[c] <=> {b:{c:d}}
                // json[c] = d       <=> {c:d}
                nlohmann::ordered_json tmpJson(m_json);

                for (auto i = 0; i < noOfNestedJsons; i++)
                {
                    currKey = keys[i];
                    tmpJson = tmpJson[currKey];
                }

                return internal::at<T>(tmpJson, keys.back());
            }
        }
        catch (...)
        {
            throw std::invalid_argument(ERROR_MSG_JSON_AT_KEY + key);
        }
    }

    std::string serialize() const
    {
        if (empty()) throw std::invalid_argument(ERROR_MSG_JSON_SERIALIZE_EMPTY);
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
            throw std::invalid_argument(ERROR_MSG_JSON_SERIALIZED);
        }
    }

private:
    bool multipleKeys(std::string const& key) const
    {
        return key.find(kKeyDelimiter) != std::string::npos;
    }

    std::vector <std::string> getKeys(std::string const& keys) const
    {
        std::vector <std::string> tokens;
        std::stringstream stream(keys);
        std::string currToken;

        while(std::getline(stream, currToken, kKeyDelimiter))
        {
            tokens.push_back(currToken);
        }

        return tokens;
    }

    char const kKeyDelimiter = '/';
    nlohmann::ordered_json m_json;
};
}
}

#endif
