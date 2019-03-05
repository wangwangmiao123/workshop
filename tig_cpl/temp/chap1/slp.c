#include <string.h>
#include "util.h"
#include "slp.h"
//#include "../chap4/absyn.h"

A_stm A_CompoundStm(A_stm stm1, A_stm stm2) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind=A_compoundStm; s->u.compound.stm1=stm1; s->u.compound.stm2=stm2;
  return s;
}

A_stm A_AssignStm(string id, A_exp exp) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind=A_assignStm; s->u.assign.id=id; s->u.assign.exp=exp;
  return s;
}

A_stm A_PrintStm(A_expList exps) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind=A_printStm; s->u.print.exps=exps;
  return s;
}

A_exp A_IdExp(string id) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind=A_idExp; e->u.id=id;
  return e;
}

A_exp A_NumExp(int num) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind=A_numExp; e->u.num=num;
  return e;
}

A_exp A_OpExp(A_exp left, A_binop oper, A_exp right) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind=A_opExp; e->u.op.left=left; e->u.op.oper=oper; e->u.op.right=right;
  return e;
}

A_exp A_EseqExp(A_stm stm, A_exp exp) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind=A_eseqExp; e->u.eseq.stm=stm; e->u.eseq.exp=exp;
  return e;
}

A_expList A_PairExpList(A_exp head, A_expList tail) {
  A_expList e = checked_malloc(sizeof *e);
  e->kind=A_pairExpList; e->u.pair.head=head; e->u.pair.tail=tail;
  return e;
}

A_expList A_LastExpList(A_exp last) {
  A_expList e = checked_malloc(sizeof *e);
  e->kind=A_lastExpList; e->u.last=last;
  return e;
}


int maxargs_(A_exp x){
  if(x->kind != A_eseqExp){
    return 0;
  }
  int s=maxargs(x->u.eseq.stm);
  int e=maxargs_(x->u.eseq.exp);
  return s>e?s:e;
}

int expListLen(A_expList x){
  int i=1;
  A_expList t=x;
  while(t->kind!=A_lastExpList){
    i++;
    t=t->u.pair.tail;
  }
  return  i;
}

int maxargs(A_stm x){
  int len,t,rt;
  A_expList l;
  if(x->kind == A_printStm) {
    len=expListLen(x);
    rt=len;
    l=x->u.print.exps;
    while(1){
      if(l->kind == A_lastExpList){
        t=maxargs_(l->u.last);
        rt=t>rt?t:rt;
        break;
      }
      else{
        t=maxargs_(l->u.pair.head);
        rt=t>rt?t:rt;
        l=l->u.pair.tail;
      }
    }
  }
  else if(x->kind == A_compoundStm){
    t=maxargs(x->u.compound.stm1);
    rt=maxargs(x->u.compound.stm1);
    rt=rt>=t?rt:t;
  }
  else if(x->kind == A_assignStm){
    return maxargs_(x->u.assign.exp);
  }
  return rt;
}

Table_ Table(string id, int value, struct table *tail) {
  Table_ t = checked_malloc(sizeof(*t));
  t->id=id; t->value=value; t->tail=tail;
  return t;
}

Table_ interpStm (A_stm s, Table_ t){
  struct IntAndTable kv;
  A_expList tp;
  switch (s->kind){
    case A_assignStm:
      kv=interpExp(s->u.assign.exp,t);
          return(Table(s->u.assign.id,kv.i,kv.t));
    case A_compoundStm:
      t=interpStm(s->u.compound.stm1,t);
      t=interpStm(s->u.compound.stm1,t);
          return(t);
    case A_printStm:
      tp=s->u.print.exps;
      while(1){
        if(tp->kind == A_lastExpList){
          t=interpExp(tp->u.last,t).t;
          break;
        }
        else{
          t=interpExp(tp->u.pair.head,t).t;
          tp=tp->u.pair.tail;
        }
      }
          return t;
  }
}
int lookup(Table_ t, string key){
  int rt=-1;
  while(!t){
    if(!strcmp(key,t->id))return t->value;
    else{
      t=t->tail;
    }
  }
  return rt;
}

struct IntAndTable interpExp (A_exp s, Table_ t){
  int x,tp;
  struct IntAndTable t0;
  switch(s->kind){
    case A_eseqExp:
      t=interpStm(s->u.eseq.stm,t);
          return interpExp(s->u.eseq.exp,t);
    case A_numExp:
      return (struct IntAndTable){s->u.num,t};
    case A_idExp:
      x=lookup(t,s->u.id);
      return (struct IntAndTable){x,t};
    case A_opExp:
      t0=interpExp(s->u.op.left,t);
      tp=t0.i;
      t0=interpExp(s->u.op.right,t0.t);
      switch(s->u.op.oper){
        case A_plus:
          return (struct IntAndTable){tp+t0.i,t0.t};
        case A_minus:
          return (struct IntAndTable){tp-t0.i,t0.t};
        case A_times:
          return (struct IntAndTable){tp*t0.i,t0.t};
        case A_div:
          return (struct IntAndTable){tp/t0.i,t0.t};
      }
  }
}

void interp(A_stm x){
  Table_ result;
  result=interpStm(x,NULL);
}





