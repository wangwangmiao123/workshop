//
// Created by lesiamis on 2/20/17.
//

#include "types.h"
#include "translate.h"
#include "temp.h"
#include "symbol.h"

#ifndef TIGER_ENV_H
#define TIGER_ENV_H


typedef struct E_enventry_ *E_enventry;

struct E_enventry_ {
    enum E_ENUM_V_KIND {E_varEntry, E_funEntry} kind;
    union {struct {Tr_access access; Ty_ty ty;} var;
        struct {Tr_level level ;    // translate.h
                Temp_label label ;
                Ty_tyList formals;
                Ty_ty result;} fun;
    } u;
};

E_enventry E_VarEntry(Tr_access access,  Ty_ty ty);
E_enventry E_FunEntry(Tr_access access, Tr_level level, Ty_tyList formals, Ty_ty result);
S_table E_base_tenv(void); /* Ty_ ty environment */ //map a symbol to a Ty_ ty entry
S_table E_base_venv(void); /* E_ enventry environment */

//others



#endif //TIGER_ENV_H
