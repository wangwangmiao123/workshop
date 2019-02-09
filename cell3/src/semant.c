//
// Created by lesiamis on 2/20/17.
//
// info : where I implement type-checker module



#include "semant.h"
#include "errormsg.h"
#include "absyn.h"
#include "symbol.h"
#include "types.h"
#include "env.h"

///tag here ! should be enhanced !!!
///and how-to-include : {implementation-related headers vs declatration-related header}
// I think the constrcution of an environment belongs to type-checker module

S_table venv;
S_table tenv;


S_symbol finalty(Ty_ty t){
    switch (t->kind){
        case  t->Ty_array:{
            return S_Symbol("array");
        }
        case  t->Ty_int:{
            return S_Symbol("int");
        }
        case t->Ty_name:{ // this is key part
            return finalty(t->u.name.ty);
        }
        case t->Ty_nil:{
            return S_Symbol("nil");
        }
        case t->Ty_void:{
            return S_Symbol("void");
        }
        case t->Ty_record:{
            return S_Symbol("record");
        }
        case t->Ty_string:{
            return S_Symbol("string");
        }
        default: break;
    }
}


int comparefl(A_fieldList l0 , A_fieldList l1){
    while(l0 && l1){
        if(l0->head->name == l1->head->name){
            if(S_look(tenv,l0->head->typ), S_look(tenv,l1->head->typ));
        }
        l0=l0->tail;
        l1=l1->tail;
    }
    if(l0 || l1){ return 0;}
    return  1;
}

int comparety(Ty_ty t0, Ty_ty t1){
    S_symbol s0,s1;
    s0=finalty(t0);
    s1=finalty(t1);
    if(s0 == s1){
        if(s0 == S_Symbol("array")){
            return (t0->u.array,t1->u.array);
        }
        else if(s0 == S_Symbol("record")){
            A_fieldList l0 = t0->u.record;
            A_fieldList l1 = t1->u.record;
            return comparefl(l0,l1);
        }
        else{
            return 1;
        }
    }
    else{ return 0;}
}

void SEM_transexp(A_exp exp ){
    // instroduce base environment
    struct expty ret0;
    tenv = E_base_tenv();
    venv = E_base_venv();
    ret0 = transExp(venv,tenv,exp);
    return;
}

void transDec(S_table venv, S_table tenv, A_dec d) {
    switch (d->kind) {
        case A_varDec: {
            struct expty e = transExp(venv, tenv, d->u.var.init);   //
            S_symbol s0=finalty(e);
            if(comparety(e.ty,Ty_Nil()) && comparety(d->u.var.typ == S_Symbol("void"))){
                EM_error(d->pos,"with initialization value as nil, variable type should be explict declared!");
            }
            if(!comparety(e.ty,s0) && !comparety(e.ty,Ty_Nil())){ printf("In Position : %d : Initialization Value Type Dismatch; Modified to Initialization Type !\n",\
                                            d->pos);d->u.var.typ = s0; }
            S_enter(venv, d->u.var.var, E_VarEntry(e.ty));
            return;
        }
        case A_typeDec: {
            ///tag nice pattern, but function call, I just do not like this
            for(A_nametyList nt0 = d->u.type; nt0; nt0 = nt0->tail ){
                S_enter(tenv, nt0->head->name, transTy(nt0->head->ty));
            }
            return;
        }
        case A_functionDec: {   ///tag here! no mutual recursive definition here!
                                ///but if there is, they must be inside the same loop here
            for(A_fundecList fd=d->u.function->tail; fd ; fd = fd->tail){
                A_fundec h = fd->head; //notice, function venv entry do not have ot store each parameter name, we are not in python or ...
                //before starting translating its body and compair its type
                S_beginScope(venv);
                S_beginScope(tenv);
                    Ty_tyList tail0 = NULL ; //initialize a pointer
                    for ( A_fieldList fl = h->params; fl ; fl= fl->tail){
                        A_field field = fl->head;
                        Ty_ty  tempty=(Ty_ty)S_look(tenv,field->typ);   //data type must have be defined before
                        if(! tempty ) {EM_error(field ->pos, "Please Use Predefined Type "); }
                        S_enter(venv, field->name, E_VarEntry(tempty));
                        if(tail0){tail0->tail=Ty_TyList(tempty,NULL);tail0=tempty;}
                        else{
                            tail0 = Ty_TyList(tempty,NULL); //
                        }
                    } // check all the field type validity, and register the argument lists for body evaluation, then remove them
                    if( !S_look(tenv, h->result ) ){printf("Can Not Find Corresponding Type! Notice!");}
                    struct expty bodytype=transExp(venv, tenv, h->body);
                    S_symbol s0 = finalty(bodytype.ty);
                    if(!typecompare(s0,h->result)){ /// tag here ! should we keep the warpper here
                        print("Type Error\n");
                        h->result = s0 ;
                    }
                S_endScope(tenv);
                S_endScope(venv);
                //outside the poping scope
                S_enter(venv,h->name,E_FunEntry(tail0,bodytype.ty));   /// another option : get the lowest level Ty_ty, no, may cause info lose
            }
            return; // one by one, fit into the venv
        }
        default :
            break;
    }
    return;
}

struct expty transVar(S_table venv, S_table tenv, A_var v ) {
    switch (v->kind) {
        case A_simpleVar: {
            E_enventry x = S_look(venv, v->u.simple);
            if (x && x->kind == E_varEntry)
                return expTy(NULL, actual_ty(x->u.var.ty));
            else {
                EM_error(v->pos, "undefined variable %s",
                         S_name(v->u.simple));  // TAG !! EM_error do not solve problem ! terminate it
                return expTy(NULL, Ty_Int());
            }
        }
        case A_fieldVar:{ // first check the type an
                          // not ID DOT ID DOT ID is allowed here
            A_var vp=v->u.field.var;
            E_enventry ep=S_look(venv,vp->u.simple);
            if(!ep){
                EM_error(vp->pos,"undefined variable %s",S_name(vp->u.simple));
                //TAG
            }
            if(ep->kind!=ep->E_varEntry){
                EM_error(vp->pos,"try to use function: %s as a record",S_name(vp->u.simple);)
            }
            if(ep->u.var.ty.kind != ep->u.var.ty->Ty_record){}
            Ty_fieldList l=ep->u.var.ty->u.record;
            while(l!=NULL){
                if(l->head->name == v->u.field.sym){
                    break;
                }
                l=l->tail;
            }
            if(!l){
                EM_error(v->pos,"record : %s does not have field name : %s ", S_name(vp->u.field.var->u.simple),S_name(v->u.field.sym));
                ///TAG
            }
            return  expTy(NULL,actualtype(l->head->ty));
        }
        case A_subscriptVar:{
            A_var vp = v->u.subscript.var;
            E_enventry  ep=S_look(venv,vp->u.simple);
            if(!ep){
                EM_error(vp->pos,"undefined array variable %s",S_name(vp->u.simple));
                //TAG
            }
            if(ep->kind != ep->E_varEntry){
                EM_error(vp->pos,"function %s is not a array variable",S_name(vp->u.simple));
                //TAG
            }
            if(ep->u.var.ty->kind != ep->u.var.ty->Ty_array){
                EM_error(vp->pos,"variable %s is not a array variable",S_name(vp->u.simple));
                //TAG
                }

            v->u.subscript.exp
        }
    }
}

struct expty transExp(S_table venv, S_table tenv, A_exp a) {
    struct expty ret0;
    switch(a->kind) {
        case A_opExp: {
            A_oper oper = a->u.op.oper;

            struct expty left =transExp(venv,tenv,a->u.op.left);    // left and right different scope
            struct expty right=transExp(venv,tenv,a->u.op.right);
            // {A_plusOp, A_minusOp, A_timesOp, A_divideOp, A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp} A_oper;
            if (!comparety(left.ty,Ty_Int() && !comparety(left.ty,Ty_Nil()))) {
                EM_error(a->u.op.left->pos, "integer required");
                //tag
            }
            if (!comparety(right.ty,Ty_Int() && !comparety(right.ty,Ty_Nil())))
            {
                EM_error(a->u.op.right->pos, "integer required");
                //tag
            }
            if (comparety(right.ty,Ty_Nil() && comparety(right.ty,Ty_Nil())))
            {
                EM_error(a->pos, "at least one of the oprand should be integer!");
                //
            }
            if (oper==A_plusOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_minusOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_divideOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_timesOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_leOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_ltOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_gtOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_geOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_eqOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            if (oper==A_neqOp)
            {
                return expTy(NULL,Ty_Int()); //do not evaluate the value ///well step by step
            }
            break;
        }
        case A_varExp: {
            return (expTy(NULL, transVar(venv, tenv, a->u.v)));
        }
        case A_nilExp:{
            return expTy(NULL,Ty_Nil());
        }
        case A_intExp:{
            return expTy(NULL, Ty_Int());
        }
        case A_stringExp: {
            return expTy(NULL, Ty_String());
        }
        case A_callExp:
        {   //look up function name and corresponding type
            E_enventry func0=(E_enventry)S_look(venv,a->u.call.func);
            if(!(func0 != NULL && func0->kind == func0->E_funEntry)){
                EM_error(a->pos,"No Such Function ");  //in this way, EM_error should never return
            }
            else{
                Ty_tyList tylist0 = func0->u.fun.formals;
                for (A_expList elst0 = a->u.call.args; elst0 ; elst0 = elst0 ->tail){
                    if(tylist0 == NULL ){ //length
                        EM_error(a->pos,"Too Many Arguments Passed In \n");
                    }
                    A_exp exp0=elst0->head ;
                    Ty_ty ty0=transExp(venv, tenv, exp0);
                    if(!comparety(tylist0->head,ty0){ ///tag!
                        EM_error(a->pos,"Argument Type Not Matched \n");
                    }
                    tylist0 = tylist0 ->tail;
                }
                if(tylist0) // thus arguments list too short
                {
                    EM_error(a->pos,"Argument List Too Short\n");
                }
                //good result
                //here we should check the validity of transferred argument
                return expTy(NULL,func0->u.fun.result); // fine, end;
            }
        }
        case A_recordExp:
        {
            Ty_ty t0=S_look(tenv, a->u.record.typ);
            Ty_fieldList tfl0 = t0->u.record;
            if(!t0 || t0->kind!=t0->Ty_record){EM_error(a->pos,"No Such Record Type!");} ///notice : check type
            for (A_efieldList efl0 = a->u.record.fields; efl0 ; efl0 =efl0->tail) {
                if(!tfl0){EM_error(a->pos,"Not Enought Assignment for Record Fields\n");} ///tag here! should order of the field assignment matters ?
                if(efl0->head->name != tfl0->head->name){EM_error(a->pos,"FieldName Not Matched!\n");}
                Ty_ty ty0 = (struct expty)transExp(venv,tenv,efl0->head->exp).ty;
                if(!comparetype(ty0,tfl0->head->ty))   ///tag! formalize the
                {
                    EM_error(a->pos, "Record Field Type Not Matched");
                }
                tfl0 = tfl0->tail;
            }
            if(!tfl0){
                EM_error(a->pos, "Assignemnt Field List Too Short \n");
            }
            return (struct expty){NULL,t0};
        }
        case A_seqExp:
        {
            A_expList el = a->u.seq;
            struct expty ret0;
            while (el!=NULL){
                ret0 = transExp(venv,tenv,el->head);
                el=el->tail;
            }
            return ret0;
        }
        case A_assignExp:
        {
            A_var v0 = a->u.assign.var;
            E_enventry et0;
            switch (v0->kind){
                case A_simpleVar :{
                    et0 = S_look(venv, v0->u.simple);
                    if(!et0){}
                }
                case A_fieldVar:{
                    et0 = S_look(venv,v0->u.field.sym);
                    if(!et0)
                }
                case A_subscriptVar:{
                    if(!S_look(tenv,a->u.array.typ)){
                        EM_error(a->pos, "type not defined yet",S_name(a->u.array.typ));
                    }
                    struct expty ret=
                    if(v0->u.subscript.var->kind!=)
                    et0 = S_look(venv,v0->u.subscript.var.)
                }
                default:
                    break;
            }
            return expTy(NULL,Ty_Void()); // in tiger, ass
        }
        case A_arrayExp:
        {
            Ty_ty t;
            if (!(t = S_look(tenv, a->u.array.typ))) {
                EM_error(a->pos, "arraytype not defined yet", S_name(a->u.array.typ));
            }
            struct expty ret = transExp(venv, tenv, a->u.array.init);
            if (!ret || !comparety(t->u.array, ret.ty)) {
                EM_error(a->pos, "initial value type not matched\n");
                //
            }
            ret = transExp(a->u.array.size); // notice the value check(>0 and < MAX ) TAG
            if(!comparety(ret.ty,Ty_Int())){
                EM_error(a->pos,"array size should be positive integer\n");
                //tag
            }
            return expTy(NULL,t->u.array);
        }
        case A_letExp:
        {
            struct expty exp;
            A_decList d;

            S_beginScope(venv);
            S_beginScope(tenv);

            for (d = a->u.let.decs; d; d = d->tail) // no mutual-recursive definition here
                transDec(venv, tenv, d->head);

            exp = transExp(venv, tenv, a->u.let.body);
            S_endScope(tenv);
            S_endScope(venv);
            return exp;
        }
        case A_breakExp:
        {
            return expTy(NULL, Ty_Void());
        }
        case A_forExp:
        {
            E_enventry e0;
            struct expty exp0, exp1;
            if(!(e0=S_look(venv, a->u.forr.var)){
                EM_error(a->pos, "variable %s not declared\n",S_name(a->u.forr.var));
                //TAG
            }
            if(!e0->kind != e0->E_varEntry){
                EM_error(a->pos, "require a variable name instead of function name : %s",S_name(a->u.forr.var));
                //tag
            }
            exp0=transExp(venv,tenv,a->u.forr.lo);
            if(!comparety(exp0.ty,e0->u.var.ty)){
                EM_error(a->pos,"type do not match, require a %s, but get a %s",S_name(a->u.forr.var),S_name(finalty(exp0.ty)));
                //
            }
            exp0=transExp(venv,tenv,a->u.forr.hi);
            if(!comparety(exp0.ty,e0->u.var.ty)){
                EM_error(a->pos,"type do not match, require a %s, but get a %s",S_name(a->u.forr.var),S_name(finalty(exp0.ty)));
                //
            }
            return transExp(venv, tenv, a->u.forr.body);
        }

        case A_ifExp:
        {
            struct expty exp0,exp1;
            exp0 = transExp(venv,tenv, a->u.iff.test);
            if(!comparety(exp0.ty,Ty_Int())){
                EM_error(a->pos,"test part should yield a interger value instead of %s",S_name(finaltype(exp0.ty)));
                //
            }
            exp0 = transExp(venv, tenv, a->u.iff.then); //TAG, should consider NULL expression!!!
            if(a->u.iff.elsee) { /// then if .. then .. was used here
                exp1 = transExp(venv, tenv, a->u.iff.elsee);
            }
            else exp1 = NULL;
            if(!exp1){
                return exp0;
            }
            else{
                if(!comparety(exp0->ty, exp1.ty)){
                    EM_error(a->pos,"then part and else part do not have the same type!");
                    // TAG
                }
                else{
                    return exp0;
                }
            }
        }

        case A_whileExp:
        {
            struct expty test;
            test=transExp(venv, tenv, a->u.whilee.test);
            if(!comparety(test.ty, Ty_Int())){
                EM_error(a->pos,"test part of while shold yield a integer!");
                //tag
            }
            return  transExp(venv, tenv, a->u.whilee.body);
        }
        default :
            break;
    }
    assert(0); /* should have returned from some clause of the switch */
    return ret0;
}

Ty_ty transTy( S_table tenv, A_ty a){
    switch (a->kind)
    {
        case A_nameTy:{
            Ty_ty t0=(Ty_ty)S_look(tenv,a->u.name);
            if(!(t0)){ EM_error( a->pos, "Can not find the symbol");}
            while(t0->kind == t0->Ty_name){
                t0=t0->u.name.ty;
            }
            switch (t0->kind){
                case t0->Ty_int:
                    return Ty_Int();
                case t0->Ty_nil:
                    return Ty_Nil();
                case t0->Ty_string:
                    return Ty_String();
                case t0->Ty_void:
                    return Ty_Void();
                default:
                    return t0;  // if we reach a array or record, regard them as final type
                                // what if t0 is not in the environment,
                                // late bind is required
            }
        }
        case A_arrayTy:
        {
            Ty_ty t=S_look(tenv,a->u.array); //a.array stores the array element type, which should be pre-defined earlier
            if(!(t)){EM_error(a->pos, "Can not find the symbol");}
            return Ty_Array(t); //Ty_Array(t) return a array type which is the array of passed_in t
        }
        case A_recordTy:
        {
            A_fieldList f=a->u.record;
            while(NULL!=f){
                if(!S_look(tenv,f->head->name)){
                    EM_error(f->head->pos,"Type is Not declared!\n");
                }
                f=f->tail;  // that type is not in the evironment
            }
            Ty_ty t=S_look(tenv,Ty_Record(a->u.record0));
        }
        default:
            break;
    }
}
