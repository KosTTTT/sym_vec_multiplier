#pragma once
#include <string>
#include <vector>
//#include "TemplatesH.h"



class Symbol
{
public:
	Symbol();
	explicit Symbol(wchar_t const * str):
		m_sym(str)
	{}
	/*! Holds symbolic name of a variable*/
	std::wstring m_sym;
	bool operator==(Symbol const & other) const;
	bool operator !=(Symbol const& other)const;
};

/*! Holding a symbol name for a vector variable*/
class Vec:public Symbol
{
public:
	using Symbol::Symbol;
};

/*! Holding a symbol name for a sclar variable*/
class Scalar :public Symbol
{
public:
	Scalar();
	explicit Scalar(wchar_t const * str, unsigned power=1);
	/*! The power of a scalar variable. E.g. t^2*/
	unsigned m_power=1;
	bool operator == (Scalar const& other) const ;
	bool operator != (Scalar const& other) const ;
};


/*! A dot product of two vectors*/
class Vecdotted
{
public:
	Vecdotted() {};
	explicit Vecdotted(Vec const & v1, Vec const & v2):
		m_v1(v1),
		m_v2(v2)
	{}
	explicit Vecdotted(Vec && v1, Vec && v2) :
		m_v1(std::move(v1)),
		m_v2(std::move(v2))
	{}
	explicit Vecdotted(wchar_t const * v1, wchar_t const* v2) :
		m_v1(v1),
		m_v2(v2)
	{}
	Vec m_v1;
	Vec m_v2;
	bool operator == (Vecdotted const& other) const;
	bool operator != (Vecdotted const& other) const;
};



//template<typename T>
//T functestt(T var)
//{
//	var += 1;
//	return var;
//}
//
//template <typename T>
//T functest02(typename Identity<T>::type arg)
//{
//	return arg += 1;
//}