#ifndef BASE64_H
#define BASE64_H

#include <string>

typedef unsigned char uchar;

namespace util
{
  namespace base64
  {
    std::string encode(const std::string &in);
    std::string decode(const std::string &in);
  }
}

#endif