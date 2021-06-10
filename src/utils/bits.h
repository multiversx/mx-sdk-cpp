#ifndef ERD_BITS_H
#define ERD_BITS_H

#include <vector>
#include <cstdint>

namespace util
{
std::vector<uint8_t> convertBits(unsigned char *data, unsigned int dataLength,
                                 unsigned int fromBits, unsigned int toBits, bool pad);
}
#endif
