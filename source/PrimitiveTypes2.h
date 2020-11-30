#ifndef PT2_H
#define PT2_H
#include "PrimitiveTypes.h"
#include <memory>
#include <utility>
#include <algorithm>
#include <list>
#include "Settings.h"
#include <initializer_list>
#include <iostream>
#include <cmath>




/*! Scalars multiplied together. It might be a single scalar also.*/
class ScalarGroup
{
	friend class Unit;
		friend std::wostream& operator<<(std::wostream& out, ScalarGroup const& u);

        /*! A number that is multiplied by this ScalarGroup. Default is 1*/
        Settings::type_real m_multiple = 1;
        /*! Array of scalar multiples*/
        std::vector<Scalar> m_arrScalar;
        /*! Array of dot products of vectors*/
        std::vector<Vecdotted> m_arrVecdotted;

public:
	ScalarGroup() {}
	explicit ScalarGroup(Settings::type_real multiple) :
		m_multiple(multiple) {}
	/*! 
	Multiplies ScalarGroup sg with this. 
	@return A reference to this new Scalargroup
	*/
    ScalarGroup& multiply(ScalarGroup const& sg);
	/*! Return true if two ScalarGroups can be added together.*/
    bool canBeAdded(ScalarGroup const & other) const;
	/*!
    Adds ScalarGroup sg with this..
    Befor using this function bool canBeAdded(ScalarGroup const & other) const must be used
	@return A reference to this new Scalargroup
	*/
    ScalarGroup& add(ScalarGroup const & sg);
    /*! Set to zero and clears data members*/
	void setZero();
	inline Settings::type_real multiple() const
	{
		return m_multiple;
	}
	inline bool isZero() const
	{
        return std::abs(m_multiple) < Settings::scalar_tol;
	}
	inline std::vector<Scalar> const& arrScalar() const
	{
		return m_arrScalar;
	}
	inline std::vector<Vecdotted> const& arrVecDotted() const
	{
		return m_arrVecdotted;
	}
    /*! Assign a multiple, replacing existing value*/
    void AssignMultipe(Settings::type_real m);
    /*! Multiply by a Scalar*/
    void MultByScalar(Scalar const sc);
    /*! Append a dot product of two vectors.*/
    inline void MultByVecdotted(Vecdotted const & vd)
	{
        m_arrVecdotted.push_back(vd);
	}
    /*! Returns true if ScalarGroup equals 1*/
	inline bool isOne() const
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && std::abs(m_multiple-1) < Settings::scalar_tol;
	}
	inline bool isMinusOne() const
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && std::abs(m_multiple +1)< Settings::scalar_tol;
	}
};

/*! Class can represent a whole expression, e.g. a*b*(5*r + 3)*(1+7*a) + a*r
Here a*b*(5*r + 3)*(1+7*a) is the first Unit, a*r is the second one of a list of Units.
*/
class Unit
{
public:
	using sum_queue = std::list<std::unique_ptr<Unit>>;
private:
	/*multiple of the gobal sum of units */
	class Multiple
	{
		friend std::wostream& operator<<(std::wostream& out, Unit const& u);
	public:
		Multiple() :
			m_sg(new ScalarGroup())
		{}
		Multiple(Settings::type_real v) :
			m_sg(new ScalarGroup(v))
		{

		}
		Multiple(Multiple const& other);
		Multiple& operator=(Multiple const& other);
		Multiple(Multiple&& other) noexcept;
		Multiple& operator=(Multiple&& other) noexcept;
		/*returns true is this multiple is zero. So everything that multiplies on it will be zero*/
		inline bool isZero() const
		{
			return m_sg->isZero();
		}
		inline void setZero()
		{
			m_sg->setZero();
			m_arrUnits.clear();
			m_vec.reset(nullptr);
		}
		/*returns true if multiple is one*/
		bool isOne() const;
		bool isMinusOne() const;

		/*sets this multiple to one*/
		void setOne();
		/* Returns true if two Multiples can be added together
		Use m_sg->add to add them
		*/
		static bool canbeadded(Multiple const& mthis, Multiple const& mother);
		/*multiplies other to this*/
		void multiply(Multiple const& other);
	private:
		void copy_arrunits(std::list<  sum_queue   >  const& other);
	public:


		//==============================================data members
		std::unique_ptr<ScalarGroup> m_sg;
		std::unique_ptr<Vec> m_vec;

		/*first queue elements are multiples and second ones are terms of the first
		e.g. (5*a + r)*(3*u)
		where (5*a + r) is the first element of the queue, which has queue containng 5*a and r
		(3*u) is the second which has queue containing 3*u
		*/
		std::list<  sum_queue   > m_arrUnits;
	};
	friend std::wostream& operator<<(std::wostream& out, Unit const& u);
public:
	Unit()
	{}
	Unit(Settings::type_real v) :
		m_m(new Multiple(v))
	{}
	Unit(Unit const& other);
	Unit& operator=(Unit const & other);
	Unit(Unit && other) noexcept;
	Unit& operator=(Unit && other) noexcept;

	/*! Multiply the Unit by Vec*/
	template <typename T>
	void multiplyByVec(T&& vec)
	{
		if (!m_m)
		{
			m_m.reset(new Multiple);
			m_m->m_vec.reset(new Vec(std::forward<T>(vec)));
		}
		else
		{
			if (m_m->m_vec)
			{
				//multiply both vectors together (dot product)

				std::unique_ptr<ScalarGroup> s(new ScalarGroup);
				if constexpr (std::is_same_v<std::decay_t<T>, Vec>)
				{
					s->addVecdotted(std::move(*m_m->m_vec), std::forward<T>(vec));
				}
				else
				{
					s->addVecdotted(std::move(*m_m->m_vec), Vec(std::forward<T>(vec)));
				}
				m_m->m_sg->multiply(s);
				m_m->m_vec.reset(nullptr);
			}
			else
			{
				m_m->m_vec.reset(new Vec(std::forward<T>(vec)));
			}
		}
	}
	/*! 
	Multiplies everything together so that there will be either simple multiple without parentheses
	or only global terms without brackets and Multiple will be 1. 
	Those terms will include only ScalarGroups and Vectors.
	Or in case there is only 1 global 
	term after expansion it goes to the Multiple and global terms array will be 0.
	Multiple is also expanded so that there is only ScalarGroups and possibly a Vector
	*/
	void expand();
	/*! Adds argument to this Unit. Argument and this Unit will be expanded. After summation argument will be destroyed.*/
	void sum(std::unique_ptr<Unit> & u);
	/*! Multiplies this Unit to argument. Argument and this Unit will be expanded.*/
	void multiply(std::unique_ptr<Unit> const & u);
	/*! Multiply this Unit by an array of Unit sum. Argument will be moved.The unit becomes not expanded.*/
	void multiplyBySum(sum_queue&l);
	template<typename T>
	void multiplyByScalar(T&& arg)
	{
		std::unique_ptr<ScalarGroup> s(new ScalarGroup);
		s->addScalar(std::forward<T>(arg));
		if (!m_m)
			m_m.reset(new Multiple);
		m_m->m_sg->multiply(s);
		m_expanded = false;
	}
	template<typename T>
	void multiplyByNumber(T&& arg)
	{
		if (!m_m)
		{
			m_m.reset(new Multiple(arg));
		}
		else
		{
			m_m->m_sg->addMultipe(m_m->m_sg->multiple() * arg);
		}
		m_expanded = false;
	}
	/*!Appends Unit argument to the terms of this Unit. The argument will vanish. The unit becomes not expanded.*/
	void appendUnit(std::unique_ptr<Unit> & u);
	/*!
	For each term a unit has group them together if they has common multiple symbol str.
	E.g. if u was x*t + a*t ,
	after call with argument "t"
	u becomes t(x+a).
	If a symbol with str will not be found. The method will just expand the Unit and return
	*/
	void group(std::wstring const & str);
	/*! Returns true, if a Unit has compounds expression in its multiple. E.g. if a Unit is 5*(t+3) method returns true. If 7*u*b - returns false*/
	inline bool has_parenthesis_m() const
	{
		if (m_m && !m_m->m_arrUnits.empty())
			return true;
		return false;
	}


private:
	/*sets multiple of this object to zero and clears everything else*/
	void setZero();
	/*
	both args are expanded.
	*/
	void hm(Unit& empty, Unit& notempty);

	/*all elements of v must be with only one simple mutiple
	tries to add everything together,if a unit can be added it is removed and is added to another Unit in the array, 
	*/
	static void hmta(sum_queue &v);
	/*If an Unit has a single term and no multiple it is moved to multiple*/
	static void hmfe(Unit & u);
	/*units which sum together*/

	/*
	This method expands all the children and multiple of an argument, muliplies or adds everything if needed 
	and returns terms as a list of expanded Units with only one simple multiple.
	An object UnitChild will be destroyed and deallocated
	*/
	static sum_queue expand_move(std::unique_ptr<Unit> & UnitChild);
	/* Turns multiple of this unit to an expanded Unit and returns it. Multiple of this object becomes nullptr.
	If the Unit had not multiple returns nullptr*/
	std::unique_ptr<Unit> moveMultiple();
	/* Turns multiple of this unit to an array of expanded Units with only one simple multiple. Multiple of this object becomes nullptr.
	Return array will be empty if the Unit does not have a multiple*/
	sum_queue moveMultipleq();
	static sum_queue copy_sum_queue(sum_queue const & v);

	//returns an iterator to the unit from m_s with a maximum power Scalar and this Scalar in its array
	//will return end if nothing was found
	auto h_fsm(std::wstring const& str)
	{
		auto const it_end = m_s.end();
		auto it_ret1 = it_end;
		decltype((*m_s.begin())->m_m->m_sg->m_arrScalar.end()) it_ret2;
		Settings::type_real vmax = std::numeric_limits<Settings::type_real>::min();
		for (auto it_next = m_s.begin(); it_next != it_end; ++it_next)
		{
			if (*it_next)
			{
				Unit& u = **it_next;
				if (u.m_m)
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
		}
		return std::make_pair(it_ret1, it_ret2);//++
	};

	///* Move unit to this multiple. It should be done only if unit is expanded.*/
	//void mUtoM(std::unique_ptr<Unit> &unit);


	/*Multiple of Global terms of units or just a term if there are no Global terms*/
	std::unique_ptr<Multiple> m_m;
	/*Global terms of units*/
	sum_queue m_s;
private:
	/*true if the unit is expanded, e.g. 2(a + 3) is not expanded but 2a + 6 is.*/
	bool m_expanded = false;
};

std::wostream& operator<<(std::wostream& out, Unit const& u);

#endif // PT2_H
