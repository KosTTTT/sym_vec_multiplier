#ifndef PT_H
#define PT_H

#include <string>
#include <vector>
#include <iostream>


/*! Holds symbolic name of a variable*/
class Symbol
{
	friend std::wostream& operator<<(std::wostream& out, Symbol const& u);
public:
	Symbol();
	explicit Symbol(wchar_t const * str):
		m_sym(str)
	{}
	explicit  Symbol(std::wstring const& str) :
		m_sym(str)
	{}
    explicit  Symbol(std::wstring&& str) noexcept:
		m_sym(std::move(str))
	{}

	std::wstring m_sym;
	bool operator==(Symbol const & other) const;
	bool operator !=(Symbol const& other)const;
    void swap(Symbol & other) noexcept;
};

/*! Holding a symbol name for a vector variable*/
class Vec:public Symbol
{
	friend std::wostream& operator<<(std::wostream& out, Vec const& u);
public:
	using Symbol::Symbol;
};

/*! Holding a symbol name for a scalar variable*/
class Scalar :public Symbol
{
	friend std::wostream& operator<<(std::wostream& out, Scalar const& u);
public:
	Scalar();
	explicit Scalar(wchar_t const * str, unsigned power=1);
	explicit Scalar(std::wstring const& str, unsigned power = 1);
    explicit Scalar(std::wstring && str, unsigned power = 1) noexcept;
    /*! The power of a scalar variable. E.g. t^2. So far must be > 0*/
	unsigned m_power=1;
	bool operator == (Scalar const& other) const ;
	bool operator != (Scalar const& other) const ;
    /*! Multiply this scalar by other.
    @return reference to this new value. Must not be called if this->m_sym!=other.m_sym*/
    Scalar & multiply(Scalar const & other);
    void swap(Scalar & other) noexcept;
};


/*! A dot product of two vectors*/
class Vecdotted
{
	friend std::wostream& operator<<(std::wostream& out, Vecdotted const& u);
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
    Vecdotted(wchar_t const * v1, wchar_t const* v2) :
		m_v1(v1),
		m_v2(v2)
	{}
	Vec m_v1;
	Vec m_v2;
	bool operator == (Vecdotted const& other) const;
	bool operator != (Vecdotted const& other) const;
    void swap(Vecdotted & other) noexcept;
};



#endif // PT_H
