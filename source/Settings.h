#ifndef SETTINGS_H
#define SETTINGS_H

#include <type_traits>
#include <limits>
#include <cstdint>
#include <memory>
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
    extern type_real scalar_tol;
}
/*copy unique_ptr*/
template<typename T>
inline static void cuptr(std::unique_ptr<T>& destinaton, std::unique_ptr<T> const& sourse)
{
	if (sourse)
		destinaton.reset(new T(*sourse));
	else
		destinaton.reset(nullptr);
}

#endif // SETTINGS_H
