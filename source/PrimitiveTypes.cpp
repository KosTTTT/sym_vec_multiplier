#include "PrimitiveTypes.h"
#include <utility>
#include <iostream>
#include <stdexcept>
#include <numeric>




std::ostream& operator<<(std::ostream& out, Scalar const& u)
{
    out << static_cast<Symbol const&>(u);
    //if the power of scalar equals 1 do not print it. User will assume it is 1
    if(u.m_power == 1)
        return out;
    out<<'^';
    out<<'(';
    out<< u.m_power;
    out<<')';
    return out;
}


bool Vecdotted::operator == (Vecdotted const& other) const noexcept
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
    return out << u.m_v1 << ".dot(" << u.m_v2<<')';
}

