#ifndef H_FRACTION
#define H_FRACTION
#include <stdint.h>
#include <iosfwd>
#include <utility>

/**
 * @brief  munus sign is alway at the denominator. Default is zero.
 * Implicitly converts to double
 */
class Fraction
{

    int64_t m_num = 0;
    int64_t m_den = 1;
public:

        inline Fraction() noexcept
        {}
        Fraction(int64_t n, int64_t d = 1);
        inline Fraction(int n, int d = 1):
            Fraction(static_cast<int64_t>(n), static_cast<int64_t>(d))
        {}
        Fraction(double v);
        inline int64_t num() const  noexcept
        {
            return m_num;
        }
        inline int64_t den() const  noexcept
        {
            return m_den;
        }
        inline Fraction& operator=(int v)
        {
            return *this = Fraction(static_cast<int64_t>(v));
        }
        inline Fraction& operator=(int64_t v)
        {
            return *this = Fraction(v);
        }
        inline Fraction& operator=(double v)
        {
            return *this = Fraction(v);
        }
        inline operator bool() const  noexcept
        {
            return m_num;
        }
        inline operator float() const noexcept
        {
            return static_cast<float>(m_num)/m_den;
        }
        inline operator double() const noexcept
        {
            return static_cast<double>(m_num)/m_den;
        }
        inline Fraction operator-() const noexcept
        {
            return {-m_num, m_den};
        }
        inline bool operator<(Fraction const & other) const noexcept
        {
            return m_num*other.m_den < other.m_num*m_den;
        }
        inline bool operator<(int other) const noexcept
        {
            return *this<Fraction(other);
        }
        inline bool operator<(int64_t other) const noexcept
        {
            return *this<Fraction(other);
        }
        inline bool operator<(double other) const noexcept
        {
            return *this<Fraction(other);
        }
        friend inline bool operator==(Fraction const& lhs, Fraction const& rhs) noexcept
        {
            return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
        }
        friend inline bool operator==(Fraction const& lhs, int r) noexcept
        {
            return lhs==Fraction(r);
        }
        friend inline bool operator==(Fraction const& lhs, int64_t r) noexcept
        {
            return lhs==Fraction(r);
        }
        friend inline bool operator!=(Fraction const& lhs, Fraction const& rhs) noexcept
        {
            return !(lhs==rhs);
        }
        friend inline bool operator!=(Fraction const& lhs, int rhs) noexcept
        {
            return !(lhs==rhs);
        }
        friend inline bool operator!=(Fraction const& lhs, int64_t rhs) noexcept
        {
            return !(lhs==rhs);
        }
        inline Fraction & operator+=(Fraction const& rhs)  noexcept
        {
            return *this = Fraction {m_num*rhs.m_den + rhs.m_num*m_den,
                                    m_den*rhs.m_den};
        }
        friend inline Fraction operator+( Fraction lhs,  Fraction const& rhs) noexcept
        {
            return lhs+=rhs;
        }
        friend inline Fraction operator+( Fraction lhs,  int rhs) noexcept
        {
            return lhs+=rhs;
        }
        friend inline Fraction operator+( int lhs,  Fraction rhs) noexcept
        {
            return rhs+=lhs;
        }
        friend inline Fraction operator+( Fraction lhs,  int64_t rhs) noexcept
        {
            return lhs+=rhs;
        }
        friend inline Fraction operator+( int64_t lhs,  Fraction rhs) noexcept
        {
            return rhs+=lhs;
        }
        friend inline Fraction operator+( Fraction lhs,  double rhs) noexcept
        {
            return lhs+=rhs;
        }
        friend inline Fraction operator+( double lhs,  Fraction rhs) noexcept
        {
            return rhs+=lhs;
        }
        friend inline Fraction operator+( Fraction lhs,  float rhs) noexcept
        {
            return lhs+=rhs;
        }
        friend inline Fraction operator+( float lhs,  Fraction rhs) noexcept
        {
            return rhs+=lhs;
        }
        inline Fraction & operator-=(Fraction const& rhs) noexcept
        {
            return *this = Fraction {m_num*rhs.m_den - rhs.m_num*m_den,
                                    m_den*rhs.m_den};
        }
        friend inline Fraction operator-( Fraction lhs,  Fraction const& rhs) noexcept
        {
            return lhs-=rhs;
        }
        friend inline Fraction operator-( Fraction lhs,  int rhs) noexcept
        {
            return lhs-=rhs;
        }
        friend inline Fraction operator-( int lhs,  Fraction const& rhs) noexcept
        {
            return Fraction(lhs)-=rhs;
        }
        friend inline Fraction operator-( Fraction lhs,  int64_t rhs) noexcept
        {
            return lhs-=rhs;
        }
        friend inline Fraction operator-( int64_t lhs,  Fraction const& rhs) noexcept
        {
            return Fraction(lhs)-=rhs;
        }
        friend inline Fraction operator-( Fraction lhs,  double rhs) noexcept
        {
            return lhs-=rhs;
        }
        friend inline Fraction operator-( double lhs,  Fraction const& rhs) noexcept
        {
            return Fraction(lhs)-=rhs;
        }
        friend inline Fraction operator-( Fraction lhs,  float rhs) noexcept
        {
            return lhs-=rhs;
        }
        friend inline Fraction operator-( float lhs,  Fraction const& rhs) noexcept
        {
            return Fraction(lhs)-=rhs;
        }
        inline Fraction & operator*=( Fraction const& rhs) noexcept
        {
            return *this = Fraction {m_num*rhs.m_num,
                                     m_den*rhs.m_den};
        }
        friend inline Fraction operator*(Fraction lhs,  Fraction const& rhs) noexcept
        {
            return lhs*=rhs;
        }
        friend inline Fraction operator*( Fraction lhs,  int rhs) noexcept
        {
            return lhs*=rhs;
        }
        friend inline Fraction operator*( int lhs,  Fraction rhs) noexcept
        {
            return rhs*=lhs;
        }
        friend inline Fraction operator*( Fraction lhs,  int64_t rhs) noexcept
        {
            return lhs*=rhs;
        }
        friend inline Fraction operator*( int64_t lhs,  Fraction rhs) noexcept
        {
            return rhs*=lhs;
        }
        friend inline Fraction operator*( Fraction lhs,  double rhs) noexcept
        {
            return lhs*=rhs;
        }
        friend inline Fraction operator*( double lhs,  Fraction rhs) noexcept
        {
            return rhs*=lhs;
        }
        friend inline Fraction operator*( Fraction lhs,  float rhs) noexcept
        {
            return lhs*=rhs;
        }
        friend inline Fraction operator*( float lhs,  Fraction rhs) noexcept
        {
            return rhs*=lhs;
        }
        inline Fraction & operator/=(Fraction const& rhs)
        {
            return *this = Fraction{m_num*rhs.m_den,
                                    m_den*rhs.m_num};
        }
        friend inline Fraction operator/(Fraction lhs,  Fraction const& rhs)
        {
            return lhs/=rhs;
        }
        friend inline Fraction operator/( Fraction lhs,  int rhs)
        {
            return lhs/=rhs;
        }
        friend inline Fraction operator/( int lhs,  Fraction const& rhs)
        {
            return Fraction(lhs)/=rhs;
        }
        friend inline Fraction operator/( Fraction lhs,  int64_t rhs)
        {
            return lhs/=rhs;
        }
        friend inline Fraction operator/( int64_t lhs,  Fraction const& rhs)
        {
            return Fraction(lhs)/=rhs;
        }
        friend inline Fraction operator/( Fraction lhs,  double rhs)
        {
            return lhs/=rhs;
        }
        friend inline Fraction operator/( double lhs,  Fraction const& rhs)
        {
            return Fraction(lhs)/=rhs;
        }
        friend inline Fraction operator/( Fraction lhs,  float rhs)
        {
            return lhs/=rhs;
        }
        friend inline Fraction operator/( float lhs,  Fraction const& rhs)
        {
            return Fraction(lhs)/=rhs;
        }
        inline void swap(Fraction & other) noexcept
        {
            std::swap(m_num, other.m_num);
            std::swap(m_den, other.m_den);
        }
        friend std::ostream& operator<<(std::ostream &strm, Fraction const &a);
};
#endif
