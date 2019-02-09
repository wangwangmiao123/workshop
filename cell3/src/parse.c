/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include "tools/util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "semant.h"
#include "env.h"

extern int yyparse(void);
extern A_exp absyn_root;




/* parse source file fname; 
   return abstract syntax data structure */
A_exp parse(string fname) 
{
    table0 = S_empty();
    EM_reset(fname);
    if (yyparse() == 0) /* parsing worked */
        return absyn_root;
    else return NULL;
}

int main(){
    A_exp proot parse("./sample.tig");
    SEM_transexp(proot); //take prog root as an entrance, recursively carry out type checking
    return 0;
}
