cmake_minimum_required (VERSION 2.6)
project (svmult)
set(CMAKE_CXX_STANDARD 17)#the minimum requred version of C++
set(CMAKE_CXX_STANDARD_REQUIRED ON)#otherwise the error occures
#set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}//install CACHE PATH "set install path ")
#set (CMAKE_INSTALL_PREFIX ${install_path})
set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}//install)

option (DEBUG_BUILD "Build with extra debug messages." OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_BUILD ON)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if(DEBUG_BUILD)
    add_definitions(-DDEBUG_BUILD)
endif(DEBUG_BUILD)
#configure_file(${PROJECT_SOURCE_DIR}/projConfig.h.in
#                ${PROJECT_BINARY_DIR}/projConfig.h)


add_executable (svmultexe main.cpp 
Local_vars.cpp
Parser.cpp
PrimitiveTypes.cpp
PrimitiveTypes2.cpp
Settings.cpp)

install(TARGETS svmultexe DESTINATION ${CMAKE_INSTALL_PREFIX})
