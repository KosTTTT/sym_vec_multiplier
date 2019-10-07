#include "PrimitiveTypes.h"

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
Scalar::Scalar():
	Symbol()
{

}
Scalar::Scalar(wchar_t const* str, unsigned power):
	Symbol(str),
	m_power(power)
{

}
inline bool Scalar::operator==(Scalar const& other) const
{
	return (static_cast<Symbol const&>(*this) == other) && (m_power == other.m_power);
}
bool Scalar::operator !=(Scalar const& other)const
{
	return !(*this == other);
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
bool Vecdotted::operator != (Vecdotted const& other) const
{
	return !(*this==other);
}