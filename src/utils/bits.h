#ifndef MX_BITS_H
#define MX_BITS_H

#include <vector>
#include <cstdint>

#include "internal/internal.h"

namespace util
{
bytes convertBits(bytes const &data, unsigned int fromBits, unsigned int toBits, bool pad);
}
#endif
