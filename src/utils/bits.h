#ifndef ERD_BITS_H
#define ERD_BITS_H

#include <vector>
#include <stdint.h>

namespace util
{
std::vector<uint8_t> convertBits(unsigned char *data, unsigned int const dataLength,
                                 int const fromBits, int const toBits, bool const pad);
}
#endif