#ifndef MX_TEST_COMMON_H
#define MX_TEST_COMMON_H

#include "utils/common.h"

std::string getCanonicalTestDataPath(std::string const &file)
{
    return util::getCanonicalRootPath("mx-sdk-cpp/tests/testData/" + file);
}

#endif //MX_TEST_COMMON_H
