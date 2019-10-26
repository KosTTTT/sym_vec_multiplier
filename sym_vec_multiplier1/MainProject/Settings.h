#pragma once

#include <type_traits>
namespace Settings
{
	/*type for presize calculations*/
	using type_real = double;

	template <typename T = type_real>
	T tolerance_init()
	{
		if constexpr (std::is_same_v<T, double>)
			return 0.0000000000000001;
		else if  constexpr (std::is_same_v<T, long double>)
			return 0.000000000000000000000000001;
		else if  constexpr (std::is_same_v<float, T>)
			return 0.0000001;
		else
			return 1;
			//static_assert(false, "wrong data type for type_real");
	}

	extern type_real tolerance;

}

