#include "PrimitiveTypes2.h"
#include <algorithm>
#include <type_traits>
#include <utility>
using namespace std;


void Unit::Multiple::copy_arrunitsh(std::list<  sum_queue   >  const& input, std::list<  sum_queue   > & output)
{
    for(auto const & sum: input)
    {
        sum_queue s;
        for(auto const & u: sum)
        {
            if(u)
                s.emplace_back(new Unit(*u));
        }
        if (s.empty())
            continue;
        output.emplace_back(std::move(s));
    }
}

void ScalarGroup::MultByScalar(Scalar const& sc)
{
    //auto it_end = m_arrScalar.end();
    for(size_t i = 0, end = m_arrScalar.size(); i < end; ++i)
    {
        if(sc.m_sym==m_arrScalar[i].m_sym)
        {
            m_arrScalar[i].multiply(sc);
            return;//++
        }
    }
    m_arrScalar.push_back(sc);
}

ScalarGroup& ScalarGroup::multiply(ScalarGroup const& sg)
{
	//multiply them together


    if(isOne())
    {
        *this = sg;
    }
    else if(isMinusOne())
    {
        *this = sg;
        m_multiple = -m_multiple;
    }
    else if(sg.isZero())
    {
        setZero();
    }
    else if(sg.isMinusOne())
    {
        m_multiple = -m_multiple;
    }
    else if(!sg.isOne() && !isZero())
    {
        m_multiple *= sg.m_multiple;

        auto const size_acother = sg.m_arrScalar.size();
        auto const size_avother = sg.m_arrVecdotted.size();
        m_arrScalar.reserve(m_arrScalar.size() + size_acother);
        m_arrVecdotted.reserve(m_arrVecdotted.size() + size_avother);
        //for common Scalar variables increase their extent
        //and for new ones move them from sg to this

        for (size_t i = 0; i < size_acother; ++i)
        {
            Scalar const & scnext = sg.m_arrScalar[i];
            auto end1 = m_arrScalar.end();
            auto it_found = std::find_if(m_arrScalar.begin(), end1,
                [&scnext](Scalar const& next)->bool
                {
                    return scnext.m_sym==next.m_sym;
                });
            if (it_found == end1)
                m_arrScalar.push_back(scnext);
            else//found the same symbol variable
                (*it_found).multiply(scnext);
        }
        for (size_t i = 0; i < size_avother; ++i)
        {
            m_arrVecdotted.push_back(sg.m_arrVecdotted[i]);
        }
    }
	return *this;
}
inline void ScalarGroup::setZero() noexcept
{
	m_multiple = 0;
	m_arrScalar.clear();
	m_arrVecdotted.clear();
}
static bool hf(std::vector<Scalar> const& arthis, std::vector<Scalar> const& arother)
{
    auto const sc_size = arother.size();
    //размеры должны совпадать
    if (sc_size != arthis.size())
        return false;
    auto const m_arrScalar_end = arthis.end();
    for (size_t i = 0; i < sc_size; ++i)
    {
        //if symbol is not found we cannot add them
        auto it_found = std::find(arthis.begin(), m_arrScalar_end, arother[i]);
        if (it_found == m_arrScalar_end)
            return false;//--
    }
    return true;//++
};
static bool hf2(std::vector<Vecdotted> const& arthis, std::vector<Vecdotted> const& arother)
{
    auto const sc_size = arother.size();
    //размеры должны совпадать
    if (sc_size != arthis.size())
        return false;
    for (size_t i = 0; i < sc_size; ++i)
    {
        //check if the number of symbol occurrence is the same
        if (std::count(arthis.begin(), arthis.end(), arother[i])
            != std::count(arother.begin(), arother.end(), arother[i]))
        {
            return false;//--
        }
    }
    return true;//++
};
bool ScalarGroup::canBeAdded(ScalarGroup const& other) const
{
    if(isZero() || other.isZero())
        return true;//++

	//if symbolic variables are the same in any order than two ScalarGroups can be added

    return hf(m_arrScalar, other.m_arrScalar) && hf2(m_arrVecdotted, other.m_arrVecdotted);
}
ScalarGroup& ScalarGroup::add(ScalarGroup const& sg)
{
    if (isZero())
    {
        *this = sg;
    }
    else if (!sg.isZero())
    {
        m_multiple += sg.m_multiple;
        if (isZero())
        {
            setZero();
        }
    }
	return *this;
}
inline void ScalarGroup::AssignMultipe(Settings::type_real m)
{
	m_multiple = m;
}
void ScalarGroup::swap(ScalarGroup & other) noexcept
{
    std::swap(m_multiple, other.m_multiple);
    m_arrScalar.swap(other.m_arrScalar);
    m_arrVecdotted.swap(other.m_arrVecdotted);
}
//--
void Unit::expand()
{
	
	//if (m_expanded)
	//	return;//++

	//if (m_m && m_m->isZero())
	//{
	//	setZero();
	//	m_expanded = true;
	//	return;//++
	//}

	//sum_queue m_snew;
	//auto const itend1 = m_s.end();
	//for (auto it = m_s.begin(); it != itend1; ++it)
	//{
	//	m_snew.splice(m_snew.end(), expand_move(*it));
	//}
	//

	////units from the multiple. Each Unit has only simple multiples in it
	//sum_queue m = moveMultipleq();
	//if (m.empty())
	//{
	//	throw std::exception{"error in expand: empty multiple"};
	//}
	//if (m_snew.empty())
	//{
	//	m_s = std::move(m);
	//}
	//else
	//{
	//	//multiply m by m_snew

	//	sum_queue ret;

	//	auto const itend1 = m.end();
	//	for (auto it1 = m.begin(); it1 != itend1; ++it1)
	//	{
	//		auto const itend2 = m_snew.end();
	//		for (auto it2 = m_snew.begin(); it2 != itend2; ++it2)
	//		{
	//			unique_ptr<Unit>u(new Unit(**it1));
	//			u->m_expanded = true;
	//			u->m_m.multiply((*it2)->m_m);
	//			ret.emplace_back(std::move(u));
	//		}
	//	}
	//	m_s = std::move(ret);
	//}
	//Unit::hmta(m_s);
	//m_expanded = true;
	//Unit::hmfe(*this);








	if (m_expanded)
		return;//++

	//+++handle base case
	if (m_s.empty() && m_m && m_m->m_arrUnits.empty())
	{
		m_expanded = true;
		return;//+++
	}
	//+++++++++++++++++++


	//handle not a base case

	//sum from multiple 
	sum_queue sm = moveMultipleq();

	// multiple is nullptr.
	//sm stores the multiple or empty




	//expand each term of m_s
	sum_queue ms_expanded;
	auto const endms = m_s.end();
	for (auto it2 = m_s.begin(); it2 != endms; ++it2)
	{
		sum_queue ustmp = expand_move(*it2);
		ms_expanded.splice(ms_expanded.end(), ustmp);
	}
	m_s.clear();

	if (ms_expanded.empty() && sm.empty())
		throw std::exception("error during the expansion");
	if (ms_expanded.empty() && !sm.empty())
	{
		m_s = std::move(sm);//+++
	}
	else if (!ms_expanded.empty() && sm.empty())
	{
		Unit::hmta(ms_expanded);
		m_s=std::move( ms_expanded );//++
	}
	else
	{
		Unit::hmta(ms_expanded);
		//multiply multiple on each term

		auto const end1 = sm.end();
		for (auto it1 = sm.begin(); it1 != end1; ++it1)
		{
			auto const itend3 = ms_expanded.end();
			for (auto it3 = ms_expanded.begin(); it3 != itend3; ++it3)
			{
				std::unique_ptr<Unit> u(new Unit);
				u->m_expanded = true;
				u->m_m.reset(new Multiple(*(*it1)->m_m));
				u->m_m->multiply(*(*it3)->m_m);
				m_s.emplace_back(std::move(u));
			}
		}
		//+++
	}

	Unit::hmta(m_s);
	Unit::hmfe(*this);
	m_expanded = true;
}
std::unique_ptr<Unit> Unit::moveMultiple()
{
	if (!m_m)
		return nullptr;
	unique_ptr<Unit> u(new Unit);
	u->m_expanded = true;
	if (m_m->isZero())//don't need to open brackets if multiple is zero
	{
		u->m_m.reset(new Multiple(0));
	}
	//if there are no Unit multipliers in the multiple, e.g. 5*a and not 5*a*(b+1), 
	//no need to open brackets
	else if (m_m->m_arrUnits.empty())
	{
		u->m_m.reset(new Multiple(std::move(*m_m)));
	}
	else
	{

		u->m_m.reset(new Multiple);
		//multiple is not zero and is not simple. It might be like
		//...*(...)*(...+...+...)

		//prepare the multiple
		u->m_m->m_vec = std::move(m_m->m_vec);
		*u->m_m->m_sg = std::move(*m_m->m_sg);
		//we will multiply and add everything to this new u variable. 
		auto const it_lmEnd = m_m->m_arrUnits.end();
		for (auto it_lmnext = m_m->m_arrUnits.begin(); it_lmnext != it_lmEnd; ++it_lmnext)
		{
			//expand each term of *it_lmnext

			//end of the terms
			auto const sumsEnd = (*it_lmnext).end();
			//start of the terms
			auto sumsBegin = (*it_lmnext).begin();
			for (auto it_unext = sumsBegin; it_unext != sumsEnd; ++it_unext)
			{
				//expand unit
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
		m_m->m_arrUnits.clear();
	}
	m_m.reset(nullptr);
	return u;
}
Unit::sum_queue Unit::moveMultipleq()
{
	sum_queue sm;
	unique_ptr<Unit> m = moveMultiple();
	if (m)
	{
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

inline bool Unit::Multiple::isOne() const noexcept
{
    return m_arrUnits.empty() && !m_vec && m_sg && m_sg->isOne();
}
inline bool Unit::Multiple::isMinusOne() const noexcept
{
    return m_arrUnits.empty() && !m_vec && m_sg && m_sg->isMinusOne();
}
//--
Unit::sum_queue Unit::expand_move(std::unique_ptr<Unit>& UnitChild)
{
	sum_queue ret;
	if (UnitChild)
	{
		UnitChild->expand();

		if (UnitChild->m_m)
		{
			ret.emplace_back(std::move(UnitChild));
		}
		else
		{
			ret = std::move(UnitChild->m_s);
			UnitChild.reset(nullptr);
		}
	}
	return ret;

	
	////+++handle base case
	//if (!UnitChild)
	//{
	//	return sum_queue{};//+++
	//}
	//else if (UnitChild->m_s.empty() && UnitChild->m_m && UnitChild->m_m->m_arrUnits.empty())
	//{
	//	sum_queue lret;
	//	UnitChild->m_expanded = true;
	//	lret.emplace_back(std::move(UnitChild));
	//	return lret;//+++
	//}
	////+++++++++++++++++++


	////handle not a base case

	////sum from multiple 
	//sum_queue sm = UnitChild->moveMultipleq();

	//// UnitChild multiple is nullptr.
	////sm stores the multiple or empty
	//



	////expand each term of m_s
	//sum_queue ms_expanded;
	//auto const endms = UnitChild->m_s.end();
	//for (auto it2 = UnitChild->m_s.begin(); it2 != endms; ++it2)
	//{
	//	sum_queue ustmp = expand_move(*it2);
	//	ms_expanded.splice(ms_expanded.end(), ustmp);
	//}
	//UnitChild.reset(nullptr);
	//
	//if (ms_expanded.empty() && sm.empty())
	//	throw std::exception("error during the expansion");
	//if (ms_expanded.empty() && !sm.empty())
	//{
	//	return sm;//+++
	//}
	//else if (!ms_expanded.empty() && sm.empty())
	//{
	//	Unit::hmta(ms_expanded);
	//	return ms_expanded;//++
	//}
	//else
	//{
	//	Unit::hmta(ms_expanded);
	//	//multiply multiple on each term

	//	sum_queue lret;
	//	auto const end1 = sm.end();
	//	for (auto it1 = sm.begin(); it1 != end1; ++it1)
	//	{
	//		auto const itend3 = ms_expanded.end();
	//		for (auto it3 = ms_expanded.begin(); it3 != itend3; ++it3)
	//		{
	//				std::unique_ptr<Unit> u(new Unit);
	//				u->m_expanded = true;
	//				u->m_m.reset(new Multiple( *(*it1)->m_m) );
	//				u->m_m->multiply(*(*it3)->m_m);
	//				lret.emplace_back(std::move(u));
	//		}
	//	}
	//	Unit::hmta(lret);
	//	return lret;//+++
	//}
}

void Unit::sum(std::unique_ptr<Unit>& u)
{
	if (!u)
		return;


	u->expand();
	expand();

	if (u->m_m && u->m_m->isZero())
	{
		//do nothing 
		u.reset(nullptr);
		return;//++
	}
	else if (m_m && m_m->isZero())
	{
		//replace this Unit with another one

		*this = std::move(*u);
		u.reset(nullptr);
		return;//++
	}
	//two units are not zero

	bool const thisempty = m_s.empty();
	bool const otherempty = u->m_s.empty();
	if (thisempty && otherempty)
	{
		//just add multiples together
		if (Multiple::canbeadded(*m_m,*u->m_m))
		{
			m_m->m_sg->add(u->m_m->m_sg);
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
		Unit::hmfe(*this);
		u.reset(nullptr);
		return;//++
	}
	else if (!thisempty && otherempty)
	{
		hm(*u, *this);
		Unit::hmfe(*this);
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
				if (*othernext)
				{
					if (Multiple::canbeadded(*(*thisnext)->m_m, *(*othernext)->m_m))
					{
						(*thisnext)->m_m->m_sg->add((*othernext)->m_m->m_sg);
						(*othernext).reset(nullptr);
					}
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
		Unit::hmfe(*this);
		u.reset(nullptr);
		return;//++
	}
}

void Unit::multiply(std::unique_ptr<Unit>const& u)
{
	if (!u)
		return;

	//two units are not zero

	u->expand();
	expand();


	if (m_m && m_m->isZero())
	{
		return;//++
	}
	if (u->m_m && u->m_m->isZero())
	{
		setZero();
		return;//++
	}


	bool const thisempty = m_s.empty();
	bool const otherempty = u->m_s.empty();
	if (thisempty && otherempty)
	{
		//multiply multiples together
		m_m->multiply(*u->m_m);
		return;//++
	}
	else if (thisempty && !otherempty)
	{

		m_s = copy_sum_queue(u->m_s);
		auto const itend = m_s.end();
		for (auto itnext = m_s.begin(); itnext != itend; ++itnext)
		{
			(*itnext)->m_m->multiply(*m_m);
		}
		m_m.reset(nullptr);
		return;//++
	}
	else if (!thisempty && otherempty)
	{
		auto const itend = m_s.end();
		for (auto itnext = m_s.begin(); itnext != itend; ++itnext)
		{
			(*itnext)->m_m->multiply(*u->m_m);
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
				unique_ptr<Unit>utmp(new Unit);
				utmp->m_m.reset(new Multiple(*(*tn)->m_m));
				utmp->m_m->multiply(*(*on)->m_m);
				utmp->m_expanded = true;
				result.emplace_back(std::move(utmp));
			}
		}
		m_s = std::move(result);
		hmta(m_s);
		hmfe(*this);
		return;//++
	}
}
bool Unit::Multiple::canbeadded(Multiple const& mother) const
{
    bool b = !m_vec && !mother.m_vec;
    if (!b)
	{
        if (m_vec && mother.m_vec)
		{
            b = *m_vec == *mother.m_vec;
		}
	}

    if (b)
	{
        if(m_arrUnits.empty() && mother.m_arrUnits.empty())
        {
            b = !m_sg && !mother.m_sg;
            if(!b)
            {
                if(m_sg && mother.m_sg)
                {
                    b = m_sg->canBeAdded(*mother.m_sg);
                }
            }
            return b;
        }
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
		if (*itnext)
		{
			//try to add it
			if (Multiple::canbeadded(*uempty->m_m, *(*itnext)->m_m))
			{
				(*itnext)->m_m->m_sg->add(uempty->m_m->m_sg);
				wasnotadded = false;
				break;
			}
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
    if(isZero())
        return;

    if(other.isZero())
    {
        setZero();
        return;
    }

    if(m_vec && other.m_vec)
	{
        if(!m_sg)
            m_sg = std::make_optional<ScalarGroup>();
        m_sg->MultByVecdotted(m_vec, other.m_vec);
        m_vec.reset();
	}
	else if (!m_vec && other.m_vec)
	{
        m_vec.emplace(*other.m_vec);
	}

    copy_arrunitsh(other.m_arrUnits,m_arrUnits);

    if(other.m_sg && m_sg)
    {
        m_sg->multiply(*other.m_sg);
    }
    else if(!m_sg && other.m_sg)
    {
        m_sg.emplace(*other.m_sg);
    }
}

Unit::Multiple::Multiple(Multiple const& other):
    m_sg(other.m_sg),
    m_vec(other.m_vec)
{
	copy_arrunits(other.m_arrUnits);
}
Unit::Multiple& Unit::Multiple::operator=(Multiple const& other)
{
    Multiple m(other);
    return *this = std::move(m);
}
Unit::Multiple::Multiple(Multiple&& other)noexcept :
	m_sg(std::move(other.m_sg)),
	m_vec(std::move(other.m_vec)),
	m_arrUnits(std::move(other.m_arrUnits))
{

}
Unit::Multiple& Unit::Multiple::operator=(Multiple&& other) noexcept
{
    swap(other);
    return *this;
}
bool Unit::Multiple::isZero() const noexcept
{
    if(m_arrUnits.empty() && !m_sg && !m_vec)
        return true;
    if(m_sg && m_sg->isZero())
        return true;
    return false;
}
void Unit::Multiple::setZero() noexcept
{
    m_arrUnits.clear();
    m_sg.reset();
    m_vec.reset();
}
void Unit::Multiple::copy_arrunits(std::list<  sum_queue   >  const& other)
{
	m_arrUnits.clear();
    copy_arrunitsh(other,m_arrUnits);
}
template<typename T>
static void swapo(std::optional<T> & f, std::optional<T> & s)
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
void Unit::Multiple::swap(Multiple & other) noexcept
{
    swapo(m_sg, other.m_sg);
    swapo(m_vec, other.m_vec);
    m_arrUnits.swap(other.m_arrUnits);
}
void Unit::Multiple::setOne()
{
	m_arrUnits.clear();
    m_vec.reset();
    m_sg.emplace(1);
}

Unit::Unit(Unit const& other) :
    m_m(other.m_m),
	m_expanded(other.m_expanded)
{
	m_s = copy_sum_queue(other.m_s);
}
Unit& Unit::operator=(Unit const& other)
{
    Unit u(other);
    return *this = std::move(u);
}
Unit::Unit(Unit&& other) noexcept :
	m_m(std::move(other.m_m)),
	m_s(std::move(other.m_s)),
	m_expanded(other.m_expanded)
{

}
Unit& Unit::operator=(Unit&& other) noexcept
{
    swap(other);
	return *this;
}

void Unit::hmta(sum_queue & v)
{

	auto const itb1 = v.begin();
	auto const ite1 = v.end();

	for (auto it1 = itb1; it1 != ite1; ++it1)
	{
		if (*it1)
		{
			auto it2 = it1;
			++it2;
			for (; it2 != ite1; ++it2)
			{
				if (*it2)
				{
						if (Multiple::canbeadded(*(*it1)->m_m, *(*it2)->m_m))
						{
							(*it1)->m_m->m_sg->add((*it2)->m_m->m_sg);
							(*it2).reset(nullptr);
						}
				}
			}
		}
	}

		//clean empty elements
		for (auto it = itb1; it != ite1; )
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
void Unit::hmfe(Unit& u)
{
	if(u.m_m)
		throw std::exception("error1 during the expansion in hmfe method");
	if (u.m_s.empty())
	{
		return;//++
	}
	auto next = ++u.m_s.begin();
	if (next == u.m_s.end())
	{
		//there is only one unit in m_s. Move it to multiple

		Unit& utmp = *u.m_s.front();
		if (utmp.m_m)
		{
			u.m_m = std::move(utmp.m_m);


			u.m_s.clear();//++
		}
		else
			throw std::exception("error2 during the expansion in hmfe method");

	}
}
Unit::sum_queue Unit::copy_sum_queue(sum_queue const& v)
{
	sum_queue vret;
    for(auto const & u:v)
    {
        if(u)
            vret.emplace_back(new Unit(*u));//copy Unit
    }
	return vret;
}

void Unit::multiplyBySum(sum_queue& l)
{
	if (!m_m)
		m_m.reset(new Multiple);
	m_m->m_arrUnits.emplace_back(std::move(l));
	m_expanded = false;
}
void Unit::appendUnit(std::unique_ptr<Unit>& u)
{
	m_s.emplace_back(std::move(u));
	m_expanded = false;
}
void Unit::group(std::wstring const& str)
{
	expand();
	
	//the unit either a simple multiple or the multiple is one and it has a list or terms like 5*a + 2*b etc.

	if (m_s.empty())
		return;//++no need to group


	auto hmv = [&]()
	{
		auto it_end = m_s.end();
		auto it_ret = it_end;
		for (auto it_next = m_s.begin(); it_next != it_end; ++it_next)
		{
			if (*it_next)
			{
				Unit const& u = **it_next;
				if (u.m_m)
				{
					if (u.m_m->m_vec)
					{
						if (u.m_m->m_vec->m_sym == str)
						{
							return it_next;
						}
					}
				}
			}
		}
		return it_ret;
	};
	//list with grouped Units e.g. t^2(u+1) + t(a+u)...
	sum_queue msnew;
	bool work = true;
	while (!m_s.empty() && work)
	{
		auto p = h_fsm(str);
		if (p.first != m_s.end())
		{
			std::unique_ptr<Unit> u = std::move(*p.first);
			auto it_next = p.first;
			++it_next;
			m_s.erase(p.first);

			//a unit like t*(a+b+d)
			std::unique_ptr<Unit> unew(new Unit);
			unew->multiplyByScalar(std::move(*p.second));
			u->m_m->m_sg->m_arrScalar.erase(p.second);
			//fill ms with units without the common multiple
			sum_queue ms;
			ms.emplace_back(std::move(u));

			auto it_end = m_s.end();
			while( it_next != it_end )
			{
				auto it_tmp = it_next;
				++it_tmp;
				Unit& u = **it_next;
				if (u.m_m)
				{
					auto it_end2 = u.m_m->m_sg->m_arrScalar.end();
					auto it_found = std::find(u.m_m->m_sg->m_arrScalar.begin(), it_end2, unew->m_m->m_sg->m_arrScalar.front());
					if (it_found != it_end2)
					{
						u.m_m->m_sg->m_arrScalar.erase(it_found);
						ms.emplace_back(std::move(*it_next));
						m_s.erase(it_next);
					}
				}


				it_next = it_tmp;
			}

			unew->multiplyBySum(ms);
			msnew.emplace_back(std::move(unew));
		}
		else
		{
			//find if a vector has the same symbolic name
			auto pv = hmv();
			if (pv != m_s.end())
			{
				//a unit e.g. v(a+b+c*d) we will append to msnew
				std::unique_ptr<Unit> unew(new Unit);
				std::unique_ptr<Unit>& u = *pv;
				Vec& v = *u->m_m->m_vec;
				unew->m_m.reset(new Multiple);
				unew->m_m->m_vec = std::move(u->m_m->m_vec);
				auto it_next = pv;
				++it_next;
				sum_queue sum;
				sum.emplace_back(std::move(u));
				m_s.erase(pv);

				auto end = m_s.end();
				while (it_next != end)
				{
					auto it_tmp = it_next;
					++it_tmp;
					Unit& unext = **it_next;
					if (unext.m_m)
					{
						if (unext.m_m->m_vec)
						{
							if (*unext.m_m->m_vec == *unew->m_m->m_vec)
							{
								unext.m_m->m_vec.reset(nullptr);
								sum.emplace_back(std::move(*it_next));
								m_s.erase(it_next);
							}
						}
					}
					it_next = it_tmp;
				}
				unew->multiplyBySum(sum);
				msnew.emplace_back(std::move(unew));
				break;//++exit
			}
			else
				break;//++exit	
		}	
	}
	if (msnew.empty())
		return;//++end
	m_expanded = false;
	m_s.splice(m_s.begin(), msnew);//++end
}
std::wostream& operator<<(std::wostream& out, Unit const& u)
{
	//return out<<L"test";

	//print multiple first

	//return out << *u.m_m.m_sg;



	if (u.m_m && u.m_m->isZero())
	{
		out << 0;
		return out;//++exit
	}

	if (u.m_m)
	{
		bool bprint_m_before_v = false;
		if (u.m_m->m_sg)
		{
			//--decide print multiple or not

			if (u.m_m->m_sg->isMinusOne())
			{
				if (u.m_s.empty() && !u.m_m->m_vec && u.m_m->m_arrUnits.empty())
				{
					//prints -1
					out << *u.m_m->m_sg;
				}
				else
				{
					out << std::wstring{ L"-" };
				}
			}
			else if (u.m_m->m_sg->isOne())
			{
				if (u.m_s.empty() && !u.m_m->m_vec && u.m_m->m_arrUnits.empty())
				{
					//prints 1
					out << *u.m_m->m_sg;
				}
			}
			else
			{
				bprint_m_before_v = true;
				out << *u.m_m->m_sg;
			}
		}
		if (u.m_m->m_vec)
		{
			if (bprint_m_before_v)
			{
				out << std::wstring{ L"*" };
			}
			out << *u.m_m->m_vec;
		}
		//print all multiples with brackets, e.g. (7*r +1)*(8u-i)
		for (auto const& sum : u.m_m->m_arrUnits)
		{
			if (!sum.empty())
			{
				out << std::wstring{ L"(" };

				//print the first unit
				if (*sum.begin())
				{
					Unit const& u1 = **sum.begin();
					out << u1;
					//print others
					auto end = sum.end();
					for (auto it = ++sum.begin(); it != end; ++it)
					{
						if (*it)
						{
							if ((*it)->m_m && (*it)->m_m->isZero())
								continue;
							if ((*it)->m_m && (*it)->m_m->m_sg)
								if ((*it)->m_m->m_sg->multiple() > 0)
								{
									out << std::wstring{ L"+" };
								}
							Unit const& utmp = **it;
							out << utmp;
						}
					}
					out << std::wstring{ L")" };
				}
			}
		}
	}
	//--

	//print the sum of multiples. Wrap it in brackets if a multiple was printed

	if (!u.m_s.empty())
	{
		if (u.m_m && !u.m_m->isOne())
		{
			out << std::wstring{ L"(" };
		}
		if (*u.m_s.begin())
		{
			Unit const& ubref = **u.m_s.begin();
			out << ubref;
			auto end = u.m_s.end();
			for (auto it = ++u.m_s.begin(); it != end; ++it)
			{
				if (*it)
				{
					if ((*it)->m_m && (*it)->m_m->m_sg)
					{
						if ((*it)->m_m->isZero())
						{
							continue;
						}
						else if ((*it)->m_m->m_sg->multiple() > 0)
						{
							out << std::wstring{ L"+" };
						}
					}
					Unit const& utmp = **it;
					out << utmp;
				}
			}
		}

		if (u.m_m && !u.m_m->isOne())
		{
			out << std::wstring{ L")" };
		}
	}
	return out;
}

std::wostream& operator<<(std::wostream& out, ScalarGroup const& u)
{
	bool was1 = false;
	bool was_m1 = false;
	if (abs(u.m_multiple + 1) < Settings::tolerance)
	{
		was_m1 = true;
		//if it is "-1"
		if (u.m_arrScalar.empty() && u.m_arrVecdotted.empty())
			out << std::wstring{ L"-1" };
		else
			out << std::wstring{ L"-" };
	}
	else
	{
		if (abs(u.m_multiple - 1) < Settings::tolerance)
		{
			was1 = true;
			//if multiple is 1
			if (u.m_arrScalar.empty() && u.m_arrVecdotted.empty())
			{
				out << u.m_multiple;
			}
		}
		else
		{
			out << u.m_multiple;
		}
	}
	if (!u.m_arrScalar.empty())
	{
		auto const& elb = u.m_arrScalar.front();
		if (!was1 && !was_m1)
		{
			out << std::wstring{ L"*" };
		}
		out << elb;
		auto end = u.m_arrScalar.end();
		for (auto it = ++u.m_arrScalar.begin(); it!=end; ++it)
		{
			out<< std::wstring{ L"*" } << *it;
		}
	}
	if (!u.m_arrVecdotted.empty())
	{
		//print "*" or no
		if (u.m_arrScalar.empty())
		{
			if (!was1 && !was_m1)
			{
				out << std::wstring{ L"*" };
			}
		}
		else
		{
			out << std::wstring{ L"*" };
		}
		auto const& elb = u.m_arrVecdotted.front();
		out << elb;
		auto end = u.m_arrVecdotted.end();
		for (auto it = ++u.m_arrVecdotted.begin(); it != end; ++it)
		{
			out << std::wstring{L"*"} << *it;
		}
	}
	return out;
}

void Unit::setZero()
{
	m_s.clear();
	if (m_m)
	{
		m_m->setZero();
	}
	else
	{
		m_m.reset(new Multiple(0));
	}
}
void Unit::swap(Unit & other) noexcept
{
    swapo(m_m, other.m_m);
    m_s.swap(other.m_s);
    std::swap(m_expanded, other.m_expanded);
}
