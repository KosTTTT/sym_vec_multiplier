cmake_minimum_required (VERSION 2.6)
project (svmult)
set(CMAKE_CXX_STANDARD 17)#the minimum requred version of C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)#otherwise the error occures


set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                "Debug" "Release" "RelWithDebInfo" "MinSizeRel" "")



add_executable (svmultexe
main.cpp
#Local_vars.cpp
Parser.cpp
PrimitiveTypes.cpp
PrimitiveTypes2.cpp
Local_vars.h
Parser.h
PrimitiveTypes.h
PrimitiveTypes2.h
#Settings.h
Fraction.h
Fraction.cpp
)


