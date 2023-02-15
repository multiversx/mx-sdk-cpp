#ifndef MX_BIG_UINT_H
#define MX_BIG_UINT_H

#include <string>

class BigUInt
{
public:
    explicit BigUInt(uint64_t value);

    explicit BigUInt(std::string value);

    BigUInt operator*(BigUInt const &rhs) const;

    BigUInt operator/(BigUInt const &rhs) const;

    bool operator==(BigUInt const &rhs) const;

    bool operator>(BigUInt const &rhs) const;

    bool operator<(BigUInt const &rhs) const;

    BigUInt operator-(BigUInt const &rhs) const;

    BigUInt operator+(BigUInt const &rhs) const;

    std::pair<BigUInt, BigUInt> divmod(BigUInt const &rhs) const;

    std::string getHexValue() const;

    const std::string &getValue() const;

private:
    std::string m_value;
};

#endif //MX_BIG_UINT_H
