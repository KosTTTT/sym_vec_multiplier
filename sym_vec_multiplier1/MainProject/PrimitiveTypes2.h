#pragma once
#include "PrimitiveTypes.h"
#include <memory>
#include <utility>
#include <algorithm>


extern template class std::vector<Vecdotted>;
extern template class std::vector<Scalar>;
/*! Scalars multiplied together. It might be a single scalar also.*/
class ScalarGroup
{
public:
	ScalarGroup() {};
	explicit ScalarGroup(int64_t multiple) :
		m_multiple(multiple) {}
	/*! 
	Multiplies ScalarGroup sg with this. The other ScalarGroup will vanish. 
	@return A reference to this new Scalargroup
	*/
	ScalarGroup& multiply(std::unique_ptr<ScalarGroup>& sg);
	/*! Return true if two ScalarGroups can be added together.*/
	bool canBeAdded(std::unique_ptr<ScalarGroup>const & other) const;
	/*!
	Adds ScalarGroup sg with this. The other ScalarGroup will vanish.
	Befor using this function bool canBeAdded(ScalarGroup const & other) const should be used
	@return A reference to this new Scalargroup
	*/
	ScalarGroup& add(std::unique_ptr<ScalarGroup>& sg);

	inline int64_t multiple() const
	{
		return m_multiple;
	}
	inline std::vector<Scalar> const& arrScalar() const
	{
		return m_arrScalar;
	}
	inline std::vector<Vecdotted> const& arrVecDotted() const
	{
		return m_arrVecdotted;
	}
	/*! Assign a multiple, replacing existend value*/
	void addMultipe(int64_t m);
	/*! Append a scalar value to the array of Scalars or replaces it if the same symbol found*/
	template<typename ...Params>
	void addScalar(Params&& ...sc)
	{
		hfadd(m_arrScalar, Scalar( std::forward<Params>(sc)... ));
	}
	/*! Append a dot product of two vectors towards VecDotted array or replaces the same element*/
	template<typename ...Params>
	void addVecdotted(Params&& ...vd)
	{
		hfadd(m_arrVecdotted, Vecdotted(std::forward<Params>(vd)...));
	}
private:
	template<typename Container, typename T>
	void hfadd(Container& c, T&& value)
	{
		auto const cend = c.end();
		decltype(c.end()) it_f;


		if constexpr (std::is_same_v<std::decay_t<T>, Scalar>)
		{
			it_f = std::find_if(c.begin(), cend,
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

	/*! A number like -1, 3, 7 etc. Default is 1*/
	int64_t m_multiple = 1;
	/*! Array of scalar multiples*/
	std::vector<Scalar> m_arrScalar;
	/*! Array of scalar dot products of vectors*/
	std::vector<Vecdotted> m_arrVecdotted;
};

