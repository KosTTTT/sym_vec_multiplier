#include "Parser.h"
#include <iostream>

int main(int argc, char** argv)
{
    if(argc==2)
    {
        try
        {
            handle_input(argv[1]);


            return 0;//++
        }
        catch(std::string const & str)
        {
            std::cerr<<str;
        }
        catch(std::exception const & er)
        {
            std::cerr<<er.what();
        }
        catch(...)
        {
            std::cerr<<"unknown error";
        }
    }
    else
    {
        std::cerr<<"expected the name of one input file as an argument";
    }
    return 1;
}
