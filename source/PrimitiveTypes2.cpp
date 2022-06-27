#include "PrimitiveTypes2.h"
#include <algorithm>
#include <type_traits>
#include <utility>
#include <iostream>
#include <exception>

using namespace std;


void ScalarGroup::multByNumber(Fraction const & v) noexcept
{
    m_multiple*=v;
    if(!m_multiple)
        setZero();
}
void ScalarGroup::MultByScalar(Scalar const& sc)
{
    if(isZero() == false)
    {
        auto itnext = m_arrScalar.begin();
        auto const itend = m_arrScalar.end();
        while(itnext!=itend)
        {
            auto ittmp = itnext;
            ++itnext;
            if(ittmp->sym() == sc.sym())
            {
                ittmp->multiply(sc);
                if(ittmp->isOne())
                    m_arrScalar.erase(ittmp);
                return;//++
            }
        }
        m_arrScalar.push_back(sc);
    }
}
void ScalarGroup::clean_scalar_arr()
{
    auto itnext = m_arrScalar.begin();
    auto itend = m_arrScalar.end();
    while(itnext!=itend)
    {
        auto tmp = itnext;
        ++itnext;
        if(tmp->isOne())
        {
            m_arrScalar.erase(tmp);
        }
    }
}
ScalarGroup& ScalarGroup::multiply(ScalarGroup const& sg)
{
    if(sg.isZero())
    {
        setZero();
        return *this;
    }
    if(isZero())
    {
        return *this;
    }



    m_multiple *= sg.m_multiple;

    //for common Scalar variables increase their power
    //and for new ones move them from sg to this

    for (Scalar const & scnext : sg.m_arrScalar)
    {
        auto end1 = m_arrScalar.end();
        auto it_found = std::find_if(m_arrScalar.begin(), end1,
            [&scnext](Scalar const& next)->bool
            {
                return scnext.sym() == next.sym();
            });
        if(it_found == end1)
            m_arrScalar.push_back(scnext);
        else//found the same symbol variable
            (*it_found).multiply(scnext);
    }
    m_arrVecdotted.insert(m_arrVecdotted.end(), sg.m_arrVecdotted.begin(), sg.m_arrVecdotted.end());
    clean_scalar_arr();
    return *this;
}
void ScalarGroup::setZero() noexcept
{
	m_multiple = 0;
	m_arrScalar.clear();
	m_arrVecdotted.clear();
}
template<typename Contaner>
static bool hf(Contaner const& arthis, Contaner const& arother)
{
    if (arother.size() != arthis.size())
        return false;
    auto const arthis_begin = arthis.begin();
    auto const arthis_end = arthis.end();
    for (auto const & arother_value: arother)
    {
        //if symbol is not found we cannot add them
        auto it_found = std::find(arthis_begin, arthis_end, arother_value);
        if (it_found == arthis_end)
            return false;//--
    }
    return true;//++
};
template<typename Contaner>
static bool hf2(Contaner const& arthis, Contaner const& arother)
{

    if (arother.size() != arthis.size())
        return false;

    auto const tb = arthis.begin();
    auto const te = arthis.end();
    auto const ob = arother.begin();
    auto const oe = arother.end();
    for (auto onext = ob; onext!=oe; ++onext)
    {
        //check if the number of symbol occurrence is the same
        if (std::count(tb, te, *onext) != std::count(ob, oe, *onext))
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
    else
    {
        m_multiple += sg.m_multiple;
        if (!m_multiple)
        {
            setZero();
        }
    }
	return *this;
}
std::pair<bool,std::list<Scalar>::iterator> ScalarGroup::getScalar(std::string const& str)
{
    auto b = m_arrScalar.begin();
    auto e = m_arrScalar.end();
    bool bret = false;
    while(b != e)
    {
        if(b->sym() == str)
        {
            bret = true;
            break;
        }
        ++b;
    }
    return std::make_pair(bret, b);
}
void ScalarGroup::swap(ScalarGroup & other) noexcept
{
    m_multiple.swap(other.m_multiple);
    m_arrScalar.swap(other.m_arrScalar);
    m_arrVecdotted.swap(other.m_arrVecdotted);
}
std::ostream& operator<<(std::ostream& out, ScalarGroup const& u)
{
    if(u.isZero())
    {
        return out<<'0';
    }

    bool was1 = false;
    bool was_m1 = false;
    if (u.m_multiple == -1)
    {
        was_m1 = true;
        if(u.m_arrScalar.empty() && u.m_arrVecdotted.empty())
        {
            return out<<u.m_multiple;
        }
        else
        {
            out<<'-';
        }
    }
    else if (u.m_multiple == 1)
    {
        was1 = true;
        if(u.m_arrScalar.empty() && u.m_arrVecdotted.empty())
        {
            return out<<u.m_multiple;
        }
    }
    else
    {
        out<<u.m_multiple;
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
//--
bool Unit::is_expanded() const noexcept
{
    if(m_s.empty() && m_m.is_expanded())
        return true;//e2++
    if((bool)m_m == false)
    {
        if(m_s.empty() == false)
        {
            size_t i = 0;
            for(auto const & el: m_s)
            {
                ++i;
                if((el.m_s.empty() && el.m_m.is_expanded()) == false)
                    return false;
            }
            if(i > 1)
                return true;//e1++
        }
    }
    return false;
}
void Unit::multiplyByVec(Vec const & vec)
{
    if (!m_m)
    {
        m_m = Multiple{vec};
    }
    else
    {
        m_m.multiply(Multiple{vec});
    }
}
void Unit::multiplyByScalar(Scalar const& arg)
{
    if(m_m)
    {
        Multiple m;
        m.m_sg = ScalarGroup{1};
        m.m_sg->MultByScalar(arg);
        m_m.multiply(m);
    }
    else
    {
        m_m.m_sg = ScalarGroup{1};
        m_m.m_sg->MultByScalar(arg);
    }
}
void Unit::multiplyByNumber(Fraction const & arg)
{
    if(m_m)
    {
        m_m.multiply(Multiple{arg});
    }
    else
    {
        m_m.m_sg = ScalarGroup{arg};
    }
    if(m_m.isZero())
        setZero();
}

void Unit::expand()
{
    //+++base case
    if (is_expanded())
    {
        return;
    }
	//+++++++++++++++++++


	//sum from multiple 
	sum_queue sm = moveMultipleq();

    // multiple is empty.
	//sm stores the multiple or empty


	//expand each term of m_s
	sum_queue ms_expanded;
    for (auto &unit:m_s)
	{
        ms_expanded.splice(ms_expanded.end(), expand_move(unit));
	}
    Unit::hmta(ms_expanded);
	m_s.clear();

#ifndef NDEBUG
    if (ms_expanded.empty() && sm.empty())
        throw std::logic_error{"error: empty Unit"};
#endif

	if (ms_expanded.empty() && !sm.empty())
	{
		m_s = std::move(sm);//+++
	}
	else if (!ms_expanded.empty() && sm.empty())
	{
        m_s = std::move( ms_expanded );//++
	}
	else
	{
        for(auto const & el: sm)
        {
            for(auto const & el3: ms_expanded)
            {
                Unit u;
                u.m_m = el.m_m;
                u.m_m.multiply(el3.m_m);
                m_s.emplace_back(std::move(u));
            }
        }
        Unit::hmta(m_s);
    }
    //m_s has Units in it
    Unit::hmfe(*this);
}
void Unit::clear_multiple()
{
    m_m.m_arrUnits.clear();
    m_m.m_sg.reset();
    m_m.m_vec.reset();
}
void Unit::clear_unit()
{
    clear_multiple();
    m_s.clear();
}
bool Unit::moveMultiple(Unit & u)
{
	if (!m_m)
        return false;

    u.clear_unit();
    if (m_m.isZero())//don't need to open brackets if multiple is zero
	{
        u.setZero();
	}
	//if there are no Unit multipliers in the multiple, e.g. 5*a and not 5*a*(b+1), 
	//no need to open brackets
    else if (m_m.m_arrUnits.empty())
	{
        u.m_m = std::move(m_m);
	}
	else
	{
		//multiple is not zero and is not simple. It might be like
		//...*(...)*(...+...+...)

		//prepare the multiple
        u.m_m.m_vec = std::move(m_m.m_vec);
        u.m_m.m_sg = std::move(m_m.m_sg);
		//we will multiply and add everything to this new u variable. 
        if(!u.m_m.m_vec && !u.m_m.m_sg)
        {
            u.m_m.m_sg.emplace(1);
        }
        for (auto it_lmnext = m_m.m_arrUnits.begin(),it_lmEnd = m_m.m_arrUnits.end(); it_lmnext != it_lmEnd; ++it_lmnext)
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
                    sumsBegin->sum(std::move(*it_next));
                }
                //now multiply *sumsBegin by u

                u.multiply(*sumsBegin);
                (*it_lmnext).clear();
            }
		}
    }

#ifndef NDEBUG
    if(u.is_expanded() == false)
        throw std::logic_error{"a unit is not expanded after multiplication/addition"};
#endif

    clear_multiple();
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

void Unit::sum(Unit const& u)
{
    sum(Unit{u});
}
void Unit::sum(Unit && u)
{
    u.expand();
	expand();


    if (u.m_m.isZero())
	{
		//do nothing 
		return;//++
	}
    else if (m_m.isZero())
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
        if (m_m.canbeadded(u.m_m))
		{
            m_m.add(u.m_m);
            return;//++
		}
		else
		{
            //cannot be added together
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
    Unit u = upar;



    u.expand();
	expand();


    if (m_m.isZero())
	{
		return;//++
	}
    if (u.m_m.isZero())
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
        m_m.multiply(u.m_m);
        //++
	}
	else if (thisempty && !otherempty)
	{
        m_s = std::move(u.m_s);
        for(auto & el: m_s)
        {
            el.m_m.multiply(m_m);
        }
        clear_multiple();
        //++
	}
	else if (!thisempty && otherempty)
	{
        for(auto & u: m_s)
        {
            u.m_m.multiply(u.m_m);
        }
        //++
	}
	else
	{
		//!thisempty && !otherempty

        sum_queue result;

        for(auto const & f: m_s)
        {
            for(auto const & s: u.m_s)
            {
                Unit unew{f};
                unew.m_m.multiply(s.m_m);
                result.emplace_back(std::move(unew));
            }
        }
		m_s = std::move(result);
        Unit::hmta(m_s);
        Unit::hmfe(*this);
        //++
	}
}
bool Unit::Multiple::canbeadded(Multiple const& other) const
{
    if(isZero() || other.isZero())
        return true;

    if(m_arrUnits.empty() && other.m_arrUnits.empty())
    {
        if(!m_vec && !other.m_vec)
        {
            if(m_sg && other.m_sg)
            {
                return m_sg->canBeAdded(*other.m_sg);
            }
        }
        else if(m_vec && other.m_vec)
        {
            if(*m_vec==*other.m_vec)
            {
                if(m_sg && !other.m_sg)
                {
                    return m_sg->canBeAdded(ScalarGroup{1});
                }
                else if(!m_sg && other.m_sg)
                {
                    return other.m_sg->canBeAdded(ScalarGroup{1});
                }
                else if(!m_sg && !other.m_sg)
                    return true;
                else if(m_sg && other.m_sg)
                    return m_sg->canBeAdded(*other.m_sg);
            }
        }
    }
	return false;
}
void Unit::Multiple::add(Multiple const & other)
{
    if(other.isZero())
        return;
    if(isZero())
    {
        *this = other;
        return;
    }

    if(!m_vec && !other.m_vec)
    {
        if(m_sg && other.m_sg)
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
            m_sg = ScalarGroup{1};
        m_sg->MultByVecdotted(Vecdotted{*m_vec, *other.m_vec});
        m_vec.reset();
	}
	else if (!m_vec && other.m_vec)
	{
        m_vec = *other.m_vec;
	}

    m_arrUnits.insert(m_arrUnits.end(), other.m_arrUnits.begin(), other.m_arrUnits.end());

    if(other.m_sg && m_sg)
    {
        m_sg->multiply(*other.m_sg);
    }
    else if(!m_sg && other.m_sg)
    {
        m_sg = *other.m_sg;
    }
}

void Unit::Multiple::setZero()
{
    m_arrUnits.clear();
    m_sg.emplace(0);
    m_vec.reset();
}
void Unit::Multiple::setOne()
{
    m_arrUnits.clear();
    m_vec.reset();
    m_sg.emplace(1);
}

void Unit::Multiple::swap(Multiple & other) noexcept
{
    std::swap(m_sg, other.m_sg);
    std::swap(m_vec, other.m_vec);
    m_arrUnits.swap(other.m_arrUnits);
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
        if((*it1).m_m.isZero() == false)
        {
            auto it2 = it1;
            ++it2;
            for (; it2 != ite1; ++it2)
            {
                if(it2->m_m.isZero() == false)
                {
                    if((*it1).m_m.canbeadded((*it2).m_m))
                    {
                        (*it1).m_m.add((*it2).m_m);
                        (*it2).m_m.setZero();
                        if((*it1).m_m.isZero())
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
            auto ittmp = it1;
            ++it1;
            if ((*ittmp).m_m.isZero())
            {
                v.erase(ittmp);
                --count;
                if(count == 1)
                    break;
            }
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

void Unit::group(std::string const& str)
{
    Unit uthis = *this;
    uthis.expand();

	//the unit either a simple multiple or the multiple is one and it has a list or terms like 5*a + 2*b etc.

    if (uthis.m_s.empty())
		return;//++no need to group


    auto hmv = [&uthis, &str]()
	{
        auto it_end = uthis.m_s.end();
		auto it_ret = it_end;
        for (auto it_next = uthis.m_s.begin(); it_next != it_end; ++it_next)
		{
            Unit const& u = *it_next;
            if (u.m_m.m_vec)
            {
                if (u.m_m.m_vec->sym() == str)
                {
                    return it_next;
                }
            }
		}
		return it_ret;
	};
	//list with grouped Units e.g. t^2(u+1) + t(a+u)...
	sum_queue msnew;
    while (!uthis.m_s.empty())
	{
        auto p = uthis.h_fsm(str);
        if (p.first != uthis.m_s.end())
		{
            Unit u = std::move(*p.first);
			auto it_next = p.first;
			++it_next;
            uthis.m_s.erase(p.first);//can uthis be empty now?

			//a unit like t*(a+b+d)
            Unit unew;
            unew.multiplyByScalar(*p.second);
            u.m_m.m_sg->m_arrScalar.erase(p.second);


			//fill ms with units without the common multiple
			sum_queue ms;
			ms.emplace_back(std::move(u));

            auto it_end = uthis.m_s.end();
			while( it_next != it_end )
			{
				auto it_tmp = it_next;
				++it_tmp;
                    Unit& u = *it_next;
                    if (u.m_m.m_sg.has_value())
                    {
                        auto it_end2 = u.m_m.m_sg->m_arrScalar.end();
                        auto it_found = std::find(u.m_m.m_sg->m_arrScalar.begin(), it_end2, unew.m_m.m_sg->m_arrScalar.front());
                        if (it_found != it_end2)
                        {
                            u.m_m.m_sg->m_arrScalar.erase(it_found);
                            ms.emplace_back(std::move(*it_next));
                            uthis.m_s.erase(it_next);
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
            if (pv != uthis.m_s.end())
			{
				//a unit e.g. v(a+b+c*d) we will append to msnew
                Unit unew;
                Unit& u = *pv;
                Vec& v = *u.m_m.m_vec;
                unew.m_m.m_vec = v;
                u.m_m.m_vec.reset();
                if(!u.m_m.m_sg)
                    u.m_m.m_sg.emplace(1);
				auto it_next = pv;
				++it_next;
				sum_queue sum;
				sum.emplace_back(std::move(u));
                uthis.m_s.erase(pv);

                auto end = uthis.m_s.end();
				while (it_next != end)
				{
					auto it_tmp = it_next;
					++it_tmp;
                        Unit& unext = *it_next;
                        if (unext.m_m.m_vec)
                        {
                            if (unext.m_m.m_vec.value() == unew.m_m.m_vec.value())
                            {
                                unext.m_m.m_vec.reset();
                                if(!unext.m_m.m_sg)
                                    unext.m_m.m_sg.emplace(1);
                                sum.emplace_back(std::move(*it_next));
                                uthis.m_s.erase(it_next);
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
    uthis.m_s.splice(uthis.m_s.begin(), msnew);//++end
    swap(uthis);
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
            //**decide put "+" or not
            if ((*it).m_m.m_sg)
            {
                if ((*it).m_m.m_sg->m_multiple.num() < 0)
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



    if (u.m_m.isZero())
	{
        out << '0';
		return out;//++exit
	}
	if (u.m_m)
	{
		bool bprint_m_before_v = false;
        if (u.m_m.m_sg)
		{
			//--decide print multiple or not

            if (u.m_m.isOne())
			{
                if (u.m_s.empty())
				{
                    out << '1';
                    return out;//++exit
				}
			}
			else
			{
				bprint_m_before_v = true;
                out << *u.m_m.m_sg;
			}
		}
        if (u.m_m.m_vec)
		{
			if (bprint_m_before_v)
			{
                out << '*';
			}
            out << *u.m_m.m_vec;
		}
		//print all multiples with brackets, e.g. (7*r +1)*(8u-i)
        for (auto const& sum : u.m_m.m_arrUnits)
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
        out << '(';
        printsum_h(out,u.m_s);
        out << ')';

	}
	return out;
}



void Unit::setZero()
{
	m_s.clear();
    m_m.setZero();
}
void Unit::swap(Unit & other) noexcept
{
    m_m.swap(other.m_m);
    m_s.swap(other.m_s);
}
auto Unit::h_fsm(std::string const& str) -> std::pair<sum_queue::iterator, std::list<Scalar>::iterator>
{
    auto const it_end = m_s.end();
    auto it_ret1 = it_end;
    std::list<Scalar>::iterator it_ret2;
    Fraction vmax = std::numeric_limits<decltype(declval<Fraction>().num())>::lowest();
    for (auto it_next = m_s.begin(); it_next != it_end; ++it_next)
    {
            Unit & u = *it_next;
            if (u.m_m.m_sg.has_value())
            {
                auto res = u.m_m.m_sg->getScalar(str);
                if(res.first)
                {
                    if (res.second->power() > vmax)
                    {
                        vmax = res.second->power();
                        it_ret1 = it_next;
                        it_ret2 = res.second;
                    }
                }
            }
    }
    return std::make_pair(it_ret1, it_ret2);//++
}
