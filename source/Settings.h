#ifndef SETTINGS_H
#define SETTINGS_H

#include <type_traits>
#include <cstdint>
#include <memory>
namespace Settings
{
	/*type for presize calculations*/
	using type_real = int64_t;

	template <typename T = type_real>
	T tolerance_init()
	{
		if constexpr (std::is_same_v<T, double>)
			return T(0.0000000000000001);
		else if  constexpr (std::is_same_v<T, long double>)
			return T(0.000000000000000000000000001l);
		else if  constexpr (std::is_same_v<float, T>)
			return T(0.0000001f);
		else
			return T(1);
			//static_assert(false, "wrong data type for type_real");
	}
	extern type_real tolerance;
}
/*copy unique_ptr*/
template<typename T>
static void cuptr(std::unique_ptr<T>& destinaton, std::unique_ptr<T> const& sourse)
{
	if (sourse)
		destinaton.reset(new T(*sourse));
	else
		destinaton.reset(nullptr);
}

#endif // SETTINGS_H
