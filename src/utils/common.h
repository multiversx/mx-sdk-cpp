#ifndef ERD_COMMON_H
#define ERD_COMMON_H

#include "errors.h"
#include "stdexcept"

namespace util
{
template <typename T>
inline void checkParam(T const &param, T const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + std::to_string(expectedParam) +", got: " + std::to_string(param));
    }
}

template <>
inline void checkParam<std::string>(std::string const &param, std::string const &expectedParam, errorMessage const &error)
{
    if (param != expectedParam)
    {
        throw std::invalid_argument
                (error + "Expected: " + expectedParam +", got: " + param);
    }
}

inline std::string getCanonicalRootPath(std::string const &path)
{
    // Get absolute path to executable
    std::string canonicalPath = std::string(canonicalize_file_name("/proc/self/exe"));

    // Remove everything in path until mx-sdk-cpp directory and concatenate it with the path
    // Use rfind because github action runs into mx-sdk-cpp/mx-sdk-cpp folder
    auto const pos = canonicalPath.rfind("mx-sdk-cpp");
    canonicalPath = canonicalPath.substr(0, pos);
    return canonicalPath + path;
}

}

#endif
