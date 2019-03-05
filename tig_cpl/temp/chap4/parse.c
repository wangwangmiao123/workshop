/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "../chap3/errormsg.h"
#include "parse.h"

extern int yyparse(void);
A_exp absyn_root;
S_table  table0;

/* parse source file fname; 
   reurn abstract syntax data structure */
A_exp parse(string fname) 
{
    table0 = S_empty();
    EM_reset(fname);
    if (yyparse() == 0) /* parsing worked */
        return absyn_root;
    else return NULL;
}

int main(){
    parse("sample.tig");
}
