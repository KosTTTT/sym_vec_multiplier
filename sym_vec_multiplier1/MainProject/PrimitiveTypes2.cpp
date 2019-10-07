#include "PrimitiveTypes2.h"
#include <algorithm>
#include <type_traits>

	template<typename Container, typename T>
	void hfadd(Container& c, T&& value)
	{
		auto const cend = c.end();
		decltype(c.end()) it_f;


		if constexpr (std::is_same_v<std::decay_t<T>, Scalar>)
		{
			it_f= std::find_if(c.begin(), cend,
				[&value](T const& next)->bool
				{
					return value.m_sym == next.m_sym;
				});
		}
		else if constexpr (std::is_same_v<std::decay_t<T>, Vecdotted>)
		{
			it_f = std::find(c.begin(), cend, value);
		}
		else
		{
			static_assert(false, "error in hfadd. Wrong data type.");
		}
		if (it_f != cend)
		{
			*it_f = value;
		}
		else
			c.emplace_back(std::forward<T>(value));
	}

ScalarGroup& ScalarGroup::multiply(std::unique_ptr<ScalarGroup>& sg)
{
	//multiply them together

	if (sg)
	{
		auto const size_acother = sg->m_arrScalar.size();
		auto const size_avother = sg->m_arrVecdotted.size();
		m_arrScalar.reserve(m_arrScalar.size() + size_acother);
		m_arrVecdotted.reserve(m_arrVecdotted.size() + size_avother);
		//for common Scalar variables increase their extent
		//and for new ones move them from sg to this

		for (decltype(sg->m_arrScalar.size())i = 0; i < size_acother; ++i)
		{
			auto & scnext = sg->m_arrScalar[i];
			auto const it_found = std::find_if(m_arrScalar.begin(), m_arrScalar.end(), 
				[&scnext](Scalar const& next)->bool
				{
					return scnext.m_sym==next.m_sym;
				});
			if (it_found != m_arrScalar.end())
			{
				//found the same symbol variable
				//increase its power
				(*it_found).m_power+=scnext.m_power;
				continue;
			}
			else
				m_arrScalar.emplace_back(std::move(scnext));
		}
		for (decltype(sg->m_arrVecdotted.size())i = 0; i < size_avother; ++i)
		{
			m_arrVecdotted.emplace_back(std::move(sg->m_arrVecdotted[i]));
		}
		m_multiple *= sg->m_multiple;
		sg.reset(nullptr);
	}
	return *this;
}
bool ScalarGroup::canBeAdded(std::unique_ptr<ScalarGroup>const& other) const
{
	//if symbolic variables are the same in any order than two ScalarGroups can be added

	//compare m_arrScalar

	auto hf = [&](auto const& arthis, auto const& arother)->bool
	{
		auto const sc_size = arother.size();

		//размеры должны совпадать
		if (sc_size != arthis.size())
		{
			//if we are trying to add to zero
			if (m_multiple && other->m_multiple)
				return false;
			return true;
		}

		auto const m_arrScalar_end = arthis.end();
		for (decltype(arother.size())i = 0; i < sc_size; ++i)
		{
			//if symbol is not found we cannot add them
			if (std::find(arthis.begin(), m_arrScalar_end, arother[i]) == m_arrScalar_end)
				return false;
		}
		return true;
	};
	if(other)
		return hf(m_arrScalar, other->m_arrScalar) && hf(m_arrVecdotted, other->m_arrVecdotted);
	return false;
}
ScalarGroup& ScalarGroup::add(std::unique_ptr<ScalarGroup>& sg)
{
	if (sg)
	{
		m_multiple += sg->m_multiple;
		sg.reset(nullptr);
	}
	return *this;
}
void ScalarGroup::addMultipe(int64_t m)
{
	m_multiple = m;
}