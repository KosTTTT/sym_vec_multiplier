#ifndef H_FRACTION
#define H_FRACTION
#include <stdint.h>
#include <iosfwd>
#include <utility>

/**
 * munus sign is alway at the denominator
 * default is zero
 * @brief The Fraction class
 */
class Fraction
{

    int64_t m_num = 0;
    int64_t m_den = 1;
public:

        inline Fraction() noexcept
        {}

        Fraction(int64_t n, int64_t d = 1);
        inline int64_t num() const  noexcept
        {
            return m_num;
        }
        inline int64_t den() const  noexcept
        {
            return m_den;
        }
        inline Fraction operator-() const noexcept
        {
            Fraction fr;
            fr.m_num = -m_num;
            fr.m_den = m_den;
            return fr;
        }
        inline bool operator>(Fraction const & other) const noexcept
        {
            return m_num*other.m_den > other.m_num*m_den;
        }
        inline explicit operator bool() const  noexcept
        {
            return m_num;
        }
        inline explicit operator float() const  noexcept
        {
            return static_cast<float>(m_num)/m_den;
        }
        inline explicit operator double() const  noexcept
        {
            return static_cast<double>(m_num)/m_den;
        }
        inline Fraction & operator+=(Fraction const& rhs)  noexcept
        {
            return *this = Fraction {m_num*rhs.m_den + rhs.m_num*m_den,
                                    m_den*rhs.m_den};
        }
        inline Fraction & operator-=(Fraction const& rhs) noexcept
        {
            return *this = Fraction {m_num*rhs.m_den - rhs.m_num*m_den,
                                    m_den*rhs.m_den};
        }
        friend inline Fraction operator+( Fraction lhs,  Fraction const& rhs) noexcept
        {
            lhs+=rhs;
            return lhs;
        }
        friend inline Fraction operator-( Fraction lhs,  Fraction const& rhs) noexcept
        {
            lhs-=rhs;
            return lhs;
        }
        inline Fraction & operator*=( Fraction const& rhs) noexcept
        {
            return *this = Fraction {m_num*rhs.m_num,
                                     m_den*rhs.m_den};
        }
        friend inline Fraction operator*(Fraction lhs,  Fraction const& rhs) noexcept
        {
            lhs*=rhs;
            return lhs;
        }
        inline Fraction & operator/=(Fraction const& rhs)
        {
            return *this = Fraction{m_num*rhs.m_den,
                                    m_den*rhs.m_num};
        }
        friend inline Fraction operator/(Fraction lhs,  Fraction const& rhs)
        {
            lhs/=rhs;
            return lhs;
        }
        friend inline bool operator==(Fraction const& lhs, Fraction const& rhs) noexcept
        {
            return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
        }
        friend inline bool operator!=(Fraction const& lhs, Fraction const& rhs) noexcept
        {
            return !(lhs==rhs);
        }
        inline void swap(Fraction & other) noexcept
        {
            std::swap(m_num, other.m_num);
            std::swap(m_den, other.m_den);
        }
        friend std::ostream& operator<<(std::ostream &strm, Fraction const &a);
};








#endif
