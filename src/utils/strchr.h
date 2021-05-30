#ifndef ERD_STRINGTOCHAR_H
#define ERD_STRINGTOCHAR_H

#include <string>
#include <vector>

namespace util
{
  inline std::string uCharToStr(unsigned char* chrBuffer, unsigned long long const bufferSize)
  {
    std::string ret;

    for (unsigned int currChar = 0; currChar < bufferSize; ++currChar)
    {
      ret.push_back(chrBuffer[currChar]);
    }

    return ret;
  }
}

#endif
