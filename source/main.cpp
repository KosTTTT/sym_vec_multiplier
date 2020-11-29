#include "Parser.h"
int main(int    argc, char** argv)
{
#ifdef DEBUG_BUILD
    int k = 0;
#endif
    if(argc==2)
    	handle_input(argv[1]);

    return 0;
}
