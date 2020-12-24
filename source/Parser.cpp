#include "Parser.h"
#include "Local_vars.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <functional>
#include <future>
#include <optional>

using namespace std;


Local_vars lv;




namespace
{
    void readUnit(std::optional<Unit>& u, bool allow_new_line = false);
    /*check if a symbol was defined elsewhere with the same name*/
    bool cvnwd(std::string const& str);
    //void throwTheVariableWasDefinedError(std::string const & str);

}



namespace {
    //flag if input was successfully read
    bool bgood = true;
    size_t lineCurrent=1;
    //input file stream associated with a file
    ifstream isfile;
    //next character read from the file
    char ch;



    inline bool is_plus_minus()
    {
        return 	ch == '+' || ch == '-';
    }
    inline bool is_blank()
    {
        return ch==' ' || ch=='\r' || ch=='\t';
    }
    Settings::type_real get_number()
    {
        Settings::type_real t;
        isfile>>t;
        if(!isfile)
            throw lineCurrent;
        return t;
    }
    void MultiplyBySymbol(std::string const& symbol, Unit& unit)
    {
        auto const it01 = lv.m_lv.find(symbol);
        if (lv.m_ss.find(symbol) != lv.m_ss.end())
        {
            if (lv.m_vs.find(symbol) != lv.m_vs.end())
            {
                //error--
                string str1("The scalar symbol ");
                str1+=symbol;
                str1+=" has already been defined as a vector symbol";
                throw str1;
            }
            else if(it01!=lv.m_lv.end())
            {
                //error--
                string str1("The scalar symbol ");
                str1+=symbol;
                str1+=" has already been defined as a variable";
                throw str1;
            }
            //it is a scalar symbol
            unit.multiplyByScalar(Scalar{symbol});
        }
        else if (lv.m_vs.find(symbol) != lv.m_vs.end())
        {
            if(it01!=lv.m_lv.end())
            {
                //error--
                string str1("The vector symbol ");
                str1+=symbol;
                str1+=" has already been defined as a variable";
                throw str1;
            }
            unit.multiplyByVec(symbol);
        }
        else if (it01 != lv.m_lv.end())
        {
            Unit const& u = (*it01).second;
            //we are trying to multiply by another equation

            //make a copy
            Unit ucopy(u);
            Unit::sum_queue l;
            l.emplace_back(std::move(ucopy));
            unit.multiplyBySum(std::move(l));
        }
        else
        {
            //error--
            string str1("The variable ");
            str1+=symbol;
            str1+=" is unknown\n";
            throw str1;
        }
    }
    std::string generateVName()
    {
        static size_t count = 0;

        while( count < std::numeric_limits<decltype(count)>::max())
        {
            string str("local_var_");
            str += std::to_string(count++);
            if (lv.m_lv.find(str) == lv.m_lv.end())
            {
                return str;
            }
        }
        string strer("Could not create new variable. All names are taken");
        throw strer;//error--
    }
    inline
    bool read_next_char()
    {
        return bgood && isfile.get(ch);
    }
    inline
    bool is_character()
    {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }
    inline bool is_number()
    {
        return ch >= '0' && ch <= '9';
    }
    inline bool is_var_start()
    {
        return is_character();// || ch=='_';
    }
    //look for variable assignment sign. If found return true
    //e.g. "u = "
    //If not found put the character read instead of " = " back
    bool lfva()
    {
        while (read_next_char())
        {
            if (is_blank())
                continue;
            else if (ch == '=')
                return true;//++
            else
            {
                isfile.putback(ch);
                break;
            }
        }
        return false;
    }
    //waits for a Unit start. If found puts the character back. It not, throws lineCurrent
    void wait_any()
    {
        while(read_next_char())
        {
            if(is_plus_minus() || is_var_start() || is_number() ||ch=='('|| is_character())
            {
                isfile.putback(ch);
                return;//++
            }
            else if(is_blank())
                continue;
            else
                break;
        }
        throw lineCurrent;//er--
    }
    /*
    get a variable name from stream. So, the method reads characters from the stream untill there is a whole name of a variable ,e.g. "var_1" of "_var".
    This method must be called only if it is known that there is a variable starting with a legitimate letter comming up.
    It does not read the character after a variable name*/
    std::string get_variable()
    {
        std::string str;

        read_next_char();
        if (is_var_start())//a variable can only start with that
        {
            str.push_back(ch);
        }
        else
            throw lineCurrent;
        while (read_next_char())
        {
            if (is_character())
            {
                str.push_back(ch);
            }
            else if (is_number())
            {
                str.push_back(ch);
            }
            else if (ch == '_')
            {
                str.push_back(ch);
            }
            else
            {
                isfile.putback(ch);
                break;
            }
        }

        return str;
    }
    /*extrants blanks untill a character apperars then  puts it back or throws if no character found*/
    void waitForVarName()
    {
        bool bfound = false;
        while (read_next_char())
        {
            if (is_var_start())
            {
                isfile.putback(ch);
                bfound = true;
                break;//++
            }
            else if (is_blank())
                continue;
            else
                throw lineCurrent;//--
        }
        if(!bfound)
            throw lineCurrent;//--
    }

    void haldlePrintKeyWord(std::string const strUname)
    {

        if (strUname == "all")
        {
            for (auto const& elPair : lv.m_lv)
            {
                printu(elPair.first, elPair.second);
            }
        }
        else
        {
            auto it_found = lv.m_lv.find(strUname);
            if (it_found != lv.m_lv.end())
            {
                printu((*it_found).first, (*it_found).second);
            }
            else
            {
                //error--
                throw std::string("Error in print. Specified unit name not found");
            }
        }
    }
    void handle_group()
    {
        //get the name of a variable we want to group

        waitForVarName();
        auto str_uname = get_variable();
        auto it_found = lv.m_lv.find(str_uname);
        if (it_found == lv.m_lv.end())
            throw lineCurrent;//error--


        //unit is being changed
        Unit& u = (*it_found).second;
        //get the name of a variable we want group
        waitForVarName();
        auto str_group_var = get_variable();
        {
            auto itfound = lv.m_ss.find(str_group_var);
            if(itfound==lv.m_ss.end())
            {
                auto itfound = lv.m_vs.find(str_group_var);
                if(itfound==lv.m_vs.end())
                    throw std::string{"Symbol not found"};
            }
        }
        u.group(str_group_var);
    }
    /*handles a keyword input, or returns false if no such keyword defined*/
    bool handleKeyword(std::string const& str)
    {
        auto hf1 = [](auto const & func)
        {
            waitForVarName();
            auto var = get_variable();
            func(var);

            //read other symbols separated by commas or blanks
            bool comma_was_hit = false;
            while (read_next_char())
            {
                if (ch == ',')
                {
                    comma_was_hit = true;
                }
                else if (ch == '\n')
                {
                    ++lineCurrent;
                    if (!comma_was_hit)
                    {
                        break;//++end
                    }
                }
                else if (is_blank())
                {
                    continue;
                }
                else if (is_character())
                {
                    isfile.putback(ch);
                    auto str = get_variable();
                    func(str);
                    comma_was_hit = false;
                }
                else
                    throw lineCurrent;
            }
            if (comma_was_hit)
                throw lineCurrent;//error--
        };
        if (str == "symv")
        {
            //define a vector variable symbol

            auto f = [](std::string const& str)
            {
                if (cvnwd(str))
                {
                    lv.m_vs.insert(str);
                }
                else
                {
                    throw lineCurrent;
                }
            };
            hf1(f);
            return true;
        }
        else if (str == "syms")
        {
            auto f = [](std::string const& str)
            {
                if (cvnwd(str))
                    lv.m_ss.insert(str);
                else
                    throw lineCurrent;
            };
            hf1(f);
            return true;
        }
        else if (str == "print")
        {
            hf1(haldlePrintKeyWord);
            return true;
        }
        else if (str == "group")
        {
            handle_group();
            return true;
        }
        else if (str == "expand")
        {
            auto f = [](std::string const& str)
            {
                auto it_found = lv.m_lv.find(str);
                if (it_found != lv.m_lv.end())
                {
                        Unit& u = (*it_found).second;
                        u.expand();
                }
                else
                {
                    throw lineCurrent;
                }
            };
            hf1(f);
            return true;
        }
        else if (str == "clear")//erase one, some or all variables from the local variables
        {
            auto f = [](std::string const& str)
            {
                if (str == "all")
                {
                    lv.m_lv.clear();
                    lv.m_ss.clear();
                    lv.m_vs.clear();
                }
                else
                {
                    auto it_found = lv.m_lv.find(str);
                    if (it_found != lv.m_lv.end())
                    {
                        lv.m_lv.erase(it_found);
                    }
                    else
                    {
                        auto it_found = lv.m_vs.find(str);
                        if (it_found != lv.m_vs.end())
                        {
                            lv.m_vs.erase(it_found);
                        }
                        else
                        {
                            auto it_found = lv.m_ss.find(str);
                            if(it_found!=lv.m_ss.end())
                            {
                                lv.m_ss.erase(it_found);
                            }
                        }
                    }
                }
            };
            hf1(f);
            return true;
        }
        return false;
    }
    //reading (5*t + e + 3 - 2*u...) and appending it to the multiple of u
    //where "(" has been already read.
    //u will not be initialized if no normal input could be read
    //throws if no ")" was hit or if no units were read before ")"
    void readSumQueue(std::optional<Unit>& u)
    {
        std::list<Unit> main_queue;
        std::optional<Unit> nextToAppend;
        bool read_all = false;
        while (read_next_char())
        {
            if(ch == ')')
            {
                read_all = true;
                break;//++finish reading
            }
            else if (is_blank())
            {
                continue;
            }
            else if (is_character() || is_number() || ch == '(' )
            {
                isfile.putback(ch);
                readUnit(nextToAppend);
                if (nextToAppend)
                {
                    main_queue.emplace_back(std::move(*nextToAppend));
                    nextToAppend.reset();
                }
                else
                {
                    throw lineCurrent;//error--
                }
            }
            else if (ch == '\n')
            {
                ++lineCurrent;
                continue;
            }
            else if (ch == '+')
            {
                continue;
            }
            else if (ch == '-')
            {
                readUnit(nextToAppend);
                if (nextToAppend)
                {
                    nextToAppend->multiplyByNumber(-1);
                    main_queue.emplace_back(std::move(*nextToAppend));
                    nextToAppend.reset();
                }
                else
                {
                    throw lineCurrent;//error--
                }
            }
            else
            {
                throw lineCurrent;//error--
            }
        }
        if (read_all)
        {
            if(main_queue.empty())
                throw std::string{"empty parenthesis"};//er--
            if (!u)
            {
                u = std::make_optional<Unit>();
            }
            u->multiplyBySum(std::move(main_queue));
            //++
        }
        else
            throw lineCurrent;//error--
    }
    //reads unit into u. U will not be initialized if no unit could be read.
    //u might be already initialized with multiples in it, so the method must be able continue to read unit in this case. U will not be deinitialized in this case.
    //Also if the Unit parameter has already been read with '-' the method continues to read the Unit. It will not read '-' by itself.
    //The method just read a unit without goint to next one, e.g. 2*(1-9*t) + t
    //where t will not be read by this method
    void readUnit(std::optional<Unit>& u, bool allow_new_line)
    {
        bool multiply_was_hit = false;
        while (read_next_char())
        {
            if (ch == '\n')
            {
                if(allow_new_line)
                {
                    ++lineCurrent;//continue reading
                }
                else if (multiply_was_hit)
                {
                    ++lineCurrent;//continue reading
                }
                else
                {
                    isfile.putback(ch);
                    return;//++exit
                }
            }
            else if (is_blank())
            {
                continue;
            }
            else if (is_number())
            {
                isfile.putback(ch);
                auto n = get_number();
                if(!u)
                    u = std::make_optional<Unit>();
                u->multiplyByNumber(n);
            }
            else if (ch == '*')
            {
                if (multiply_was_hit)//we have met two * in a row
                {
                    throw std::string{"Two '*' in a row."};//error--
                }
                if(allow_new_line)
                    throw lineCurrent;//er--
                multiply_was_hit = true;
                continue;
            }
            else if (is_character())
            {
                isfile.putback(ch);
                auto str = get_variable();
                if (!u)
                {
                    u = std::make_optional<Unit>();
                }
                MultiplyBySymbol(str, *u);
            }
            else if (ch == '(')
            {
                readSumQueue(u);
            }
            else if (ch == ')')//in case this method is called from readSumQueue
            {
                isfile.putback(ch);
                break;//exit++
            }
            else if (is_plus_minus())
            {
                isfile.putback(ch);
                break;//exit++
            }
            else
            {
                throw lineCurrent;//error--
            }
            multiply_was_hit = false;
            allow_new_line=false;
        }
        if(multiply_was_hit)
            throw lineCurrent;
    }
    /* Is called from the main loop. Creates and adds Unit with the name UnitName to the local variables.
     * The resulting Unit will be like UnitName =  Unit + Unit +...
    @param UnitName The name which is associated with the unit
    @param beg_symbol Optional parameter. The first symbol of the Unit has already been read.
    */
    void readUnits(std::string const & UnitName, std::string const & beg_symbol = std::string())
    {
        //unit to which we will append unit terms
        //e.g. u*4 + 3*r
        std::optional<Unit> umain;
        std::optional<Unit> u_term;
        if (!beg_symbol.empty())
        {
            u_term = std::make_optional<Unit>();
            MultiplyBySymbol(beg_symbol, *u_term);
            readUnit(u_term);
            umain = std::make_optional<Unit>();
            umain->appendUnit(std::move(*u_term));
            u_term.reset();
        }

        while (read_next_char())
        {
            if (ch == '+')
            {
                readUnit(u_term, true);
                if (u_term)
                {
                    if(!umain)
                        umain = std::make_optional<Unit>();
                    umain->appendUnit(std::move(*u_term));
                    u_term.reset();
                }
                else
                    throw lineCurrent;
            }
            else if (ch == '-')
            {
                readUnit(u_term, true);
                if (u_term)
                {
                    u_term->multiplyByNumber(-1);
                    if(!umain)
                        umain = std::make_optional<Unit>();
                    umain->appendUnit(std::move(*u_term));
                    u_term.reset();
                }
                else
                    throw lineCurrent;
            }
            else if (is_character()||is_number()||ch=='(')
            {
                isfile.putback(ch);
                readUnit(u_term);
                if (u_term)
                {
                    if(!umain)
                        umain = std::make_optional<Unit>();
                    umain->appendUnit(std::move(*u_term));
                    u_term.reset();
                }
                else
                    throw lineCurrent;
            }
            else if (ch == '\n')
            {
                ++lineCurrent;
                break;
            }
            else if (is_blank())
            {
                continue;
            }
            else
            {
                throw lineCurrent;
            }
        }
        if (umain)	//add unit to local variables
        {
            {
                auto it_find = lv.m_ss.find(UnitName);
                if(it_find!=lv.m_ss.end())
                {
                    throw std::string{"The variable already exists as a scalar."};
                }
                else
                {
                    auto it_find = lv.m_vs.find(UnitName);
                    if(it_find!=lv.m_ss.end())
                    {
                        throw std::string{"The variable already exists as a vector."};
                    }
                }
            }

            auto it_find = lv.m_lv.find(UnitName);
            if(it_find != lv.m_lv.end())
                lv.m_lv.erase(it_find);
            lv.m_lv.insert(std::make_pair(UnitName,std::move(*umain)));
        }
        else
            throw lineCurrent;//error--
    }
}
void handle_input(char const* fileName)
{
    isfile.open(fileName);
    if (!isfile.is_open())
        throw std::string("Could not open a file. Maybe file does not exist or file name is incorrect. You need to place a file in the same directory as the program.");

    try
    {
        while (read_next_char())
        {
            if (is_blank())
            {
                continue;
            }
            else if (ch == '\n')
            {
                ++lineCurrent;
                continue;
            }
            else if (ch == '#')
            {
                //skip whole line

                string str;
                std::getline(isfile, str);
                ++lineCurrent;
            }
            else if (is_character())
            {
                //cout << ch;
                isfile.putback(ch);
                //assume user tries to define a variable or use a keyword

                string str = get_variable();
                //check if it is a keyword

                if (handleKeyword(str))
                    continue;
                //it is not a keyword

                //it might be a declaration of variable, e.g. u = ..
                if (lfva())
                {
                    wait_any();
                    //assign Unit to a variable str
                    readUnits(str);
                    continue;
                }
                else
                {
                    //create a variable name and continue to handle the Unit, which now starts with symbol str
                    auto newname = generateVName();
                    readUnits(newname, str);
                }
            }
            else if (is_number() || ch == '(' || ch == '-')
            {
                isfile.putback(ch);
                auto newname = generateVName();
                readUnits(newname);
            }
            else
            {
                throw lineCurrent;
            }
        }
    }
    catch (size_t lineNum)
    {
        string str("Error reading input in line ");
        str += std::to_string(lineNum);
        throw str;
    }
    catch (std::string &er)
    {
        string str("Error reading input in line ");
        str += std::to_string(lineCurrent);
        str+=". ";
        str+=er;
        throw str;
    }
    if (!bgood)
    {
        string str("Error reading input in line ");
        str += std::to_string(lineCurrent);
        throw str;
    }
}
namespace
{
    bool cvnwd(std::string const& str)
    {
        if (lv.m_lv.find(str) == lv.m_lv.end())
            if (lv.m_ss.find(str) == lv.m_ss.end())
                if (lv.m_vs.find(str) == lv.m_vs.end())
                    return true;//++
        return false;
    }
}
void printu(std::string uname, Unit const& u)
{
    uname += ".txt";
    ofstream osfile(uname);
    if (osfile)
    {
        osfile << u;
    }
}
