#include "PrimitiveTypes2.h"
#include <algorithm>
#include <type_traits>

using namespace std;



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

ScalarGroup& ScalarGroup::multiply(std::unique_ptr<ScalarGroup> const& sg)
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
				m_arrScalar.push_back(scnext);
		}
		for (decltype(sg->m_arrVecdotted.size())i = 0; i < size_avother; ++i)
		{
			m_arrVecdotted.push_back(sg->m_arrVecdotted[i]);
		}
		m_multiple *= sg->m_multiple;
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

//--
void Unit::expand()
{
	if (m_expanded)
		return;//++

	if (m_m.isZero())
	{
		m_m.m_arrUnits.clear();
		m_s.clear();
		m_expanded = true;
		return;//++
	}

	sum_queue m_snew;
	auto const itend1 = m_s.end();
	for (auto it = m_s.begin(); it != itend1; ++it)
	{
		m_snew.splice(m_snew.end(), expand_move(*it));
	}
	

	//units from the multiple. Each Unit has only simple multiples in it
	sum_queue m = moveMultipleq();
	if (m.empty())
	{
		m_s = std::move(m_snew);
	}
	else
	{
		//multiply m by m_snew

		sum_queue ret;

		auto const itend1 = m.end();
		for (auto it1 = m.begin(); it1 != itend1; ++it1)
		{
			auto const itend2 = m_snew.end();
			for (auto it2 = m_snew.begin(); it2 != itend2; ++it2)
			{
				unique_ptr<Unit>u(new Unit(**it1));
				u->m_m.multiply((*it2)->m_m);
				ret.emplace_back(std::move(u));
			}
		}
		m_s = std::move(ret);
	}
	Unit::hmta(m_s);
	m_expanded = true;
	if (m_s.empty())
	{
		return;
	}
	auto next = ++m_s.begin();
	if (next == m_s.end())
	{
		//there is only one unit in m_s. Move it to multiple

		--next;
		m_m = std::move((*next)->m_m);
		m_s.clear();
	}
}
// not tested
std::unique_ptr<Unit> Unit::moveMultiple()
{

	unique_ptr<Unit> u(new Unit);
	u->m_expanded = true;
	if (m_m.isZero())//don't need to open brackets if multiple is zero
	{
		u->m_m.m_sg->addMultipe(0);
	}
	//simple: if there are no Unit multipliers in the multiple, e.g. 5*a and not 5*a*(b+1), 
	//no need to open brackets
	else if (m_m.m_arrUnits.empty())
	{
		u->m_m.m_vec = std::move(m_m.m_vec);
		*u->m_m.m_sg = std::move(*m_m.m_sg);
		m_m.m_sg->addMultipe(1);
	}
	else
	{
		//multiple is not zero and is not simple. It might be like
		//...*(...)*(...+...+...)

		//prepare the multiple
		u->m_m.m_vec = std::move(m_m.m_vec);
		*u->m_m.m_sg = std::move(*m_m.m_sg);
		m_m.m_sg->addMultipe(1);
		//we will multiply and add everything to this new u variable. 
		auto const it_lmEnd = m_m.m_arrUnits.end();
		for (auto it_lmnext = m_m.m_arrUnits.begin(); it_lmnext != it_lmEnd; ++it_lmnext)
		{
			//expand each term of *it_lmnext

			auto const sumsEnd = (*it_lmnext).end();
			auto sumsBegin = (*it_lmnext).begin();
			for (auto it_unext = sumsBegin; it_unext != sumsEnd; ++it_unext)
			{
				(*it_unext)->expand();
			}
			//sum them up in the first one
			if (sumsBegin != sumsEnd)
			{
				for (auto it_next = ++(*it_lmnext).begin(); it_next != sumsEnd; ++it_next)
				{
					(*sumsBegin)->sum(*it_next);
				}
				//now multiply *sumsBegin by u

				u->multiply(*sumsBegin);
				(*it_lmnext).clear();
			}
		}
		m_m.m_arrUnits.clear();
	}
	return u;
}
Unit::sum_queue Unit::moveMultipleq()
{
	sum_queue sm;
	if (!m_m.isOne())
	{
		unique_ptr<Unit> m = moveMultiple();
		if (m->m_s.empty())
		{
			sm.emplace_back(std::move(m));
		}
		else
		{
			sm.splice(sm.end(), m->m_s);
		}
	}
	return sm;
}

inline bool Unit::Multiple::isOne() const
{
	return m_arrUnits.empty() && !m_vec && m_sg->isOne();
}
//--
Unit::sum_queue Unit::expand_move(std::unique_ptr<Unit>& UnitChild)
{
	
	//+++handle base case
	if (!UnitChild)
	{
		return sum_queue{};//+++
	}
	else if (UnitChild->m_s.empty() && UnitChild->m_m.m_arrUnits.empty())
	{
		sum_queue lret;
		UnitChild->m_expanded = true;
		lret.emplace_back(std::move(UnitChild));
		return lret;//+++
	}
	//+++++++++++++++++++


	//handle not a base case

	//sum from multiple. If it is empty, multiple was 1
	sum_queue sm = UnitChild->moveMultipleq();

	// UnitChild multiple is one.
	//sm stores the multiple
	



	//expand each term of m_s
	sum_queue ms_expanded;
	auto const endms = UnitChild->m_s.end();
	for (auto it2 = UnitChild->m_s.begin(); it2 != endms; ++it2)
	{
		unique_ptr<Unit>utmp2(new Unit(**it2));
		sum_queue ustmp = expand_move(utmp2);
		ms_expanded.splice(ms_expanded.end(), ustmp);
	}
	if (!sm.empty())
	{
		//multiply multiple on each term

		sum_queue lret;
		auto const end1 = sm.end();
		for (auto it1 = sm.begin(); it1 != end1; ++it1)
		{
			auto const itend3 = ms_expanded.end();
			for (auto it3 = ms_expanded.begin(); it3 != itend3; ++it3)
			{
					std::unique_ptr<Unit> u(new Unit);
					u->m_m = (*it1)->m_m;
					u->m_m.multiply((*it3)->m_m);
					lret.emplace_back(std::move(u));
			}
		}
		UnitChild.reset(nullptr);
		return lret;//+++
	}
	UnitChild.reset(nullptr);
	return ms_expanded;//+++
}

// not tested
void Unit::sum(std::unique_ptr<Unit>& u)
{
	if (!u)
		return;
	if (m_m.isZero() && u->m_m.isZero())
	{
		//do nothing

		u.reset(nullptr);
		return;
	}
	else if (u->m_m.isZero())
	{
		//do nothing 
		u.reset(nullptr);
		//expand();
		return;
	}
	else if (m_m.isZero())
	{
		//replace this Unit with another one
		
		*this = std::move(*u);
		u.reset(nullptr);
		//expand();
		return;
	}

	//two units are not zero

	u->expand();
	expand();



	bool const thisempty = m_s.empty();
	bool const otherempty = u->m_s.empty();
	if (thisempty && otherempty)
	{
		//just add multiples together
		if (Multiple::canbeadded(m_m,u->m_m))
		{
			m_m.m_sg->add(u->m_m.m_sg);
			u.reset(nullptr);
			return;//++
		}
		else
		{
			//they are terms which cannot be added together
			//move them to the list of terms

			m_s.emplace_back(moveMultiple());
			m_s.emplace_back(std::move(u));
			return;// ++
		}
	}
	else if(thisempty && !otherempty)
	{
		hm(*this, *u);
		//
		m_s = std::move(u->m_s);
		u.reset(nullptr);
		return;//++
	}
	else if (!thisempty && otherempty)
	{
		hm(*u, *this);
		u.reset(nullptr);
		return;//++
	}
	else
	{
		//!thisempty && !otherempty

		//try to add terms from both units together

		auto const thisend = m_s.end();
		for (auto thisnext = m_s.begin(); thisnext != thisend; ++thisnext)
		{
			auto const otherend = u->m_s.end();
			for (auto othernext = u->m_s.begin(); othernext != otherend; ++othernext)
			{
				if (Multiple::canbeadded((*thisnext)->m_m,(*othernext)->m_m))
				{
					(*thisnext)->sum(*othernext);
				}
			}
		}
		//appen to this sum list all units which were not added
		for (auto thisnext = m_s.begin(); thisnext != thisend; ++thisnext)
		{
			auto const otherend = u->m_s.end();
			for (auto othernext = u->m_s.begin(); othernext != otherend; ++othernext)
			{
				if (*othernext)
				{
					m_s.emplace_back(std::move(*othernext));
				}
			}
		}
		u.reset(nullptr);
		return;//++
	}
}
// not tested
void Unit::multiply(std::unique_ptr<Unit>const& u)
{
	if (!u)
		return;
	if (m_m.isZero() || u->m_m.isZero())
	{
		return;//++
	}
	


	//two units are not zero

	u->expand();
	expand();



	bool const thisempty = m_s.empty();
	bool const otherempty = u->m_s.empty();
	if (thisempty && otherempty)
	{
		//multiply multiples together
		m_m.multiply(u->m_m);
		return;//++
	}
	else if (thisempty && !otherempty)
	{
		m_s = copy_sum_queue(u->m_s);
		auto const itend = m_s.end();
		for (auto itnext = m_s.begin(); itnext != itend; ++itnext)
		{
			(*itnext)->m_m.multiply(m_m);
		}

		m_m.setOne();
		return;//++
	}
	else if (!thisempty && otherempty)
	{
		auto const itend = m_s.end();
		for (auto itnext = m_s.begin(); itnext != itend; ++itnext)
		{
			(*itnext)->m_m.multiply(u->m_m);
		}
		return;//++
	}
	else
	{
		//!thisempty && !otherempty

		decltype(m_s) result;

		auto const thisend = m_s.end();
		auto const otherend = u->m_s.end();
		for (auto tn = m_s.begin(); tn != thisend; ++tn)
		{
			for (auto on = u->m_s.begin(); on != otherend; ++on)
			{
				unique_ptr<Unit>u(new Unit);
				u->m_m = (*tn)->m_m;
				u->m_m.multiply((*on)->m_m);
				result.emplace_back(std::move(u));
			}
		}
		m_s = std::move(result);
		hmta(m_s);
		return;//++
	}
}
bool Unit::Multiple::canbeadded(Multiple const& mthis, Multiple const& mother)
{
	bool vbool = !mthis.m_vec && !mother.m_vec;
	if (!vbool)
	{
		if (mthis.m_vec && mother.m_vec)
		{
			vbool = mthis.m_vec == mother.m_vec;
		}
	}
	if (vbool && mthis.m_sg->canBeAdded(mother.m_sg))
	{
		return true;
	}
	return false;
}


void Unit::hm(Unit& empty, Unit& notempty)
{
	unique_ptr<Unit> uempty(empty.moveMultiple());

	bool wasnotadded = true;
	auto itneend = notempty.m_s.end();
	for (auto itnext = notempty.m_s.begin(); itnext != itneend; ++itnext)
	{
		//try to add it
		if (Multiple::canbeadded(uempty->m_m,(*itnext)->m_m))
		{
			(*itnext)->sum(uempty);
			wasnotadded = false;
			break;
		}
	}
	if (wasnotadded)
	{
		//push uempty to the end of the list

		notempty.m_s.emplace_back(std::move(uempty));
	}
}

void Unit::Multiple::multiply(Multiple const & other)
{
	if (m_vec && other.m_vec)
	{
		m_sg->addVecdotted(*m_vec, *other.m_vec);
		m_vec.reset(nullptr);
	}
	if (!m_vec && other.m_vec)
	{
		m_vec.reset(new Vec(*other.m_vec));
	}
	m_sg->multiply(other.m_sg);
}


Unit::Multiple::Multiple(Multiple const& other) :
	m_sg(new ScalarGroup(*other.m_sg)),
	m_vec(new Vec(*other.m_vec))
{
	copy_arrunits(other.m_arrUnits);
}
Unit::Multiple& Unit::Multiple::operator=(Multiple const& other)
{
	m_sg.reset(new ScalarGroup(*other.m_sg));
	m_vec.reset(new Vec(*other.m_vec));
	copy_arrunits(other.m_arrUnits);
	return *this;
}
Unit::Multiple::Multiple(Multiple&& other)noexcept :
	m_sg(std::move(other.m_sg)),
	m_vec(std::move(other.m_vec)),
	m_arrUnits(std::move(other.m_arrUnits))
{

}
Unit::Multiple& Unit::Multiple::operator=(Multiple&& other) noexcept
{
	m_sg=std::move(other.m_sg);
	m_vec = std::move(other.m_vec);
	m_arrUnits = std::move(other.m_arrUnits);
	return *this;
}
void Unit::Multiple::copy_arrunits(std::list<  sum_queue   >  const& other)
{
	auto const itend1 = other.end();
	for (auto itnext1 = other.begin(); itnext1 != itend1; ++itnext1)
	{
		sum_queue s;
		auto const itend2 = (*itnext1).end();
		for (auto itnext2 = (*itnext1).begin(); itnext2 != itend2; ++itnext2)
		{
			std::unique_ptr<Unit> const& UnitPtr = *itnext2;
			s.emplace_back(std::make_unique<Unit>(*UnitPtr));
		}
		m_arrUnits.emplace_back(std::move(s));
	}
}

void Unit::Multiple::setOne()
{
	m_arrUnits.clear();
	m_vec.reset(nullptr);
	m_sg.reset(new ScalarGroup);
}

Unit::Unit(Unit const& other) :
	m_m(other.m_m),
	m_expanded(other.m_expanded)
{
	m_s = copy_sum_queue(other.m_s);
}
Unit& Unit::operator=(Unit const& other)
{
	m_m = other.m_m;
	m_expanded = other.m_expanded;
	m_s = copy_sum_queue(other.m_s);
	return *this;
}
Unit::Unit(Unit&& other) noexcept :
	m_m(std::move(other.m_m)),
	m_s(std::move(other.m_s)),
	m_expanded(other.m_expanded)
{

}
Unit& Unit::operator=(Unit&& other) noexcept
{
	m_m = std::move(other.m_m);
	m_s = std::move(other.m_s);
	m_expanded = other.m_expanded;
	return *this;
}

void Unit::hmta(sum_queue & v)
{
	auto const itend = v.end();
	auto itnext = v.begin();
	if (itend != itnext)
	{
		//we have at least one element


		for (; itnext != itend; ++itnext)
		{
			if (*itnext)
			{
				for (auto itn = v.begin(); itn != itend; ++itn)
				{
					if (*itn)
					{
						if (itn != itnext)
						{
							if(Multiple::canbeadded((*itn)->m_m, (*itnext)->m_m) )
							{
								(*itn)->m_m.m_sg->add((*itnext)->m_m.m_sg);
								(*itnext).reset(nullptr);
							}
						}
					}
				}
			}
		}

		//clean empty elements
		for (auto it = v.begin(); it != itend; )
		{
			auto ittmp = it;
			++it;
			if (*ittmp)
			{
				continue;
			}
			v.erase(ittmp);
		}
	}
}
Unit::sum_queue Unit::copy_sum_queue(sum_queue const& v)
{
	sum_queue vret;
	auto const itend = v.end();
	for (auto itnext = v.begin(); itnext != itend; ++itnext)
	{
		unique_ptr<Unit> const& u = *itnext;
		vret.emplace_back(std::make_unique<Unit>(*u));//copy Unit
	}
	return vret;
}