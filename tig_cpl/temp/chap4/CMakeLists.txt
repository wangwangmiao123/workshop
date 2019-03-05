cmake_minimum_required(VERSION 3.6)
project(tiger)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    chap1/main.c
    chap1/prog1.c
    chap1/prog1.h
    chap1/slp.c
    chap1/slp.h
    chap1/util.c
    chap1/util.h
    chap10/flowgraph.h
    chap10/graph.c
    chap10/graph.h
    chap11/color.h
    chap11/regalloc.h
    chap12/runtime.c
    chap2/driver.c
    chap2/errormsg.c
    chap2/errormsg.h
    chap2/tokens.h
    chap2/util.c
    chap2/util.h
    chap3/errormsg.c
    chap3/errormsg.h
    chap3/lex.yy.c
    chap3/parsetest.c
    chap3/util.c
    chap3/util.h
    chap4/absyn.c
    chap4/absyn.h
    chap4/lex.yy.c
    chap4/parse.c
    chap4/parse.h
    chap4/prabsyn.c
    chap4/prabsyn.h
    chap4/symbol.c
    chap4/symbol.h
    chap4/table.c
    chap4/table.h
    chap5/types.c
    chap5/types.h
    chap6/temp.c
    chap6/temp.h
    chap7/printtree.c
    chap7/printtree.h
    chap7/tree.c
    chap7/tree.h
    chap8/canon.c
    chap8/canon.h
    chap9/assem.c
    chap9/assem.h
    chap9/canon.c
    chap9/canon.h
    chap9/main.c chap4/temp.c.cpp chap4/temp.c chap5/types.h.cpp chap5/types.h.h chap5/env.c chap5/env.h chap5/semant.c chap5/semant.h)

add_executable(tiger ${SOURCE_FILES})