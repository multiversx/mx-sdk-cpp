#ifndef ERD_BITS_H
#define ERD_BITS_H

#include <vector>
#include <cstdint>

#include "internal/internal.h"

namespace util
{
bytes convertBits(bytes const &data, int fromBits, int toBits, bool pad);
}
#endif
