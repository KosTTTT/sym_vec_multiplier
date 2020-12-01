#include "PrimitiveTypes.h"
#include "Settings.h"
#include <utility>
Symbol::Symbol():
	m_sym()
{}

inline bool Symbol::operator == (Symbol const& other) const
{
	return m_sym == other.m_sym;
}
inline bool Symbol::operator != (Symbol const& other) const
{
	return !(*this == other);
}
void Symbol::swap(Symbol & other) noexcept
{
    m_sym.swap(other.m_sym);
}
Scalar::Scalar():
	Symbol()
{

}
Scalar::Scalar(wchar_t const* str, unsigned power):
	Symbol(str),
	m_power(power)
{

}
Scalar::Scalar(std::wstring const& str, unsigned power):
	Symbol(str),
	m_power(power)
{

}
Scalar::Scalar(std::wstring && str, unsigned power) noexcept:
	Symbol(std::move(str)),
	m_power(power)
{

}
inline bool Scalar::operator==(Scalar const& other) const
{
	return (static_cast<Symbol const&>(*this) == other) && (m_power == other.m_power);
}
inline bool Scalar::operator !=(Scalar const& other)const
{
	return !(*this == other);
}
inline Scalar & Scalar::multiply(Scalar const & other)
{
    m_power+=other.m_power;
    return *this;
}
void Scalar::swap(Scalar & other) noexcept
{
    Symbol::swap(other);
    std::swap(m_power, other.m_power);
}

bool Vecdotted::operator == (Vecdotted const& other) const
{
	if (m_v1 == other.m_v1)
	{
		return m_v2 == other.m_v2;
	}
	else if (m_v2 == other.m_v1)
	{
		return m_v1 == other.m_v2;
	}
	return false;
}
void Vecdotted::swap(Vecdotted & other) noexcept
{
    m_v1.swap(other.m_v1);
    m_v2.swap(other.m_v2);
}
inline bool Vecdotted::operator != (Vecdotted const& other) const
{
	return !(*this==other);
}
std::wostream& operator<<(std::wostream& out, Symbol const& u)
{
	return out << u.m_sym;
}
std::wostream& operator<<(std::wostream& out, Vec const& u)
{
     return out << '_' << static_cast<Symbol const&>(u);
}
std::wostream& operator<<(std::wostream& out, Vecdotted const& u)
{
    return out << u.m_v1 << "dot" << u.m_v2;
}
std::wostream& operator<<(std::wostream& out, Scalar const& u)
{
    out << static_cast<Symbol const&>(u);
    //if the power of scalar equals 1 do not print it. User will assume it is 1
    if (u.m_power > 1)
        out<<'^'<< u.m_power;
    return out;
}
