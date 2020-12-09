#include "PrimitiveTypes.h"
#include "Settings.h"
#include <utility>
Symbol::Symbol():
	m_sym()
{}


Scalar::Scalar():
	Symbol()
{

}
Scalar::Scalar(char const* str, unsigned power):
    Symbol(str),
    m_power(power)
{

}
Scalar::Scalar(std::string const& str, unsigned power):
    Symbol(str),
    m_power(power)
{

}
Scalar::Scalar(std::string && str, unsigned power) noexcept:
    Symbol(std::move(str)),
    m_power(power)
{

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


std::ostream& operator<<(std::ostream& out, Symbol const& u)
{
	return out << u.m_sym;
}
std::ostream& operator<<(std::ostream& out, Vec const& u)
{
     return out << '_' << static_cast<Symbol const&>(u);
}
std::ostream& operator<<(std::ostream& out, Vecdotted const& u)
{
    return out << u.m_v1 << "dot" << u.m_v2;
}
std::ostream& operator<<(std::ostream& out, Scalar const& u)
{
    out << static_cast<Symbol const&>(u);
    //if the power of scalar equals 1 do not print it. User will assume it is 1
    if (u.m_power > 1)
        out<<'^'<< u.m_power;
    return out;
}
