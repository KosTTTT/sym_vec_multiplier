#ifndef SETTINGS_H
#define SETTINGS_H

#include <type_traits>
#include <limits>
#include <cstdint>
#include <memory>
#include <optional>



namespace Settings
{
    /*type for scalar multiplier*/
    using type_real = long double;

	template <typename T = type_real>
    constexpr T tolerance_init()
	{
        if constexpr (std::numeric_limits<T>::is_integer)
			return T(1);
        else
            return std::numeric_limits<T>::epsilon();
	}
    inline type_real const scalar_tol=tolerance_init();
}
/*copy unique_ptr*/
template<typename T>
inline void cuptr(std::unique_ptr<T>& destinaton, std::unique_ptr<T> const& sourse)
{
	if (sourse)
		destinaton.reset(new T(*sourse));
	else
		destinaton.reset(nullptr);
}
template<typename T>
void swapo(std::optional<T> & f, std::optional<T> & s)
{
    if(f && s)
    {
        f->swap(*(s));
    }
    else if(f &&!s)
    {
        s.emplace(*f);
        f.reset();
    }
    else if(!f && s)
    {
        f.emplace(*s);
        s.reset();
    }
}
#endif // SETTINGS_H
