#include "t_fraction.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include "Fraction.hpp"

static double const told = 1.0e-15;
static float const tolf = 1.0e-7f;

int main(int argc, char*args[])
{
    std::cout<<"Running a Fraction test.\n";
    {
        {
            //test default constructions

            {
                Fraction f;
                if(f.num()!=0)
                {
                    std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f)
                {
                    std::cout<<"Error at if(f).Line "<<__LINE__;
                    return 1;
                }
                if(static_cast<bool>(f))
                {
                    std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;b)
                {
                    std::cout<<"Error at if(b).Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f{};
                if(f.num()!=0)
                {
                    std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f)
                {
                    std::cout<<"Error at if(f).Line "<<__LINE__;
                    return 1;
                }
                if(static_cast<bool>(f))
                {
                    std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;b)
                {
                    std::cout<<"Error at if(b).Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //test not default constructors

            {
                Fraction f(1);
                if(f.num()!=1)
                {
                    std::cout<<"Error at f.num()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(!f)
                {
                    std::cout<<"Error at if(!f).Line "<<__LINE__;
                    return 1;
                }
                if(!static_cast<bool>(f))
                {
                    std::cout<<"Error at if(!static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;!b)
                {
                    std::cout<<"Error at if(!b).Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(0);
                if(f.num()!=0)
                {
                    std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f)
                {
                    std::cout<<"Error at if(f).Line "<<__LINE__;
                    return 1;
                }
                if(static_cast<bool>(f))
                {
                    std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;b)
                {
                    std::cout<<"Error at if(b).Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(0,1);
                if(f.num()!=0)
                {
                    std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f)
                {
                    std::cout<<"Error at if(f).Line "<<__LINE__;
                    return 1;
                }
                if(static_cast<bool>(f))
                {
                    std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;b)
                {
                    std::cout<<"Error at if(b).Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(1,1);
                if(f.num()!=1)
                {
                    std::cout<<"Error at f.num()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(2,3);
                if(f.num()!=2)
                {
                    std::cout<<"Error at f.num()!=2.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=3)
                {
                    std::cout<<"Error at f.den()!=3.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(2,1);
                if(f.num()!=2)
                {
                    std::cout<<"Error at f.num()!=2.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(4,2);
                if(f.num()!=2)
                {
                    std::cout<<"Error at f.num()!=2.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(-1);
                if(f.num()!=-1)
                {
                    std::cout<<"Error at f.num()!=-1.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(!f)
                {
                    std::cout<<"Error at if(!f).Line "<<__LINE__;
                    return 1;
                }
                if(!static_cast<bool>(f))
                {
                    std::cout<<"Error at if(!static_cast<bool>(f)).Line "<<__LINE__;
                    return 1;
                }
                if(bool b = f;!b)
                {
                    std::cout<<"Error at if(!b).Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(-4,2);
                if(f.num()!=-2)
                {
                    std::cout<<"Error at f.num()!=-2.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=1)
                {
                    std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(-2,4);
                if(f.num()!=-1)
                {
                    std::cout<<"Error at f.num()!=-1.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=2)
                {
                    std::cout<<"Error at f.den()!=2.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(17,6);
                if(f.num()!=17)
                {
                    std::cout<<"Error at f.num()!=17.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=6)
                {
                    std::cout<<"Error at f.den()!=6.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(25,100);
                if(f.num()!=1)
                {
                    std::cout<<"Error at f.num()!=1.Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=4)
                {
                    std::cout<<"Error at f.den()!=4.Line "<<__LINE__;
                    return 1;
                }
                Fraction f2(-25,100);
                if(f2.num()!=-1)
                {
                    std::cout<<"Error at f.num()!=-1.Line "<<__LINE__;
                    return 1;
                }
                if(f2.den()!=4)
                {
                    std::cout<<"Error at f.den()!=4.Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction f(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max()-1);
                if(f.num()!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error at f.num()!=std::numeric_limits<int64_t>::max(). Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=std::numeric_limits<int64_t>::max()-1)
                {
                    std::cout<<"Error at f.den()!=std::numeric_limits<int64_t>::max()-1. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                int64_t den = 317;
                Fraction f(std::numeric_limits<int64_t>::lowest(), den);
                if(f.num()!=std::numeric_limits<int64_t>::lowest())
                {
                    std::cout<<"Error Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
                if(f.den()!=den)
                {
                    std::cout<<"Error Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                //test minus and 0 in den

                {
                    Fraction f(25,-100);
                    if(f.num()!=-1)
                    {
                        std::cout<<"Error at f.num()!=-1.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=4)
                    {
                        std::cout<<"Error at f.den()!=4.Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction f(0,-1);
                    if(f.num()!=0)
                    {
                        std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=1)
                    {
                        std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                        return 1;
                    }
                    if(f)
                    {
                        std::cout<<"Error at if(f).Line "<<__LINE__;
                        return 1;
                    }
                    if(static_cast<bool>(f))
                    {
                        std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                        return 1;
                    }
                    if(bool b = f;b)
                    {
                        std::cout<<"Error at if(b).Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction f(-0,1);
                    if(f.num()!=0)
                    {
                        std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=1)
                    {
                        std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                        return 1;
                    }
                    if(f)
                    {
                        std::cout<<"Error at if(f).Line "<<__LINE__;
                        return 1;
                    }
                    if(static_cast<bool>(f))
                    {
                        std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                        return 1;
                    }
                    if(bool b = f;b)
                    {
                        std::cout<<"Error at if(b).Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction f(-0,-1);
                    if(f.num()!=0)
                    {
                        std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=1)
                    {
                        std::cout<<"Error at f.den()!=1.Line "<<__LINE__;
                        return 1;
                    }
                    if(f)
                    {
                        std::cout<<"Error at if(f).Line "<<__LINE__;
                        return 1;
                    }
                    if(static_cast<bool>(f))
                    {
                        std::cout<<"Error at if(static_cast<bool>(f)).Line "<<__LINE__;
                        return 1;
                    }
                    if(bool b = f;b)
                    {
                        std::cout<<"Error at if(b).Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction f(-25,-100);
                    if(f.num()!=1)
                    {
                        std::cout<<"Error at f.num()!=1.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=4)
                    {
                        std::cout<<"Error at f.den()!=4.Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction f(2,-3);
                    if(f.num()!=-2)
                    {
                        std::cout<<"Error in Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=3)
                    {
                        std::cout<<"Error in Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    bool fine = false;
                    try
                    {
                        Fraction{1,0};
                    }
                    catch(std::exception const &e)
                    {
                        fine=true;
                    }
                    if(!fine)
                    {
                        std::cout<<"Error at if(!fine). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    int64_t den = std::numeric_limits<int64_t>::lowest()+1;
                    int64_t num=317;
                    Fraction f(num, den);
                    if(f.num()!=-num)
                    {
                        std::cout<<"Error Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=-den)
                    {
                        std::cout<<"Error Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
            }
            {
                //test floats in constructor

                {
                    Fraction f(0.0);
                    if(f.num()!=0)
                    {
                        std::cout<<"Error at f.num()!=0.Line "<<__LINE__;
                        return 1;
                    }
                    if(f.den()!=1)
                    {
                        std::cout<<"Error at f.den()!=1. Line "<<__LINE__;
                        return 1;
                    }
                    if(f)
                    {
                        std::cout<<"Error at if(f). Line "<<__LINE__;
                        return 1;
                    }
                    if(static_cast<bool>(f))
                    {
                        std::cout<<"Error at if(static_cast<bool>(f)). Line "<<__LINE__;
                        return 1;
                    }
                    if(bool b = f;b)
                    {
                        std::cout<<"Error at if(b). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=0.1;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-0.1;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-3456.134524;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=0.00000001;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=1.00000001;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=0.01091901;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=9999999.99999999;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-9999999.99999999;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=19090900.01091901;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-19090900.01091901;
                    Fraction f(tmp1);
                    double d = f;
                    if(!(std::abs(d-tmp1)<told))
                    {
                        std::cout<<"Error at double constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    float const tmp1=-3456.134f;
                    Fraction f(tmp1);
                    if(!(std::abs(static_cast<float>(f)-tmp1)<tolf))
                    {
                        std::cout<<"Error at float constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
            }
            {
                //test copy/move constructors
                {
                    Fraction fr1(3,4);
                    Fraction fr2(fr1);
                    if(fr1.num()!=fr2.num())
                    {
                        std::cout<<"Error at fr1.num()!=fr2.num(). Line "<<__LINE__;
                        return 1;
                    }
                    if(fr1.den()!=fr2.den())
                    {
                        std::cout<<"Error at fr1.den()!=fr2.den(). Line "<<__LINE__;
                        return 1;
                    }
                    if(fr1!=fr2)
                    {
                        std::cout<<"Error at fr1!=fr2. Line "<<__LINE__;
                        return 1;
                    }
                    if(!(fr1==fr2))
                    {
                        std::cout<<"Error at !(fr1==fr2). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(std::numeric_limits<int64_t>::max(),std::numeric_limits<int64_t>::max()-1);
                    Fraction fr2(fr1);
                    if(fr2.num() != std::numeric_limits<int64_t>::max())
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!=std::numeric_limits<int64_t>::max()-1)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    int64_t num=std::numeric_limits<int64_t>::lowest();
                    int64_t den=3173;
                    Fraction fr1(num, den);
                    Fraction fr2(fr1);
                    if(fr2.num() != num)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!= den)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(3,4);
                    Fraction fr2=fr1;
                    if(fr1.num()!=fr2.num())
                    {
                        std::cout<<"Error at fr1.num()!=fr2.num(). Line "<<__LINE__;
                        return 1;
                    }
                    if(fr1.den()!=fr2.den())
                    {
                        std::cout<<"Error at fr1.den()!=fr2.den(). Line "<<__LINE__;
                        return 1;
                    }
                    if(fr1!=fr2)
                    {
                        std::cout<<"Error at fr1!=fr2. Line "<<__LINE__;
                        return 1;
                    }
                    if(!(fr1==fr2))
                    {
                        std::cout<<"Error at !(fr1==fr2). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(std::numeric_limits<int64_t>::max(),std::numeric_limits<int64_t>::max()-1);
                    Fraction fr2=fr1;
                    if(fr2.num() != std::numeric_limits<int64_t>::max())
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!=std::numeric_limits<int64_t>::max()-1)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    int64_t num=std::numeric_limits<int64_t>::lowest();
                    int64_t den=3173;
                    Fraction fr1(num, den);
                    Fraction fr2=fr1;
                    if(fr2.num() != num)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den() != den)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(3,4);
                    Fraction fr2(std::move(fr1));
                    if(fr2.num()!=3)
                    {
                        std::cout<<"Error at fr2.num()!=3. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!=4)
                    {
                        std::cout<<"Error at fr2.den()!=4. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(3,4);
                    Fraction fr2=std::move(fr1);
                    if(fr2.num()!=3)
                    {
                        std::cout<<"Error at fr2.num()!=3. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!=4)
                    {
                        std::cout<<"Error at fr2.den()!=4. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    Fraction fr1(std::numeric_limits<int64_t>::max(),std::numeric_limits<int64_t>::max()-1);
                    Fraction fr2(std::move(fr1));
                    if(fr2.num() != std::numeric_limits<int64_t>::max())
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!=std::numeric_limits<int64_t>::max()-1)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    int64_t num=std::numeric_limits<int64_t>::lowest();
                    int64_t den=3173;
                    Fraction fr1(num, den);
                    Fraction fr2=std::move(fr1);
                    if(fr2.num() != num)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                    if(fr2.den()!= den)
                    {
                        std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                        return 1;
                    }
                }
            }
        }
        {
            //test operator=

            {
                Fraction fr1;
                Fraction fr2(3,4);
                fr1=fr2;
                if(fr1.num()!=3)
                {
                    std::cout<<"Error at fr1.num()!=3. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=4)
                {
                    std::cout<<"Error at fr1.den()!=4. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1;
                Fraction fr2(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max()-1);
                fr1=fr2;
                if(fr2.num() != std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=std::numeric_limits<int64_t>::max()-1)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                int64_t num=std::numeric_limits<int64_t>::lowest();
                int64_t den=3173;
                Fraction fr1;
                Fraction fr2(num, den);
                fr1=fr2;
                if(fr2.num() != num)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!= den)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1;
                Fraction fr2(3,4);
                fr1=std::move(fr2);
                if(fr1.num()!=3)
                {
                    std::cout<<"Error at fr1.num()!=3. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=4)
                {
                    std::cout<<"Error at fr1.den()!=4. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1;
                Fraction fr2(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max()-1);
                fr1=std::move(fr2);
                if(fr2.num() != std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=std::numeric_limits<int64_t>::max()-1)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                int64_t num=std::numeric_limits<int64_t>::lowest();
                int64_t den=3173;
                Fraction fr1;
                Fraction fr2(num, den);
                fr1=std::move(fr2);
                if(fr2.num() != num)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!= den)
                {
                    std::cout<<"Error at Fraction constructor. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1;
                fr1=3;
                if(fr1.num()!=3)
                {
                    std::cout<<"Error at fr1.num()!=3. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=1)
                {
                    std::cout<<"Error at fr1.den()!=1. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                int64_t const three = 3;
                Fraction fr1;
                fr1=three;
                if(fr1.num()!=three)
                {
                    std::cout<<"Error at fr1.num()!=3. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=1)
                {
                    std::cout<<"Error at fr1.den()!=1. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                double const tmp1=-3456.134524;
                Fraction f;
                f = tmp1;
                if(!(std::abs(static_cast<double>(f)-tmp1)<told))
                {
                    std::cout<<"Error at !(std::abs(static_cast<double>(f)-tmp1)<told). Line "<<__LINE__;
                    return 1;
                }
            }
            {
                float const tmp1=-3456.134f;
                Fraction f;
                f = tmp1;
                if(!(std::abs(static_cast<float>(f)-tmp1)<tolf))
                {
                    std::cout<<"Error at if(!(std::abs(static_cast<float>(f)-tmp1)<tolf)). Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //test conversion operators

            {
                //bool

                {
                    bool b = Fraction{};
                    if(b)
                    {
                        std::cout<<"Error at if(b). Line "<<__LINE__;
                        return 1;
                    }
                    if(Fraction{})
                    {
                        std::cout<<"Error at if(Fraction{}). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    bool b = Fraction(1);
                    if(!b)
                    {
                        std::cout<<"Error at if(!b). Line "<<__LINE__;
                        return 1;
                    }
                    if(!Fraction(1))
                    {
                        std::cout<<"Error at if(!Fraction{1}). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    bool b = Fraction(0);
                    if(b)
                    {
                        std::cout<<"Error at if(b). Line "<<__LINE__;
                        return 1;
                    }
                    if(Fraction(0))
                    {
                        std::cout<<"Error at if(Fraction(0)). Line "<<__LINE__;
                        return 1;
                    }
                }
            }
            {
                //float

                {
                    float const tmp1=-3456.134f;
                    Fraction f(tmp1);
                    float tmp2 = f;
                    if(!(std::abs(tmp2-tmp1)<tolf))
                    {
                        std::cout<<"Error at if(!(std::abs(static_cast<float>(f)-tmp1)<tolf)). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    float const tmp1=-3456.134f;
                    Fraction f(tmp1);
                    if(!(std::abs(static_cast<float>(f)-tmp1)<tolf))
                    {
                        std::cout<<"Error at if(!(std::abs(static_cast<float>(f)-tmp1)<tolf)). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-3456.134;
                    Fraction f(tmp1);
                    double tmp2 = f;
                    if(!(std::abs(tmp2-tmp1)<told))
                    {
                        std::cout<<"Error at if(!(std::abs(static_cast<double>(f)-tmp1)<told)). Line "<<__LINE__;
                        return 1;
                    }
                }
                {
                    double const tmp1=-3456.134;
                    Fraction f(tmp1);
                    if(!(std::abs(static_cast<double>(f)-tmp1)<told))
                    {
                        std::cout<<"Error at if(!(std::abs(static_cast<double>(f)-tmp1)<told)). Line "<<__LINE__;
                        return 1;
                    }
                }
            }
        }
        {
            //negation operator "Fraction operator-()"

            Fraction fr1(3,4);
            Fraction fr2=-fr1;
            if(fr1.num()!=3)
            {
                std::cout<<"Error in Fraction operator-(). Line "<<__LINE__;
                return 1;
            }
            if(fr1.den()!=4)
            {
                std::cout<<"Error in Fraction operator-(). Line "<<__LINE__;
                return 1;
            }
            if(fr2.num()!=-3)
            {
                std::cout<<"Error in Fraction operator-(). Line "<<__LINE__;
                return 1;
            }
            if(fr2.den()!=4)
            {
                std::cout<<"Error in Fraction operator-(). Line "<<__LINE__;
                return 1;
            }
        }
        {
            //bool operator>(Fraction const & other)

            {
                Fraction fr1(1,10);
                Fraction fr2(2,10);
                if(fr2<fr1)
                {
                    std::cout<<"Error in operator>. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(1,11);
                Fraction fr2(1,10);
                if(fr2<fr1)
                {
                    std::cout<<"Error in operator>. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator==

            {
                Fraction fr1(1,10);
                Fraction fr2(1,10);
                if(!(fr1==fr2))
                {
                    std::cout<<"Error in operator==. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(3,10);
                Fraction fr2(3,11);
                if(fr1==fr2)
                {
                    std::cout<<"Error in operator==. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                if(Fraction{3}==2)
                {
                    std::cout<<"Error in operator==. Line "<<__LINE__;
                    return 1;
                }
                if(!(Fraction{std::numeric_limits<int64_t>::max()}==std::numeric_limits<int64_t>::max()))
                {
                    std::cout<<"Error in operator==. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator!=

            {
                Fraction fr1(1,10);
                Fraction fr2(1,10);
                if(fr1!=fr2)
                {
                    std::cout<<"Error in operator!=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(3,10);
                Fraction fr2(3,11);
                if(!(fr1!=fr2))
                {
                    std::cout<<"Error in operator!=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                if(Fraction{2}!=2)
                {
                    std::cout<<"Error in operator!=. Line "<<__LINE__;
                    return 1;
                }
                if(Fraction{3}!=int64_t{3})
                {
                    std::cout<<"Error in operator!=. Line "<<__LINE__;
                    return 1;
                }
            }
            if(Fraction{std::numeric_limits<int64_t>::max()}!=std::numeric_limits<int64_t>::max())
            {
                std::cout<<"Error in operator==. Line "<<__LINE__;
                return 1;
            }
        }
        {
            //operator+=

            {
                Fraction fr1(1,3);
                fr1+=Fraction(1,3);
                if(fr1.num()!=2)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=1;
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=int64_t(1);
                if(fr1.num()!=8)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=int64_t(-1);
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=-1;
                if(fr1.num()!=2)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=Fraction(-1,3);
                if(fr1.num()!=1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=-Fraction(1,3);
                if(fr1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=Fraction(1,2);
                if(fr1.num()!=1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=2)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(std::numeric_limits<int64_t>::max()-1);
                fr1+=int64_t(1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(3,7);
                fr1+=Fraction{4,7};
                if(fr1.num()!=1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1+=-1;
                if(fr1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(1,10);
                fr1+=0.1;
                double d = fr1;
                if(!(std::abs(d-0.2)<told))
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator-=

            {
                Fraction fr1(8,3);
                fr1-=Fraction(1);
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=1;
                if(fr1.num()!=2)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=Fraction(1,3);
                if(fr1.num()!=1)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=Fraction(-1,3);
                if(fr1.num()!=2)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=3)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=-Fraction(-2,3);
                if(fr1)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=Fraction(1,2);
                if(fr1.num()!=-1)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=2)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(std::numeric_limits<int64_t>::lowest()+1);
                fr1-=int64_t(1);
                if(fr1!=std::numeric_limits<int64_t>::lowest())
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(10,7);
                fr1-=Fraction{3,7};
                if(fr1.num()!=1)
                {
                    std::cout<<"Error in operator+=. Line "<<__LINE__;
                    return 1;
                }
                fr1-=1;
                if(fr1)
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(2,10);
                fr1-=0.1;
                double d = fr1;
                if(!(std::abs(d-0.1)<told))
                {
                    std::cout<<"Error in operator-=. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //testing operator+

            {
                Fraction fr1(2,3);
                Fraction fr2(1,2);
                Fraction fr3=fr1+fr2;
                if(fr3.num()!=7)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                if(fr3.den()!=6)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(2,3);
                Fraction fr2=fr1+1;
                if(fr2.num()!=5)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                fr2=1+fr1;
                if(fr2.num()!=5)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                fr2=-1+fr1+1+fr1+2+(-fr1)+(-1);
                if(fr2.num()!=5)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=(std::numeric_limits<int64_t>::max()-1) + Fraction(1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=Fraction(1)+(std::numeric_limits<int64_t>::max()-1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1+0.1;
                if(!(std::abs(static_cast<double>(fr2)-0.2)<told))
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1+fr1;
                if(!(std::abs(static_cast<double>(fr2)-0.2)<told))
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1+0.1f;
                if(!(std::abs(static_cast<float>(fr2)-0.2f)<tolf))
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1f+fr1;
                if(!(std::abs(static_cast<float>(fr2)-0.2f)<tolf))
                {
                    std::cout<<"Error in operator+. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator -

            {
                Fraction fr1(2,3);
                Fraction fr2(1,2);
                Fraction fr3=fr1-fr2;
                if(fr3.num()!=1)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                if(fr3.den()!=6)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(2,3);
                Fraction fr2=fr1-1;
                if(fr2.num()!=-1)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                fr2=1-fr2;
                if(fr2.num()!=4)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                fr2=-1-fr1-(-1)-fr1-2-(-fr1)-(-1);
                if(fr2.num()!=-5)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=(std::numeric_limits<int64_t>::max()-1) - Fraction{-1};
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=Fraction(std::numeric_limits<int64_t>::max()-1)-(-1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(2,10);
                Fraction fr2=fr1-0.1;
                if(!(std::abs(static_cast<double>(fr2)-0.1)<told))
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.2-fr1;
                if(!(std::abs(static_cast<double>(fr2)-0.1)<told))
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(2,10);
                Fraction fr2=fr1-0.1f;
                if(!(std::abs(static_cast<float>(fr2)-0.1f)<tolf))
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.2f-fr1;
                if(!(std::abs(static_cast<float>(fr2)-0.1f)<tolf))
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator*

            {
                Fraction fr1(2,3);
                Fraction fr2(1,2);
                Fraction fr3=fr1*fr2;
                if(fr3.num()!=1)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                if(fr3.den()!=3)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,3);
                Fraction fr2=fr1*3;
                if(fr2.num()!=1)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=1)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                fr2=2*fr1;
                if(fr2.num()!=2)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                fr2=-2*fr1;
                if(fr2.num()!=-2)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
                fr2=2*fr1*(-1);
                if(fr2.num()!=-2)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in operator-. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1 = std::numeric_limits<int64_t>::max() * Fraction(1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=Fraction(std::numeric_limits<int64_t>::max())*1;
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1 * 0.1;
                if(!(std::abs(static_cast<double>(fr2)-0.01)<told))
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1*fr1;
                if(!(std::abs(static_cast<double>(fr2)-0.01)<told))
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1*0.1f;
                if(!(std::abs(static_cast<float>(fr2)-0.01f)<tolf))
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1f*fr1;
                if(!(std::abs(static_cast<float>(fr2)-0.01f)<tolf))
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //operator/

            {
                Fraction fr1(2,3);
                Fraction fr2(1,2);
                Fraction fr3=fr1/fr2;
                if(fr3.num()!=4)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                if(fr3.den()!=3)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,3);
                Fraction fr2=fr1/3;
                if(fr2.num()!=1)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=9)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                fr2=2/fr1;
                if(fr2.num()!=6)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=1)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                fr2=-2/fr1;
                if(fr2.num()!=-6)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=1)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                fr2=2/fr1/(-1);
                if(fr2.num()!=-6)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=1)
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1 = std::numeric_limits<int64_t>::max() / Fraction(1);
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1=Fraction(std::numeric_limits<int64_t>::max())/1;
                if(fr1!=std::numeric_limits<int64_t>::max())
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1 / 0.1;
                if(!(std::abs(static_cast<double>(fr2)-1.0)<told))
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1/fr1;
                if(!(std::abs(static_cast<double>(fr2)-1.0)<told))
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=fr1/0.1f;
                if(!(std::abs(static_cast<float>(fr2)-1.0f)<tolf))
                {
                    std::cout<<"Error in operator/. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(1,10);
                Fraction fr2=0.1f/fr1;
                if(!(std::abs(static_cast<float>(fr2)-1.0f)<tolf))
                {
                    std::cout<<"Error in operator*. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //test swap

            {
                Fraction fr1(2,3);
                Fraction fr2(5,4);
                std::swap(fr1, fr2);
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=4)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.num()!=2)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(2,3);
                Fraction fr2(5,4);
                fr1.swap(fr2);
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=4)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.num()!=2)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction fr1(2,3);
                Fraction fr2(5,4);
                fr2.swap(fr1);
                if(fr1.num()!=5)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr1.den()!=4)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.num()!=2)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
                if(fr2.den()!=3)
                {
                    std::cout<<"Error in swap. Line "<<__LINE__;
                    return 1;
                }
            }
        }
        {
            //test <<

            {
                Fraction const fr1(5);
                std::ostringstream str;
                str<<fr1;
                if(str.str()!="5")
                {
                    std::cout<<"Error in <<. Line "<<__LINE__;
                    return 1;
                }
            }
            {
                Fraction const fr1(5,3);
                std::ostringstream str;
                str<<fr1;
                if(str.str()!="5/3")
                {
                    std::cout<<"Error in <<. Line "<<__LINE__;
                    return 1;
                }
            }
        }
    }
    std::cout<<"+Fraction test passed.\n";
    return 0;
}
