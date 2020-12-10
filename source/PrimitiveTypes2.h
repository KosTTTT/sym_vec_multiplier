#ifndef PT2_H
#define PT2_H
#include "PrimitiveTypes.h"
#include <memory>
#include <utility>
#include <algorithm>
#include <list>
#include "Settings.h"
#include <initializer_list>
#include <iosfwd>
#include <cmath>
#include <optional>




/*! Scalars multiplied together. It might be a single scalar also.*/
class ScalarGroup
{
	friend class Unit;
        friend std::ostream& operator<<(std::ostream& out, ScalarGroup const& u);

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
    void setZero() noexcept;
    Settings::type_real multiple() const noexcept
	{
		return m_multiple;
	}
    bool isZero() const noexcept
	{
        return std::abs(m_multiple) < Settings::scalar_tol;
	}
    std::vector<Scalar> const& arrScalar() const noexcept
	{
		return m_arrScalar;
	}
    std::vector<Vecdotted> const& arrVecDotted() const noexcept
	{
		return m_arrVecdotted;
	}
    /*! Assign a multiple, replacing existing value*/
    void AssignMultipe(Settings::type_real m);
    /*! Multiply by a Scalar*/
    void MultByScalar(Scalar const & sc);
    /*! Append a dot product of two vectors.*/

    template<typename ...Params>
    void MultByVecdotted(Params && ...params)
	{
        m_arrVecdotted.emplace_back(std::forward<Params>(params)...);
	}
    /*! Returns true if ScalarGroup equals 1*/
    bool isOne() const noexcept
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && std::abs(m_multiple-1) < Settings::scalar_tol;
	}
    bool isMinusOne() const noexcept
	{
        return m_arrScalar.empty() && m_arrVecdotted.empty() && std::abs(m_multiple +1)< Settings::scalar_tol;
	}
    void swap(ScalarGroup & other) noexcept;
};

/*! This class can represent a whole expression, e.g. 5*(a*b*(5*r + 3)*(1+7*a) + a*r)
*/
class Unit
{

public:
    using sum_queue = std::list<std::unique_ptr<Unit>>;
private:
    /*multiple of the gobal sum of units */
    class Multiple
    {
        friend class Unit;
        std::optional<ScalarGroup> m_sg;//scalar multiple
        std::optional<Vec> m_vec;//vector multiple

        /*
        e.g. (5*a + r)*(3*u)
        where (5*a + r) is the first element of the list
        (3*u) is the second
        */
        std::list<  sum_queue   > m_arrUnits;

        friend std::ostream& operator<<(std::ostream& out, Unit const& u);
    public:
        Multiple(){}
        Multiple(Settings::type_real v) :
            m_sg(v)
        {}
        explicit Multiple(std::string const& vec) :
            m_vec(vec)
        {}
        explicit Multiple(Vec const& vec) :
            m_vec(vec)
        {}
        Multiple(std::list<  sum_queue   > const& list)
        {
            copy_arrunitsh(list,m_arrUnits);
        }
        Multiple(Multiple const& other);
        Multiple& operator=(Multiple const& other);
        Multiple(Multiple&& other) noexcept;
        Multiple& operator=(Multiple&& other) noexcept;
        /*returns true is this multiple is zero. So everything that multiplies on it will be zero*/
        bool isZero() const;
        void setZero() noexcept;
        /*returns true if multiple is one*/
        bool isOne() const noexcept;
        bool isMinusOne() const noexcept;

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

        void copy_arrunits(std::list<  sum_queue   >  const& other);
        static void copy_arrunitsh(std::list<  sum_queue   >  const& input, std::list<  sum_queue   > & output);

    };

    friend std::ostream& operator<<(std::ostream& out, Unit const& u);


    /*Multiple of m_s*/
    std::optional<Multiple> m_m;
    /*Summ of Units inside this Unit*/
    sum_queue m_s;
    /*true if the unit is expanded, e.g. 2(a + 3) is not expanded but 2a + 6 is.*/
    bool m_expanded = false;




public:
	Unit()
	{}
    Unit(Settings::type_real v):
        m_m(v)
	{}
	Unit(Unit const& other);
	Unit& operator=(Unit const & other);
	Unit(Unit && other) noexcept;
	Unit& operator=(Unit && other) noexcept;

    /*! Multiply the Unit by Vec + */
	template <typename T>
	void multiplyByVec(T&& vec)
	{
		if (!m_m)
		{
            m_m.emplace(std::forward<T>(vec));
		}
		else
		{
            Multiple mtmp{std::forward<T>(vec)};
            m_m->multiply(mtmp);
		}
	}
	/*! 
    Multiplies everything together so that there will be either a simple multiple( without parentheses )
    or sum of Units each of which has only simple multiples.
    If a Unit is expanded it has either only simple multiple or it is without multiple but with sum of expanded Units .
     */
	void expand();
    /*! Adds parameter to this Unit and expands it.
     */
    void sum(Unit const & u);
    /*! Multiplies this Unit to argument. Argument and this Unit will be expanded.
     */
    void multiply(Unit const & u);
    /*! Multiply this Unit by an array of Unit sums.The unit becomes not expanded.
     */
    void multiplyBySum(sum_queue const & l);
    void multiplyBySum(sum_queue && l);

    /*!  */
    void multiplyByScalar(Scalar const& arg);
    /*!  */
    void multiplyByNumber(Settings::type_real const & arg);
    /*!Appends the Unit to be in the list of the sum of Units. The unit becomes not expanded.
    */
    void appendUnit(Unit const& u);
    void appendUnit(Unit && u);
	/*!
    Group Units together if they have common multiple symbol str.
    E.g. if Unit is x*t + a*t ,
	after call with argument "t"
	u becomes t(x+a).
	If a symbol with str will not be found. The method will just expand the Unit and return
    */
    void group(std::string const & str);
    /*! Returns true, if a Unit has compounds expression in its multiple. E.g. if a Unit is 5*(t+3) method returns true. If 7*u*b - returns false*/
    inline bool has_parenthesis_m() const
    {
        return m_m && !m_m->m_arrUnits.empty();
    }

    void swap(Unit & other) noexcept;

private:
    /*sets multiple of this object to zero and clears everything else
     */
	void setZero();


	/*all elements of v must be with only one simple mutiple
	tries to add everything together,if a unit can be added it is removed and is added to another Unit in the array, 
    */
	static void hmta(sum_queue &v);
    /*If an Unit has a single term it is moved to a multiple. It is assumed that the parameter is expanded
    */
	static void hmfe(Unit & u);
	/*units which sum together*/

    /* Parameter UnitChild must be not null.
    This method expands an argument,
    and returns a list of expanded Units with only one simple multiple.
    An object UnitChild will be destroyed
    */
    static sum_queue expand_move(std::unique_ptr<Unit> & UnitChild);
    /* Turns multiple of this unit to an expanded Unit and returns it. Multiple of this object will be destroyed.
    If the Unit had not multiple returns nullptr
    */
	std::unique_ptr<Unit> moveMultiple();
    /* Turns multiple of this unit to an array of expanded Units. Multiple of this object is destroyed.
    Return array will be empty if the Unit does not have a multiple
    */
	sum_queue moveMultipleq();
    /*  */
    static void append_sum_queue(sum_queue const & source, sum_queue & dest);

    //returns pair <an iterator to the unit from m_s with a maximum power Scalar , an iterator to this Scalar in its array>
	//will return end if nothing was found
    //
    auto h_fsm(std::string const& str)->std::pair<sum_queue::iterator,decltype(ScalarGroup::m_arrScalar)::iterator>;

};
//-
std::ostream& operator<<(std::ostream& out, Unit const& u);
//+
std::ostream& operator<<(std::ostream& out, ScalarGroup const& u);
#endif // PT2_H
