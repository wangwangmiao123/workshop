//
// Created by lesiamis on 2/20/17.
//

#ifndef TIGER_SEMANT_H
#define TIGER_SEMANT_H

typedef void *Tr_exp;

struct expty {Tr_exp exp; Ty_ty ty;};   //why we call this empty : it is a temporary struct
struct expty expTy(Tr_exp exp, Ty_ty ty) {
    struct expty e; e.exp=exp; e.ty=ty; return e;
}

struct Ty_ty transTy ( S_table tenv, A_ty a);

void SEM_transexp(A_exp exp );

struct expty transVar(S_table venv, S_table tenv, A_var v  );
struct expty transExp(S_table venv, S_table tenv, A_exp a);
void  transDec(S_table venv, S_table tenv, A_dec d);

#endif //TIGER_SEMANT_H
