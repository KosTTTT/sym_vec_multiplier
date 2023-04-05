#include "Fraction.hpp"
#include <iostream>
#include <stdexcept>
#include <numeric>
#include <cassert>
#include <cmath>

Fraction::Fraction(int64_t n, int64_t d)
{
    if(d == 0)
    {
        throw std::runtime_error{"Denominator is 0\n"};
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
        int64_t sign = 1;
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

        int64_t const tmp = std::gcd(n, d);
        m_num = n/tmp*sign;
        m_den = d/tmp;
    }
}
Fraction::Fraction(double v)
{
    int64_t const precision = 100000000;
    //integral part. Might be negative and 0
    int64_t const integral = static_cast<int64_t>(v);
    //get fractional part. Might be negative. might be 0
    int64_t const frac = static_cast<int64_t>(std::round((v - integral)*precision));
    //positive. Cannot be 0
    int64_t const vgcd = std::gcd(frac, precision);
    int64_t const den = precision / vgcd;
    int64_t const num = frac / vgcd;
    *this = Fraction((integral*den)+num, den);
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
