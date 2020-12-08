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

#endif // PARSE_H
