#ifndef PT_H
#define PT_H

#include <string>
#include <vector>
#include <iosfwd>


/*! Holds symbolic name of a variable*/
class Symbol
{
    friend std::ostream& operator<<(std::ostream& out, Symbol const& u);
public:
    std::string m_sym;
	Symbol();
    explicit Symbol(char const * str):
        m_sym(str)
    {}
    explicit  Symbol(std::string const& str) :
        m_sym(str)
    {}
    explicit  Symbol(std::string&& str) noexcept:
        m_sym(std::move(str))
    {}


    bool operator == (Symbol const& other) const noexcept
    {
        return m_sym == other.m_sym;
    }
    bool operator != (Symbol const& other) const noexcept
    {
        return !(*this == other);
    }
    void swap(Symbol & other) noexcept
    {
        m_sym.swap(other.m_sym);
    }
};

/*! Holding a symbol name for a vector variable*/
class Vec:public Symbol
{
    friend std::ostream& operator<<(std::ostream& out, Vec const& u);
public:
	using Symbol::Symbol;
};

/*! Holding a symbol name for a scalar variable*/
class Scalar :public Symbol
{
    friend std::ostream& operator<<(std::ostream& out, Scalar const& u);
public:
	Scalar();
    explicit Scalar(char const * str, unsigned power=1);
    explicit Scalar(std::string const& str, unsigned power = 1);
    explicit Scalar(std::string && str, unsigned power = 1) noexcept;
    /*! The power of a scalar variable. E.g. t^2. So far must be > 0*/
	unsigned m_power=1;
    bool operator==(Scalar const& other) const noexcept
    {
        return (static_cast<Symbol const&>(*this) == other) && (m_power == other.m_power);
    }
    bool operator !=(Scalar const& other)const noexcept
    {
        return !(*this == other);
    }
    /*! Multiply this scalar by other.
    @return reference to this new value. Must not be called if this->m_sym!=other.m_sym*/
    Scalar & multiply(Scalar const & other) noexcept
    {
        m_power+=other.m_power;
        return *this;
    }
    void swap(Scalar & other) noexcept
    {
        Symbol::swap(other);
        std::swap(m_power, other.m_power);
    }
};


/*! A dot product of two vectors*/
class Vecdotted
{
    friend std::ostream& operator<<(std::ostream& out, Vecdotted const& u);
public:
	Vecdotted() {}
    Vecdotted(Vec const & v1, Vec const & v2):
		m_v1(v1),
		m_v2(v2)
	{}
    Vecdotted(Vec && v1, Vec && v2) noexcept:
		m_v1(std::move(v1)),
		m_v2(std::move(v2))
	{}
    Vecdotted(char const * v1, char const* v2) :
		m_v1(v1),
		m_v2(v2)
	{}
	Vec m_v1;
	Vec m_v2;
	bool operator == (Vecdotted const& other) const;
    bool operator != (Vecdotted const& other) const
    {
        return !(*this==other);
    }
    void swap(Vecdotted & other) noexcept
    {
        m_v1.swap(other.m_v1);
        m_v2.swap(other.m_v2);
    }
};



#endif // PT_H
