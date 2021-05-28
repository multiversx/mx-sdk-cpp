#ifndef BASE64_H
#define BASE64_H

#include <string>

typedef unsigned char uchar;

namespace util
{
  std::string base64_encode(const std::string &in);
  std::string base64_decode(const std::string &in);
}

#endif