#ifndef ERD_HEX_H
#define ERD_HEX_H

#include <string>
#include "params.h"
#include "internal/internal.h"

namespace util
{
  bytes hexToBytes(const std::string& hex);

  std::string stringToHex(const std::string& input);

  std::string hexToString(const std::string& input);
}

#endif