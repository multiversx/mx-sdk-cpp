#ifndef ERD_BITS_H
#define ERD_BITS_H

#include <vector>
#include <cstdint>

#include "internal/internal.h"

namespace util
{
bytes convertBits(bytes const &data, unsigned int fromBits, unsigned int toBits, bool pad);
}
#endif
