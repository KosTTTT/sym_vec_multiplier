#include "Parser.h"
#include "Local_vars.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;


Local_vars lv;

namespace {
	//flag if input was successfully read
	bool bgood = true;
	size_t lineCurrent=1;
	//input file stream associated with a file
	wifstream isfile;
	//next character read from the file
	wchar_t ch;

	inline bool is_plus_minus()
	{
		return 	ch == L'+' || ch == L'-';
	}
	Settings::type_real get_number()
	{
		Settings::type_real t;
		isfile>>t;
		return t;
	}
	void MultiplyBySymbol(std::wstring const& symbol, Unit& unit)
	{
		if (lv.m_ss.find(symbol) != lv.m_ss.end())
		{
			//it is a scalar symbol
			unit.multiplyByScalar(symbol);
		}
		else if (lv.m_vs.find(symbol) != lv.m_vs.end())
		{
			unit.multiplyByVec(symbol);
		}
		else
		{
			throw lineCurrent;//--error
		}
	}
	std::wstring generateVName()
	{
		static size_t count = 0;

		while( count < std::numeric_limits<decltype(count)>::max())
		{
			wstring str(L"local_var_");
			str += std::to_wstring(count++);
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
		return (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
	} 
	inline bool is_number()
	{
		return ch >= L'0' && ch <= L'9';
	}
	inline bool is_operator()
	{
		return ch == L'+' || ch == L'-' || ch == L'*';
	}

	//look for variable assignment sign. If found return true
	//e.g. "u = "
	//If not found put the character read instead of " = " back
	bool lfva()
	{
		while (read_next_char())
		{
			if (is_character() || is_number() || is_operator() || ch==L'(')
			{
				isfile.putback(ch);
				return false;
			}
			else if (ch == L' ' || ch == L'\r')
				continue;
			else if (ch == L'\n')
			{
				++lineCurrent;
				continue;
			}
			else if (ch == L'=')
				return true;//++
			else
				throw lineCurrent;//error--
		}
		return false;
	}
	/*
	get a variable name from stream. So, the method reads characters from the stream untill there is a whole name of a variable ,e.g. "var_1".
	This method must be called only if it is known that there is a variable starting with a letter comming up.
	It does not read the character after a variable name*/
	std::wstring get_variable()
	{
		std::wstring str;

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
			else if (ch == L'_')
			{
				str.push_back(ch);
			}
			else
			{
				isfile.putback(ch);
				break;
			}
		}
		if (str.empty())
			throw lineCurrent;
		return str;
	}
	/*extrants blanks untill a character apperars then  puts it back or throws if no character found*/
	void waitForVarName()
	{
		while (read_next_char())
		{
			if (is_character())
			{
				isfile.putback(ch);
				return;//++
			}
			else if (ch == L' ')
				continue;
			else
				throw lineCurrent;//--
		}
	}

	void haldlePrintKeyWord()
	{
		waitForVarName();
		auto strUname = get_variable();

		if (strUname == L"all")
		{
			for (auto const& elPair : lv.m_lv)
			{
				printu(elPair.first, *elPair.second);
			}
			return;//exit++
		}
		else
		{
			auto it_found = std::find_if(lv.m_lv.begin(), lv.m_lv.end(),
				[&strUname](auto const& elpair) -> bool
			{
				return strUname==elpair.first;
			});
			if (it_found != lv.m_lv.end())
			{
				printu((*it_found).first, *(*it_found).second);
				//exit++
			}
			else
			{
				//error--
				throw std::exception("Specified unit name not found");
			}
		}
	}

	/*handles a keyword input, or returns false if no such keyword defined*/
	bool handleKeyword(std::wstring const& str)
	{
		auto hf1 = [&](auto& container)
		{
			waitForVarName();
			auto str = get_variable();
			container.insert(str);

			//read other symbols separated by commas or blanks
			bool comma_was_hit = false;
			while (read_next_char())
			{
				if (ch == L',')
				{
					comma_was_hit = true;
				}
				else if (ch == L'\n')
				{
					++lineCurrent;
					if (!comma_was_hit)
					{
						break;//++end
					}
				}
				else if (ch == L'\r' || ch == ' ')
				{
					continue;
				}
				else if (is_character())
				{
					isfile.putback(ch);
					auto str = get_variable();
					container.insert(str);
					comma_was_hit = false;
				}
				else
					throw lineCurrent;
			}
			if (comma_was_hit)
				throw lineCurrent;//error--
		};
		if (str == L"symv")
		{
			//define a vector variable symbol
			hf1(lv.m_vs);
			return true;
		}
		else if (str == L"syms")
		{
			//define a scalar variable symbol
			hf1(lv.m_ss);
			return true;
		}
		else if (str == L"print")
		{
			haldlePrintKeyWord();
			return true;
		}
		return false;
	}
	//reading (5*t + e + 3 - 2*u...) and appending it to the multiple of u
	//where "(" has been already read.
	//u will become null if an error during reading occurs
	void readSumQueue(std::unique_ptr<Unit>& u)
	{
		list<unique_ptr<Unit>> main_queue;
		unique_ptr<Unit>nextToAppend;
		bool read_all = false;
		while (read_next_char())
		{
			if(ch == L')')
			{
				read_all = true;
				break;//++finish reading
			}
			else if (ch == L' ' || ch == L'\r')
			{
				continue;
			}
			else if (is_character() || is_number() || ch==L'(')
			{
				isfile.putback(ch);
				readUnit(nextToAppend);
				if (!nextToAppend)
				{
					u.reset(nullptr);
					return;//error--
				}
				else
				{
					main_queue.emplace_back(std::move(nextToAppend));
				}
			}
			else if (ch == L'\n')
			{
				++lineCurrent;
				continue;
			}
			else if (ch == L'+')
			{
				continue;
			}
			else if (ch == L'-')
			{
				nextToAppend.reset(new Unit(-1));
				continue;
			}
			else
			{
				u.reset(nullptr);
				return;//error--
			}
		}
		if (read_all)
		{
			if (!u)
			{
				u.reset(new Unit);
			}
			u->multiplyBySum(main_queue);
			return;//++
		}
		u.reset(nullptr);//error--
	}
	//reads unit into u. U will not be initialized if no unit could be read.
	//u might be already initialized with multiples in it, so the method must be able continue to read unit in this case
	//The method just read a unit without goint to next one, e.g. 2*(1-9*t) + t
	//where t will not be read by this method, but will be when this method has finished
	void readUnit(std::unique_ptr<Unit>& u, bool allow_new_line)
	{
		bool multiply_was_hit = allow_new_line;
		while (read_next_char())
		{
			if (ch == L'\n')
			{
				if (!multiply_was_hit)
				{
					isfile.putback(ch);
					break;//++exit
				}
				else
					++lineCurrent;//continue reading
			}
			else if (ch == L' ' || ch == L'\r')
			{
				continue;
			}
			else if (is_number())
			{
				isfile.putback(ch);
				auto n = get_number();
				if (!u)
				{
					u.reset(new Unit(n));
				}
				else
				{
					u->multiplyByNumber(n);
				}
			}
			else if (ch == L'*')
			{
				if (multiply_was_hit)//we have met two * in a row
				{
					u.reset(nullptr);
					return;//error--
				}
				multiply_was_hit = true;
				continue;
			}
			else if (is_character())
			{
				isfile.putback(ch);
				auto str = get_variable();
				if (!u)
				{
					u.reset(new Unit);
				}
				MultiplyBySymbol(str, *u);
			}
			else if (ch == L'(')
			{
				readSumQueue(u);
				if (!u)
					return;//error--
			}
			else if (ch == L')')
			{
				isfile.putback(ch);
				break;//++return
			}
			else if (is_plus_minus())
			{
				isfile.putback(ch);
				return;//exit++
			}
			else
			{
				u.reset(nullptr);
				return;//error--
			}
			multiply_was_hit = false;
		}
	}
	/* Is called from the main loop. Creates and adds Unit with the name UnitName to the local variables. 
	The method adds terms to newly created Unit and does not proceed to handle next Units. Next Unit will be handled by this method when called from the main loop again.
	@param UnitName The name which is associated with the unit
	@param beg_symbol Optional parameter. The first symbol of the Unit has already been read.*/
	void readUnits(std::wstring const & UnitName, std::wstring const & beg_symbol = std::wstring())
	{
		//unit to which we will append unit terms
		//e.g. u*4 + 3*r
		unique_ptr<Unit> umain;
		unique_ptr<Unit> u_term;
		if (!beg_symbol.empty())
		{
			u_term.reset(new Unit);
			MultiplyBySymbol(beg_symbol, *u_term);
			readUnit(u_term);
			if (u_term)
			{
				umain.reset(new Unit);
				umain->appendUnit(u_term);
			}
			else
				throw lineCurrent;
		}
		while (read_next_char())
		{
			if (ch == L'+')
			{
				readUnit(u_term, true);
				if (u_term)
				{
					if(!umain)
						umain.reset(new Unit);
					umain->appendUnit(u_term);
				}
				else
					throw lineCurrent;
			}
			else if (ch == L'-')
			{
				u_term.reset(new Unit(-1));
				readUnit(u_term, true);
				if (u_term)
				{
					if (!umain)
						umain.reset(new Unit);
					umain->appendUnit(u_term);
				}
				else
					throw lineCurrent;
			}
			else if (is_character()||is_number()||ch==L'(')
			{
				isfile.putback(ch);
				readUnit(u_term);
				if (u_term)
				{
					if (!umain)
						umain.reset(new Unit);
					umain->appendUnit(u_term);
				}
				else
					throw lineCurrent;
			}
			else if (ch == L'\n')
			{
				++lineCurrent;
				break;
			}
			else if (ch == L' '||ch==L'\r')
			{
				continue;
			}
			else
			{
				isfile.putback(ch);
				break;
			}
		}
		if (umain)	//add unit to local variables
		{
			lv.m_lv.insert(std::make_pair(UnitName,std::move(umain)));
		}
		else
			throw lineCurrent;//error--
	}
}
void handle_input(char const* fileName)
{
	isfile.open(fileName);
	if (!isfile.is_open())
		throw std::exception("Could not open a file. Maybe file does not exist or file name is incorrect. You want to place a file in the same directory as the program.");

	try
	{
		while (read_next_char())
		{
			if (ch == L' ')
			{
				continue;
			}
			else if (ch == L'\r')
				continue;
			else if (ch == L'\n')
			{
				++lineCurrent;
				continue;
			}
			else if (ch == L'#')
			{
				//skip whole line

				wstring str;
				std::getline(isfile, str);
				++lineCurrent;
			}
			else if (is_character())
			{
				//cout << ch;
				isfile.putback(ch);
				//assume user tries to define a variable or use a keyword

				wstring str = get_variable();
				//check if it is a keyword
				if (handleKeyword(str))
					continue;
				//it is not a keyword

				//it might be a declaration of variable, e.g. u = ..
				if (lfva())
				{
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
			else if (is_number() || ch == L'(')
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
		throw exception(str.c_str());
	}
	if (!bgood)
	{
		string str("Error reading input in line ");
		str += std::to_string(lineCurrent);
		throw exception(str.c_str());
	}
}

void printu(std::wstring uname, Unit const& u)
{
	uname += L".txt";
	wofstream osfile(uname);
	if (osfile)
	{
		osfile << u;
	}
}