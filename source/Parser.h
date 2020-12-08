#ifndef PARSE_H
#define PARSE_H

#include "Local_vars.h"
#include "PrimitiveTypes2.h"
#include <string>
#include <memory>

extern Local_vars lv;
void handle_input(char const * fileName);
/*! prints Unit from Local variables with the name "uname" to a separate file <uname>.txt. The file is located at the same directory as the project files.*/
void printu(std::string uname, Unit const& u);
namespace
{
//	//flag if input was successfully read
//	extern bool bgood;
//	extern size_t lineCurrent;
//	//input file stream associated with a file
//	extern ifstream isfile;
//	//next character read from the file
//	extern char ch;
//	void createUnit(std::string beg_symbol = std::string());

    //void MultiplyBySymbol(std::string const& symbol, Unit& unit);

    void readUnit(std::unique_ptr<Unit>& u, bool allow_new_line = false);
    /*check if a symbol was defined elsewhere with the same name*/
    bool cvnwd(std::string const& str);
    //void throwTheVariableWasDefinedError(std::string const & str);
}
#endif // PARSE_H
