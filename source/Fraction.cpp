#include "Fraction.h"
#include <iostream>
#include <stdexcept>
#include <numeric>


Fraction::Fraction(int64_t n, int64_t d)
{
    if (d == 0)
    {
        throw std::runtime_error("Denominator must not be 0.");
    }

    if(d == 1)
    {
        m_num = n;
        m_den = 1;
        return;
    }

    if (n == 0)
    {
        m_num = 0;
        m_den = 1;
    }
    else
    {
        int8_t sign = 1;
        if (n < 0)
        {
            sign = -sign;
            n = -n;
        }
        if (d < 0)
        {
            sign = -sign;
            d = -d;
        }

        int64_t tmp = std::gcd(n, d);
        m_num = n/tmp*sign;
        m_den = d/tmp;
    }
}
std::ostream& operator<<(std::ostream &strm, Fraction const &a)
{
    if (a.m_den == 1)
    {
        strm << a.m_num;
    }
    else
    {
        strm << a.m_num << '/' << a.m_den;
    }
    return strm;
}
