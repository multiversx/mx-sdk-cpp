#ifndef MX_HEX_H
#define MX_HEX_H

#include <string>
#include "internal/internal.h"

namespace util
{
bytes hexToBytes(const std::string &hex);

std::string stringToHex(const std::string &input);

std::string hexToString(const std::string &input);
}

#endif
