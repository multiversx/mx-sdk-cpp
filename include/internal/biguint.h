#ifndef ERD_BIG_UINT_H
#define ERD_BIG_UINT_H

#include <string>

class BigUInt
{
public:
    explicit BigUInt(std::string value);

    BigUInt operator*(BigUInt const &rhs);

    std::string getHexValue() const;

    const std::string& getValue() const;

private:
    std::string m_value;
};

#endif //ERD_BIG_UINT_H
