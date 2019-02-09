cmake_minimum_required(VERSION 3.6)
project(pool)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    cmake-build-debug/CMakeFiles/3.6.2/CompilerIdC/CMakeCCompilerId.c
    cmake-build-debug/CMakeFiles/3.6.2/CompilerIdCXX/CMakeCXXCompilerId.cpp
    cmake-build-debug/CMakeFiles/feature_tests.c
    cmake-build-debug/CMakeFiles/feature_tests.cxx
    absyn.c
    absyn.h
    env.c
    env.h
    errormsg.c
    errormsg.h
        tools/lex.yy.c
    parse.c
    parse.h
    prabsyn.c
    prabsyn.h
    semant.c
    semant.h
    symbol.c
    symbol.h
    table.c
    table.h
        tools/tiger.grm
        tools/tiger.lex
    types.c
    types.h
        tools/util.c
        tools/util.h mipsframe.c escape.h escape.c)

add_executable(pool ${SOURCE_FILES})