//
// Created by lesiamis on 2/20/17.
//
// Some Notice :
//  -introduce undecided variable types
//  -struct TY_ty used in type environment is different from A_ty we build in absyn.h
//

#include "env.h"
#include "symbol.h"
#include "types.h"
#include "tools/util.h"

extern S_table venv;    //varibales, function infos
extern S_table tenv;    //construct for type system

//ty contains all the information you need {var or func} to augment a env
E_enventry E_VarEntry(Tr_access access, Ty_ty ty)
{
        E_enventry e0 = (E_enventry)checked_malloc(sizeof(*e0));
        e0->kind = e0->E_varEntry;
        e0->u.var.ty=ty;
    return  e0;
}

E_enventry E_FunEntry(Tr_level level, Temp_label  label , Ty_tyList formals, Ty_ty result)
{
    E_enventry e0 = (E_enventry)checked_malloc(sizeof(*e0));
    e0->kind = e0->E_funEntry;
    e0->u.fun.formals=formals;
    e0->u.fun.result=result;
    return  e0;

}

S_table E_base_tenv(void) /* Ty_ ty environment */
{
    S_table t0=S_empty();
    S_enter(t0,S_Symbol("int"),Ty_Int());
    S_enter(t0,S_Symbol("string",Ty_String()));
    S_enter(t0,S_Symbol("nil",Ty_Nil()));
    S_enter(t0,S_Symbol("void",Ty_Void()));
    return t0;

}

S_table E_base_venv(void) /* E_enventry environment */
{
    S_table e0=S_empty();
    S_enter(e0,S_Symbol("print"),E_FunEntry(Ty_TyList(Ty_String(),NULL),Ty_void()));
    S_enter(e0,S_Symbol("flush"),E_FunEntry(NULL,Ty_void()));
    S_enter(e0,S_Symbol("getchar"),E_FunEntry(NULL,Ty_String()));
    S_enter(e0,S_Symbol("ord"),E_FunEntry(Ty_TyList(Ty_String(),NULL),Ty_Int()));
    S_enter(e0,S_Symbol("chr"),E_FunEntry(Ty_TyList(Ty_Int(),NULL),Ty_String()));
    S_enter(e0,S_Symbol("size"),E_FunEntry(Ty_TyList(Ty_String(),NULL),Ty_Int()));
    S_enter(e0,S_Symbol("substring"),E_FunEntry(Ty_TyList(Ty_String(),Ty_TyList(Ty_Int(),Ty_TyList(Ty_String(),NULL))),Ty_String()));
    S_enter(e0,S_Symbol("concat"),E_FunEntry(Ty_TyList(Ty_String(),Ty_TyList(Ty_String(),NULL),Ty_String())));
    S_enter(e0,S_Symbol("not"),E_FunEntry(Ty_TyList(Ty_Int(),NULL),Ty_Int()));
    S_enter(e0,S_Symbol("eixt"),E_FunEntry(Ty_TyList(Ty_Int(),NULL),Ty_Void()));

    return e0;
}


