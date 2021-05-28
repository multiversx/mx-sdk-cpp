#ifndef HEX_H
#define HEX_H

#include <string>
#include <vector>
#include "params.h"
#include <iostream>
#include "../../include/internal/internal.h"

namespace util
{
  bytes hexToBytes(const std::string& hex);

  std::string stringToHex(const std::string& input);

  std::string hexToString(const std::string& input);
}

#endif