cmake_minimum_required (VERSION 2.6)
project (svmult)
set(CMAKE_CXX_STANDARD 17)#the minimum requred version of C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)#otherwise the error occures

option (DEBUG_BUILD "Build with extra debug messages." OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_BUILD ON)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(DEBUG_BUILD)
    add_definitions(-DDEBUG_BUILD)
endif(DEBUG_BUILD)


add_executable (svmultexe main.cpp 
Local_vars.cpp
Parser.cpp
PrimitiveTypes.cpp
PrimitiveTypes2.cpp
Settings.cpp
Local_vars.h
Parser.h
PrimitiveTypes.h
PrimitiveTypes2.h
Settings.h)

