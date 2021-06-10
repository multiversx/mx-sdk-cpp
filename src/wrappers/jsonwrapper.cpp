#include "jsonwrapper.h"

namespace wrapper
{
namespace json
{
bool OrderedJson::contains(std::string const &key) const
{
    return m_json.contains(key);
}

std::string OrderedJson::serialize() const
{
    return m_json.dump();
}

void OrderedJson::deserialize(std::string const& serialized)
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

}
}

