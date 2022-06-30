#ifndef ERD_SC_ARGUMENTS_H
#define ERD_SC_ARGUMENTS_H

#include <vector>

#include "internal/biguint.h"
#include "account/address.h"

class SCArguments
{
public:
    explicit SCArguments();

    void add(std::string const &arg);

    void add(BigUInt const &arg);

    void add(Address const &arg);

    bool empty() const;

    std::string asOnData() const;

private:
    std::vector<std::string> m_args;
};


#endif //ERD_SC_ARGUMENTS_H
