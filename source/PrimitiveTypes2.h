#ifndef PT2_H
#define PT2_H
#include "PrimitiveTypes.h"
#include <memory>
#include <utility>
#include <algorithm>
#include <list>
#include <initializer_list>
#include <iosfwd>
#include <cmath>
#include <optional>
#include <vector>




/*! Scalars multiplied together. It might be a single scalar also.*/
class ScalarGroup
{

public:
    /*! A number that is multiplied by this ScalarGroup. Default is 0*/
    Fraction m_multiple;
    /*! Array of scalar multiples*/
    std::list<Scalar> m_arrScalar;
    /*! Array of dot products of vectors*/
    std::list<Vecdotted> m_arrVecdotted;


	ScalarGroup() {}
    ScalarGroup(Fraction const & multiple) :
        m_multiple(multiple)
    {}
	/*! 
	Multiplies ScalarGroup sg with this. 
	*/
    ScalarGroup& multiply(ScalarGroup const& sg);

	/*! Return true if two ScalarGroups can be added together.*/
    bool canBeAdded(ScalarGroup const & other) const;
	/*!
    Adds ScalarGroup sg with this.
    Before using this function bool canBeAdded(ScalarGroup const & other) const must be used
	*/
    ScalarGroup& add(ScalarGroup const & sg);

    inline bool isZero() const noexcept
	{
        return !m_multiple;
	}
    void multByNumber(Fraction const & v) noexcept;
    /*! Multiply by a Scalar*/
    void MultByScalar(Scalar const & sc);

    /*! Multiply by Vecdotted.*/
    inline void MultByVecdotted(Vecdotted const & v)
	{
        if(isZero() == false)
            m_arrVecdotted.push_back(v);
	}
    /*! Returns true if ScalarGroup equals 1*/
    inline bool isOne() const noexcept
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && m_multiple == 1;
	}
    inline bool isMinusOne() const noexcept
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && m_multiple == -1;
	}
    std::pair<bool,std::list<Scalar>::iterator> getScalar(std::string const& str);
    void swap(ScalarGroup & other) noexcept;
    friend std::ostream& operator<<(std::ostream& out, ScalarGroup const& u);
private:
    void clean_scalar_arr();
    /*! Set to zero and clear data members*/
    void setZero() noexcept;
};

/*!
 * This class can represent a whole expression, e.g. 5*(a*b*(5*r + 3)*(1+7*a) + a*r)
*/
class Unit
{

public:
    using sum_queue = std::list<Unit>;
    using sum_product = std::list<sum_queue>;
private:
    /*multiple of the gobal sum of units */
    class Multiple
    {
        friend class Unit;
        friend std::ostream& operator<<(std::ostream& out, Unit const& u);
        std::optional<ScalarGroup> m_sg;//scalar multiple
        std::optional<Vec> m_vec;//vector multiple

        /*
        e.g. (5*a + r)*(3*u)
        where (5*a + r) is the first element of the list
        (3*u) is the second
        */
        sum_product m_arrUnits;


    public:
        inline Multiple(){}
        inline Multiple(Fraction const &v) :
            m_sg(v)
        {}
        inline Multiple(Vec const& vec) :
            m_vec(vec)
        {}
        //empty==false
        inline explicit operator bool() const noexcept
        {
            return m_sg.has_value() || m_vec.has_value() || (m_arrUnits.empty() == false);
        }
        //are brackets opened? when empty expanded == false
        inline bool is_expanded() const noexcept
        {
            return (m_sg.has_value() || m_vec.has_value()) && m_arrUnits.empty();
        }

        /*returns true is this multiple is zero. So everything that multiplies on it will be zero*/
        inline bool isZero() const noexcept
        {
            return m_sg.has_value() && m_sg->isZero();
        }
        void setZero();
        /*returns true if multiple is one*/
        bool isOne() const noexcept;
        inline bool isMinusOne() const noexcept
        {
            return m_arrUnits.empty() && !m_vec && m_sg && m_sg->isMinusOne();
        }
        /*sets this multiple to one*/
        void setOne();
        /*!
         * Returns true if two Multiples can be added together
        */
        bool canbeadded(Multiple const& mother) const;
        /*! Call it only if bool canbeadded(Multiple const& mother) const; returns true*/
        void add(Multiple const & other);
        /*multiplies other to this*/
        void multiply(Multiple const& other);
        void swap(Multiple & other) noexcept;
    };




    /*Multiple of m_s*/
    Multiple m_m;
    /*Summ of Units inside this Unit*/
    sum_queue m_s;




public:
    inline Unit()
	{}
    inline Unit(Fraction const & v):
        m_m(v)
	{}

    //empty==false
    inline explicit operator bool() const noexcept
    {
        return (bool)m_m || (m_s.empty()==false);
    }
    /*true if the unit is expanded, e.g. 2(a + 3) is not expanded but 2a + 6 is.
    For an empty Unit if_expanded == false*/
    bool is_expanded() const noexcept;
    /*!
     * Multiply the Unit by Vec
    */
    void multiplyByVec(Vec const & vec);
    /*! Multiply this Unit by an array of Unit sums sum_queue.
     */
    template<typename Sum_queue>
    void multiplyBySum(Sum_queue && l)
    {
        if(m_m)
        {
            Multiple m;
            m.m_arrUnits.emplace_back(std::forward<Sum_queue>(l));
            m_m.multiply(m);
        }
        else
        {
            m_m.m_arrUnits.emplace_back(std::forward<Sum_queue>(l));
        }
    }
    /*!  */
    void multiplyByScalar(Scalar const& arg);
    /*!  */
    void multiplyByNumber(Fraction const & arg);
    /*!
     * Appends the Unit to be in the list of the sum of Units.
    */
    template<typename UnitType>
    void appendUnit(UnitType && u)
    {
        if(isZero() == false)
            m_s.emplace_back(std::forward<UnitType>(u));
    }
    /*!
    Multiplies everything together so that is_expanded() returns true.
    The Unit must not be empty
     */
    void expand();
    /*! Adds parameter to this Unit and expands it.
     */
    void sum(Unit const & u);
    void sum(Unit && u);
    /*! Multiplies this Unit to argument. Argument and this Unit will be expanded.
     */
    void multiply(Unit const & u);
	/*!
    Group Units together if they have common multiple symbol str.
    E.g. if Unit is x*t + a*t ,
	after call with argument "t"
	u becomes t(x+a).
    If a symbol with str will not be found. The method will do nothing
    */
    void group(std::string const & str);
    /*! Returns true, if a Unit has compounds expression in its multiple. E.g. if a Unit is 5*(t+3) method returns true. If 7*u*b - returns false*/
    inline bool has_parenthesis_m() const
    {
        return m_m.m_arrUnits.empty()==false;
    }
    inline bool isZero() const noexcept
    {
        return m_m.isZero();
    }
    void swap(Unit & other) noexcept;
    friend std::ostream& operator<<(std::ostream& out, Unit const& u);
private:
    /*sets multiple of this object to zero and clears everything else
     */
    void setZero();


    /*all elements of v must be with only one simple mutiple.
    The function tries to add everything together.
    */
	static void hmta(sum_queue &v);
    /*If an Unit has a single term it is moved to a multiple. It is assumed that the argument is expanded and has Units in m_s
    */
	static void hmfe(Unit & u);
    /*units which sum together*/

    /* Parameter UnitChild must be not null.
    This method expands an argument,
    and returns a list of expanded Units with only one simple multiple.
    An object UnitChild will be destroyed
    */
    static sum_queue expand_move(Unit & UnitChild);
    /* Turns multiple of this unit to an expanded Unit and returns it. Multiple of this object will be destroyed.
    If the Unit had not multiple returns false
    */
    bool moveMultiple(Unit & ret);
    /* Turns multiple of this unit to an array of expanded Units. Multiple of this object is destroyed.
    Return array will be empty if the Unit does not have a multiple
    */
	sum_queue moveMultipleq();


    //returns pair <an iterator to the unit from m_s with a maximum power Scalar , an iterator to this Scalar in its array>
	//will return end if nothing was found
    //assumed that the unit is expanded and multiple is empty
    //
    auto h_fsm(std::string const& str) ->std::pair<sum_queue::iterator, std::list<Scalar>::iterator>;
    void clear_multiple();
    void clear_unit();
    static bool has_minus(Unit const & u);
    //do we need brackets when u is in multiple like (7*r +1)*u*(8-i)
    static bool needBm(Unit const & u);
    /**
     * @return can 'u' be multiplied without extra parenthesis needed around it, when 'u' is a factor in front of : u*(5+a)*(6+e)
     * E.g. u+u return false -5*(u+u) returns true
     */
    static bool cmwb(Unit const & u);
    bool isOne() const noexcept;
};



#endif // PT2_H
