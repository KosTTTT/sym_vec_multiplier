#include "Parser.h"
int main(int    argc, char** argv)
{
    static_assert (Settings::scalar_tol==1);
    if(argc==2)
    	handle_input(argv[1]);

    return 0;
}
