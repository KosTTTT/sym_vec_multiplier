#ifndef PT_H
#define PT_H
#include "Fraction.hpp"
#include <string>
#include <iosfwd>
#include <utility>



/*! Holds symbolic name of a variable*/
class Symbol
{
std::string m_sym;
public:

    inline Symbol()
    {}
    inline Symbol(std::string const& str) :
        m_sym(str)
    {}
    inline Symbol(std::string&& str) noexcept:
        m_sym(std::move(str))
    {}
    inline std::string const & sym() const noexcept
    {
        return m_sym;
    }
    inline bool operator == (Symbol const& other) const noexcept
    {
        return m_sym == other.m_sym;
    }
    inline bool operator != (Symbol const& other) const noexcept
    {
        return !(*this == other);
    }
    inline void swap(Symbol & other) noexcept
    {
        m_sym.swap(other.m_sym);
    }
    friend std::ostream& operator<<(std::ostream& out, Symbol const& u);
};

/*! Holding a symbol name for a vector variable*/
class Vec: public Symbol
{
public:
    using Symbol::Symbol;
    friend std::ostream& operator<<(std::ostream& out, Vec const& u);
};







/*! Holding a symbol name for a scalar variable*/
class Scalar: public Symbol
{
    /*! The power of a scalar variable. E.g. t^2. So far must be a rational number*/
    Fraction m_power=1;
public:


    inline Scalar():
        Symbol()
    {}
    inline Scalar(std::string const& str, Fraction power = 1):
        Symbol(str),
        m_power(power)
    {}
    inline Scalar(std::string && str, Fraction power = 1):
        Symbol(std::move(str)),
        m_power(power)
    {}

    inline bool operator==(Scalar const& other) const noexcept
    {
        return (static_cast<Symbol const&>(*this) == other) && (m_power == other.m_power);
    }
    inline bool operator !=(Scalar const& other)const noexcept
    {
        return !(*this == other);
    }
    /*! Multiply this scalar by other.Must not be called if this->m_sym!=other.m_sym
     */
    inline Scalar & multiply(Scalar const & other) noexcept
    {
        m_power+=other.m_power;
        return *this;
    }
    //power is zero
    inline bool isOne() const noexcept
    {
        return !m_power;
    }
    inline Fraction const & power() const
    {
        return m_power;
    }
    inline void swap(Scalar & other) noexcept
    {
        Symbol::swap(other);
        m_power.swap(other.m_power);
    }
    friend std::ostream& operator<<(std::ostream& out, Scalar const& u);
};


/*! A dot product of two vectors*/
class Vecdotted
{
    Vec m_v1;
    Vec m_v2;
public:



    inline Vecdotted() {}
    inline Vecdotted(Vec const & v1, Vec const & v2):
		m_v1(v1),
		m_v2(v2)
	{}
    inline Vecdotted(Vec && v1, Vec && v2) noexcept:
		m_v1(std::move(v1)),
		m_v2(std::move(v2))
	{}

    bool operator == (Vecdotted const& other) const noexcept;
    inline bool operator != (Vecdotted const& other) const noexcept
    {
        return !(*this==other);
    }
    inline void swap(Vecdotted & other) noexcept
    {
        m_v1.swap(other.m_v1);
        m_v2.swap(other.m_v2);
    }
    friend std::ostream& operator<<(std::ostream& out, Vecdotted const& u);
};



#endif // PT_H
