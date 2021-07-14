#ifndef ERD_INTERNAL_H
#define ERD_INTERNAL_H

#include <string>
#include <vector>
#include <cstdint>

typedef std::vector<uint8_t> bytes;

struct KdfParams
{
    uint32_t dklen = 32;
    uint32_t n = 4096;
    uint32_t r = 8;
    uint32_t p = 1;
    std::string salt;
};

#endif //ERD_INTERNAL_H
