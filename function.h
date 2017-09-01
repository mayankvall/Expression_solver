#include <string.h>
int check(char *s)
{
    if((strcmp(s,"sin")==0)||(strcmp(s,"cos")==0)||(strcmp(s,"tan")==0)||(strcmp(s,"cosec")==0)||(strcmp(s,"sec")==0)||(strcmp(s,"cot")==0)||(strcmp(s,"asin")==0)||(strcmp(s,"acos")==0)||(strcmp(s,"atan")==0)||(strcmp(s,"acosec")==0)||(strcmp(s,"asec")==0)||(strcmp(s,"acot")==0)||(strcmp(s,"sinh")==0)||(strcmp(s,"cosh")==0)||(strcmp(s,"tanh")==0)||(strcmp(s,"cosech")==0)||(strcmp(s,"sech")==0)||(strcmp(s,"coth")==0)||(strcmp(s,"asinh")==0)||(strcmp(s,"acosh")==0)||(strcmp(s,"tanh")==0)||(strcmp(s,"acosech")==0)||(strcmp(s,"asech")==0)||(strcmp(s,"acoth")==0)||(strcmp(s,"log")==0)||(strcmp(s,"e")==0))
    {
        return 1;
    }
    else
    {
        printf("Invalid function '%s'",s);
        exit(0);
    }
}

