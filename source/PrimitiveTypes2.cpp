#include "PrimitiveTypes2.h"
#include <algorithm>
#include <type_traits>
#include <utility>
#include <iostream>
#include <exception>

using namespace std;


void Unit::Multiple::append_arrunitsh(sum_product  const& input, sum_product & output)
{
    for(auto const & sum: input)
    {
        sum_queue s;
        for(auto const & u: sum)
        {
            s.emplace_back(u);
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
    else if(sg.isZero())
    {
        setZero();
    }
    else if(isZero() || sg.isOne())
    {
        return *this;
    }
    else if(isMinusOne())
    {
        *this = sg;
        m_multiple = -m_multiple;
    }
    else if(sg.isMinusOne())
    {
        m_multiple = -m_multiple;
    }
    else
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
void ScalarGroup::setZero() noexcept
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
    auto const it_begin = arthis.begin();
    for (size_t i = 0; i < sc_size; ++i)
    {
        //if symbol is not found we cannot add them
        auto it_found = std::find(it_begin, m_arrScalar_end, arother[i]);
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

    auto const tb = arthis.begin();
    auto const te = arthis.end();
    auto const ob = arother.begin();
    auto const oe = arother.end();
    for (size_t i = 0; i < sc_size; ++i)
    {
        //check if the number of symbol occurrence is the same
        if (std::count(tb, te, arother[i])
            != std::count(ob, oe, arother[i]))
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
void ScalarGroup::AssignMultipe(Settings::type_real m)
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

void Unit::multiplyByScalar(Scalar const& arg)
{
    ScalarGroup s;
    s.m_arrScalar.push_back(arg);
    if (!m_m)
    {
        m_m=std::make_optional<Multiple>();
        m_m->m_sg.emplace(s);
    }
    else
    {
        if(m_m->m_sg)
        {
            m_m->m_sg->multiply(s);
        }
        else
        {
            m_m->m_sg.emplace(s);
        }
    }
}
void Unit::multiplyByNumber(Settings::type_real const & arg)
{
    if (!m_m)
    {
        m_m.emplace(arg);
    }
    else
    {
        if(m_m->m_sg)
        {
            ScalarGroup s(arg);
            m_m->m_sg->multiply(s);
        }
        else
        {
            m_m->m_sg.emplace(arg);
        }
    }
}
void Unit::multiplyBySum(sum_queue const& l)
{
    sum_queue lnew;
    append_sum_queue(l, lnew);
    multiplyBySum(std::move(lnew));
}
void Unit::multiplyBySum(sum_queue && l)
{
    if (!m_m)
        m_m=std::make_optional<Multiple>();
    m_m->m_arrUnits.emplace_back(std::move(l));
    m_expanded = false;
}
void Unit::expand()
{
	if (m_expanded)
		return;//++

	//+++handle base case
    if (m_s.empty())
	{
        if(m_m)
        {
            if(m_m->m_arrUnits.empty())
            {
                #ifdef DEBUG_BUILD
                if(!m_m->m_sg && !m_m->m_vec)
                    throw std::logic_error{"Logic error empty multiple."};
                #endif

                m_expanded = true;
                return;//+++
            }
        }
        #ifdef DEBUG_BUILD
        else
        {
            throw std::logic_error{"Logic error empty multiple."};
        }
        #endif
	}
	//+++++++++++++++++++


    //handle not a base case. Either m_s is not empty or m_arrUnits in a multiple

	//sum from multiple 
	sum_queue sm = moveMultipleq();

	// multiple is nullptr.
	//sm stores the multiple or empty




	//expand each term of m_s
	sum_queue ms_expanded;
    for (auto &el:m_s)
	{
        ms_expanded.splice(ms_expanded.end(), expand_move(el));
	}
	m_s.clear();
#ifdef DEBUG_BUILD
	if (ms_expanded.empty() && sm.empty())
        throw std::logic_error{"error during the expansion. ms_expanded.empty() && sm.empty()"};
#endif
	if (ms_expanded.empty() && !sm.empty())
	{
		m_s = std::move(sm);//+++
	}
	else if (!ms_expanded.empty() && sm.empty())
	{
		m_s=std::move( ms_expanded );//++
	}
	else
	{
        for(auto const & el: sm)
        {
                for(auto const & el3: ms_expanded)
                {
                        Unit u;
                        u.m_expanded = true;
                        u.m_m.emplace(*el.m_m);
                        u.m_m->multiply(*el3.m_m);
                        m_s.emplace_back(std::move(u));
                }

        }
	}
	Unit::hmta(m_s);
    m_expanded = true;
    //m_s has Units in it
	Unit::hmfe(*this);
}
bool Unit::moveMultiple(Unit & u)
{
	if (!m_m)
        return false;

#ifdef DEBUG_BUILD
    if(m_m->m_arrUnits.empty() && !m_m->m_sg && !m_m->m_vec)
        throw std::logic_error{"logic error. All elements in a multiple are zero"};
#endif


	if (m_m->isZero())//don't need to open brackets if multiple is zero
	{
        u.m_m.emplace(0);
        u.m_expanded = true;
	}
	//if there are no Unit multipliers in the multiple, e.g. 5*a and not 5*a*(b+1), 
	//no need to open brackets
	else if (m_m->m_arrUnits.empty())
	{
        u.m_m.emplace(std::move(*m_m));
        u.m_expanded = true;
	}
	else
	{

        u.m_m = std::make_optional<Multiple>();
		//multiple is not zero and is not simple. It might be like
		//...*(...)*(...+...+...)

		//prepare the multiple
        u.m_m->m_vec = std::move(m_m->m_vec);
        u.m_m->m_sg = std::move(m_m->m_sg);
		//we will multiply and add everything to this new u variable. 
        if(!u.m_m->m_vec && !u.m_m->m_sg)
        {
            u.m_m->m_sg.emplace(1);
        }
        u.m_expanded = true;
        for (auto it_lmnext = m_m->m_arrUnits.begin(),it_lmEnd = m_m->m_arrUnits.end(); it_lmnext != it_lmEnd; ++it_lmnext)
        {
			//expand each term of *it_lmnext

            //end of the terms
            auto const sumsEnd = (*it_lmnext).end();
            //start of the terms
            auto sumsBegin = (*it_lmnext).begin();
            for (auto it_unext = sumsBegin; it_unext != sumsEnd; ++it_unext)
            {
                    //expand unit
                it_unext->expand();
            }

			//sum them up in the first one
			if (sumsBegin != sumsEnd)
            {
                    for (auto it_next = ++(*it_lmnext).begin(); it_next != sumsEnd; ++it_next)
                    {
                        sumsBegin->sum(*it_next);
                    }
                    //now multiply *sumsBegin by u

                    u.multiply(*sumsBegin);
                    (*it_lmnext).clear();
            }
		}
    }
    m_m.reset();
    return true;
}
Unit::sum_queue Unit::moveMultipleq()
{
	sum_queue sm;
    Unit m;
    bool b =moveMultiple(m);
    if (b)
	{
        if (m.m_s.empty())
		{
            sm.emplace_back(std::move(m));
		}
		else
		{
            sm =std::move(m.m_s);
		}
	}
	return sm;
}

bool Unit::Multiple::isOne() const noexcept
{
    return m_arrUnits.empty() && !m_vec && m_sg && m_sg->isOne();
}
bool Unit::Multiple::isMinusOne() const noexcept
{
    return m_arrUnits.empty() && !m_vec && m_sg && m_sg->isMinusOne();
}
//--
Unit::sum_queue Unit::expand_move(Unit& UnitChild)
{
    sum_queue ret;
    UnitChild.expand();

    if (UnitChild.m_m)
    {
        ret.emplace_back(std::move(UnitChild));
    }
    else
    {
        ret = std::move(UnitChild.m_s);
    }
    return ret;
}

void Unit::sum(Unit const & upar)
{
    Unit u(upar);


    u.expand();
	expand();


    if (u.m_m && u.m_m->isZero())
	{
		//do nothing 
		return;//++
	}
	else if (m_m && m_m->isZero())
	{
		//replace this Unit with another one

        this->swap(u);
		return;//++
	}
	//two units are not zero

	bool const thisempty = m_s.empty();
    bool const otherempty = u.m_s.empty();
	if (thisempty && otherempty)
	{
		//just add multiples together
        if (m_m->addif(*u.m_m))
		{
            return;//++
		}
		else
		{
			//they are terms which cannot be added together
			//move them to the list of terms

            Unit utmp;
            moveMultiple(utmp);
            m_s.emplace_back(std::move(utmp));
			m_s.emplace_back(std::move(u));
			return;// ++
		}
	}
	else if(thisempty && !otherempty)
	{
        Unit utmp;
        moveMultiple(utmp);
        m_s.emplace_back(std::move(utmp));
        m_s.splice(m_s.end(), u.m_s);
        Unit::hmta(m_s);
		Unit::hmfe(*this);
		return;//++
	}
	else if (!thisempty && otherempty)
	{
        m_s.emplace_back(std::move(u));
        Unit::hmta(m_s);
		Unit::hmfe(*this);
		return;//++
	}
	else
	{
		//!thisempty && !otherempty

        //try to add terms from both units together

        m_s.splice(m_s.end(), u.m_s);
        Unit::hmta(m_s);
        Unit::hmfe(*this);
		return;//++
	}
}

void Unit::multiply(Unit const& upar)
{
    Unit u(upar);



    u.expand();
	expand();


	if (m_m && m_m->isZero())
	{
		return;//++
	}
    if (u.m_m && u.m_m->isZero())
	{
		setZero();
		return;//++
	}


    //two units are not zero

	bool const thisempty = m_s.empty();
    bool const otherempty = u.m_s.empty();
	if (thisempty && otherempty)
	{
		//multiply multiples together
        m_m->multiply(*u.m_m);
        //++
	}
	else if (thisempty && !otherempty)
	{
        m_s = std::move(u.m_s);
        for(auto & el: m_s)
        {
            el.m_m->multiply(*m_m);
        }
        m_m.reset();
        //++
	}
	else if (!thisempty && otherempty)
	{
        for(auto & u: m_s)
        {
            u.m_m->multiply(*u.m_m);
        }
        //++
	}
	else
	{
		//!thisempty && !otherempty

        sum_queue result;

        for(auto & f: m_s)
        {
                for(auto & s: u.m_s)
                {
                        Unit unew(f);
                        unew.m_m->multiply(*s.m_m);
                        result.emplace_back(std::move(unew));
                }
        }
		m_s = std::move(result);
		hmta(m_s);
		hmfe(*this);
        //++
	}
}
bool Unit::Multiple::canbeadded(Multiple const& mother) const
{
    if(isZero() || mother.isZero())
        return true;

    if(m_arrUnits.empty() && mother.m_arrUnits.empty())
    {
        if(!m_vec && !mother.m_vec)
        {
            if(m_sg && mother.m_sg)
            {
                return m_sg->canBeAdded(*mother.m_sg);
            }
        }
        else if(m_vec && mother.m_vec)
        {
            if(*m_vec==*mother.m_vec)
            {
                if(m_sg && !mother.m_sg)
                {
                    if(m_sg->m_arrScalar.empty() && m_sg->m_arrVecdotted.empty())
                        return true;//++
                }
                else if(!m_sg && mother.m_sg)
                {
                    if(mother.m_sg->m_arrScalar.empty() && mother.m_sg->m_arrVecdotted.empty())
                        return true;//++
                }
                else if(!m_sg && !mother.m_sg)
                    return true;//++
                else if(m_sg && mother.m_sg)
                    return m_sg->canBeAdded(*mother.m_sg);
            }
        }
    }
	return false;
}
void Unit::Multiple::add(Multiple const & other)
{
    if(isZero())
    {
        if(!other.isZero())
            *this = other;
        return;
    }
    if(other.isZero())
        return;


    if(!m_vec && !other.m_vec)
    {
        //other.m_sg!=null and m_sg!=null
        m_sg->add(*other.m_sg);
    }
    else if(m_vec && other.m_vec)
    {
            if(m_sg && !other.m_sg)
            {
                m_sg->add(ScalarGroup{1});
            }
            else if(!m_sg && other.m_sg)
            {
                m_sg.emplace(1);
                m_sg->add(*other.m_sg);
            }
            else if(!m_sg && !other.m_sg)
                m_sg.emplace(2);//
            else if(m_sg && other.m_sg)
                m_sg->add(*other.m_sg);
    }
    if(m_sg && m_sg->isZero())
        setZero();
}
bool Unit::Multiple::addif(Multiple const & other)
{

    if(isZero())
    {
        if(!other.isZero())
            *this = other;
        return true;
    }
    if(other.isZero())
        return true;



    if(m_arrUnits.empty() && other.m_arrUnits.empty())
    {
        if(!m_vec && !other.m_vec)
        {
            if(m_sg && other.m_sg)
            {
                if(m_sg->canBeAdded(*other.m_sg))
                {
                    m_sg->add(*other.m_sg);
                    if(m_sg->isZero())
                        setZero();
                    return true;//++
                }
            }
        }
        else if(m_vec && other.m_vec)
        {
            if(*m_vec==*other.m_vec)
            {
                if(m_sg && !other.m_sg)
                {
                    if(m_sg->m_arrScalar.empty() && m_sg->m_arrVecdotted.empty())
                    {
                        m_sg->add(ScalarGroup{1});
                        if(m_sg->isZero())
                            setZero();
                        return true;//++
                    }

                }
                else if(!m_sg && other.m_sg)
                {
                    if(other.m_sg->m_arrScalar.empty() && other.m_sg->m_arrVecdotted.empty())
                    {
                        m_sg.emplace(1);
                        m_sg->add(*other.m_sg);
                        if(m_sg->isZero())
                            setZero();
                        return true;//++
                    }
                }
                else if(!m_sg && !other.m_sg)
                {
                    m_sg.emplace(2);
                    return true;//++
                }
                else if(m_sg && other.m_sg)
                {
                    if(m_sg->canBeAdded(*other.m_sg))
                    {
                        m_sg->add(*other.m_sg);
                        if(m_sg->isZero())
                            setZero();
                        return true;//++
                    }
                }
            }
        }
    }

    return false;
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
        m_sg->MultByVecdotted(*m_vec, *other.m_vec);
        m_vec.reset();
	}
	else if (!m_vec && other.m_vec)
	{
        m_vec.emplace(*other.m_vec);
	}

    append_arrunitsh(other.m_arrUnits,m_arrUnits);

    if(other.m_sg && m_sg)
    {
        m_sg->multiply(*other.m_sg);
    }
    else if(!m_sg && other.m_sg)
    {
        m_sg.emplace(*other.m_sg);
    }
//    //can it be zero after a multiplication of two numbers ?
//    if(m_sg && m_sg->isZero())
//        setZero();
}

Unit::Multiple::Multiple(Multiple const& other):
    m_sg(other.m_sg),
    m_vec(other.m_vec)
{
    append_arrunitsh(other.m_arrUnits,m_arrUnits);
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
inline bool Unit::Multiple::isZero() const
{
#ifdef DEBUG_BUILD
    if(m_arrUnits.empty() && !m_vec && !m_sg)
    {
        throw std::logic_error("error in bool Unit::Multiple::isZero() const noexcept \n if(m_arrUnits.empty() && !m_vec && !m_sg)");
    }
#endif


    return m_sg && m_sg->isZero();
}
void Unit::Multiple::setZero() noexcept
{
    m_arrUnits.clear();
    m_sg.emplace(0);
    m_vec.reset();
}
//inline void Unit::Multiple::copy_arrunits(sum_product  const& other)
//{
//	m_arrUnits.clear();
//    append_arrunitsh(other,m_arrUnits);
//}

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
    append_sum_queue(other.m_s,m_s);
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

    //how many Units
    size_t count = 0;
    for (auto it1 = itb1; it1 != ite1; ++it1)
    {
            ++count;
            if(!(*it1).m_m->isZero())
            {
                auto it2 = it1;
                ++it2;
                for (; it2 != ite1; ++it2)
                {
                    if(!it2->m_m->isZero())
                    {
                        if((*it1).m_m->addif(*(*it2).m_m))
                        {
                            (*it2).setZero();
                            if((*it1).m_m->isZero())
                                break;//not going to add zero to something else
                        }
                    }
                }
            }
    }
//remove zeroes

    if(count>1)
    {
        for (auto it1 = itb1; it1 != ite1;)
        {
            if ((*it1).m_m->isZero())
            {
                auto ittmp = it1;
                ++it1;
                v.erase(ittmp);
                --count;
                if(count==1)
                    break;
            }
            else
                ++it1;
        }
    }
}
void Unit::hmfe(Unit& u)
{
    auto next = ++u.m_s.begin();
        if (next == u.m_s.end())
        {
            //there is only one unit in m_s. Move it to multiple

            auto &el = u.m_s.front();
            u.m_m=std::move(el.m_m);
            u.m_s.clear();//++
        }
}
void Unit::append_sum_queue(sum_queue const & source, sum_queue & dest)
{
    for(auto const & u:source)
    {
        dest.emplace_back(u);//copy Unit
    }
}

void Unit::appendUnit(Unit const& u)
{
    m_s.emplace_back(u);
    m_expanded = false;
}
void Unit::appendUnit(Unit && u)
{
    m_s.emplace_back(std::move(u));
    m_expanded = false;
}
void Unit::group(std::string const& str)
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
                Unit const& u = *it_next;
					if (u.m_m->m_vec)
					{
						if (u.m_m->m_vec->m_sym == str)
						{
							return it_next;
						}
					}
		}
		return it_ret;
	};
	//list with grouped Units e.g. t^2(u+1) + t(a+u)...
	sum_queue msnew;
    while (!m_s.empty())
	{
		auto p = h_fsm(str);
		if (p.first != m_s.end())
		{
            Unit u = std::move(*p.first);
			auto it_next = p.first;
			++it_next;
			m_s.erase(p.first);

			//a unit like t*(a+b+d)
            Unit unew;
            unew.multiplyByScalar(*p.second);
            u.m_m->m_sg->m_arrScalar.erase(p.second);
			//fill ms with units without the common multiple
			sum_queue ms;
			ms.emplace_back(std::move(u));

			auto it_end = m_s.end();
			while( it_next != it_end )
			{
				auto it_tmp = it_next;
				++it_tmp;
                    Unit& u = *it_next;
                    if (u.m_m->m_sg)
                    {
                        auto it_end2 = u.m_m->m_sg->m_arrScalar.end();
                        auto it_found = std::find(u.m_m->m_sg->m_arrScalar.begin(), it_end2, unew.m_m->m_sg->m_arrScalar.front());
                        if (it_found != it_end2)
                        {
                            u.m_m->m_sg->m_arrScalar.erase(it_found);
                            ms.emplace_back(std::move(*it_next));
                            m_s.erase(it_next);
                        }
                    }
				it_next = it_tmp;
			}

            unew.multiplyBySum(std::move(ms));
			msnew.emplace_back(std::move(unew));
		}
		else
		{
			//find if a vector has the same symbolic name
			auto pv = hmv();
			if (pv != m_s.end())
			{
				//a unit e.g. v(a+b+c*d) we will append to msnew
                Unit unew;
                Unit& u = *pv;
                Vec& v = *u.m_m->m_vec;
                unew.m_m.emplace(v);
                u.m_m->m_vec.reset();
                if(!u.m_m->m_sg)
                    u.m_m->m_sg.emplace(1);
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
                        Unit& unext = *it_next;
                            if (unext.m_m->m_vec)
                            {
                                if (*unext.m_m->m_vec == *unew.m_m->m_vec)
                                {
                                    unext.m_m->m_vec.reset();
                                    if(!unext.m_m->m_sg)
                                        unext.m_m->m_sg.emplace(1);
                                    sum.emplace_back(std::move(*it_next));
                                    m_s.erase(it_next);
                                }
                            }
					it_next = it_tmp;
				}
                unew.multiplyBySum(sum);
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


std::ostream& operator<<(std::ostream& out, Unit const& u)
{
    auto printsum_h=[](std::ostream& out, Unit::sum_queue const & sum)
    {
        //print the first unit
        auto it_begin = sum.begin();
        out << *it_begin;
            //print others
        for (auto it = ++it_begin, end = sum.end(); it != end; ++it)
        {
                //if ((*it)->m_m && (*it)->m_m->isZero())
                //    continue;

                //**decide put "+" or not
                if ((*it).m_m && (*it).m_m->m_sg)
                {
                    if ((*it).m_m->m_sg->multiple() < 0)
                    {
                        out << *it;
                        continue;
                    }
                }
                //**
                out << '+';
                out << *it;
        }
    };
	//return out<<L"test";

	//print multiple first

	//return out << *u.m_m.m_sg;



	if (u.m_m && u.m_m->isZero())
	{
        out << '0';
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
                    out << '-';
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
                out << '*';
			}
			out << *u.m_m->m_vec;
		}
		//print all multiples with brackets, e.g. (7*r +1)*(8u-i)
		for (auto const& sum : u.m_m->m_arrUnits)
		{
			if (!sum.empty())
			{
                out << '(';

                printsum_h(out,sum);

                out << ')';
			}
		}
	}
	//--

    //print the sum of multiples. Wrap it in parenthesis if Unit has multiple

	if (!u.m_s.empty())
	{
        bool par=u.m_m && !u.m_m->isOne();
        if (par)
		{
            out << '(';
		}
        printsum_h(out,u.m_s);

        if (par)
		{
            out << ')';
		}
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, ScalarGroup const& u)
{
	bool was1 = false;
	bool was_m1 = false;
    if (std::abs(u.m_multiple + 1) < Settings::scalar_tol)
	{
		was_m1 = true;
		//if it is "-1"
		if (u.m_arrScalar.empty() && u.m_arrVecdotted.empty())
            out << "-1";
		else
            out << '-';
	}
	else
	{
        if (std::abs(u.m_multiple - 1) < Settings::scalar_tol)
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
		if (!was1 && !was_m1)
		{
            out << '*' ;
		}
        out << u.m_arrScalar.front();
		auto end = u.m_arrScalar.end();
		for (auto it = ++u.m_arrScalar.begin(); it!=end; ++it)
		{
            out<< '*' << *it;
		}
	}
	if (!u.m_arrVecdotted.empty())
	{
		//print "*" or no
		if (u.m_arrScalar.empty())
		{
			if (!was1 && !was_m1)
			{
                out << '*' ;
			}
		}
		else
		{
            out << '*';
		}
        out << u.m_arrVecdotted.front();
		auto end = u.m_arrVecdotted.end();
		for (auto it = ++u.m_arrVecdotted.begin(); it != end; ++it)
		{
            out << '*' << *it;
		}
	}
	return out;
}

void Unit::setZero()
{
	m_s.clear();
    m_m.emplace(0);
    m_expanded = true;
}
void Unit::swap(Unit & other) noexcept
{
    swapo(m_m, other.m_m);
    m_s.swap(other.m_s);
    std::swap(m_expanded, other.m_expanded);
}
auto Unit::h_fsm(std::string const& str)->std::pair<sum_queue::iterator,decltype(ScalarGroup::m_arrScalar)::iterator>
{
    auto const it_end = m_s.end();
    auto it_ret1 = it_end;
    //decltype((*m_s.begin())->m_m->m_sg->m_arrScalar.end()) it_ret2;
    decltype(ScalarGroup::m_arrScalar)::iterator it_ret2;
    auto vmax = std::numeric_limits<decltype(Scalar::m_power)>::lowest();
    for (auto it_next = m_s.begin(); it_next != it_end; ++it_next)
    {
            Unit& u = *it_next;
            if (u.m_m->m_sg)
            {
                auto const it_end02 = u.m_m->m_sg->m_arrScalar.end();
                for (auto it02 = u.m_m->m_sg->m_arrScalar.begin(); it02 != it_end02; ++it02)
                {
                    Scalar const& sc = *it02;
                    if (sc.m_sym == str)
                    {
                        if (sc.m_power > vmax)
                        {
                            vmax = sc.m_power;
                            it_ret1 = it_next;
                            it_ret2 = it02;
                        }
                    }
                }
            }
    }
    return std::make_pair(it_ret1, it_ret2);//++
};
