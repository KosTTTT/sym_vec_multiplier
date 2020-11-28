cmake_minimum_required (VERSION 2.6)
project (svmult)
set(CMAKE_CXX_STANDARD 17)#the minimum requred version of C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)#otherwise the error occures
set (install_path ${CMAKE_CACHEFILE_DIR}//install CACHE PATH "set install path ")



add_executable (svmultexe main.cpp 
Local_vars.cpp
Parser.cpp
PrimitiveTypes.cpp
PrimitiveTypes2.cpp
Settings.cpp)

install(TARGETS svmultexe DESTINATION ${CMAKE_CACHEFILE_DIR}//install)
