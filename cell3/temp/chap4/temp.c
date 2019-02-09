%{
#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"

int yylex(void); /* function prototype */
A_exp absyn_root;

S_table vartypedict=S_empty();  //bind each variable with its tpye
// input    binding target
// funcnamew
//
S_table varvaluedict=S_empty();    //bind each variable with its value


void yyerror(char *s)
{
    EM_error(EM_tokPos, "%s", s);
}
%}

union {
    A_pos pos;
    int ival;
    string sval;
    A_var var;
    A_exp exp;
    A_expList explist;
    A_expList expseq;
    A_dec  dec;
    A_decList  declist;
    A_fundec fundec;
    A_fundecList fundeclist;
    A_ty   ty;
    A_namety namety;
    A_nametyList  nametypelist;
    A_field field ;
    A_fieldList  fieldlist;
    A_efield  efield;
    A_efieldList efieldlist;
    A_var var;
    A_exp seqexp;

}

%token <sval> ID STRING
%token <ival> INT
%token
        COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
LBRACE RBRACE DOT
        PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
        AND OR ASSIGN
ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF
BREAK NIL
FUNCTION VAR TYPE

%type <exp> exp program
%type <expseq>    expseq
%type <explist>    explist
%type <dec>    dec
%type <declist>    declist
%type <fundec>    fundec
%type <funcdeclist>    funcdeclist
%type <ty>    ty
%type <namety>    namety
%type <nametylist>    nametylist
%type <field>    field
%type <fieldlist>    fieldlist
%type <efield>    efield
%type <efieldlist>    efieldlist
%type <var> var
%type <seqexp> seqexp


%start program

%%

program:   exp    {absyn_root=$1;}

var :   ID  {$$=A_SimpleVar(EM_tokPos,S_Symbol($1));}
|   ID DOT ID   {$$=A_FieldVar(EM_tokPos,A_SimpleVar(EM_tokPos,$1), S_Symbol(EM_tolPos,$3));}
|   ID LBRACK exp RBRACK    {$$=A_SubsciptVar(EM_tokPos, A_SmipleVar(EM_tokpos,$1),$3);}
;

exp :   ID      {$$=A_VarExp(EM_tokPos,A_SimpleVar(EM_tokPos,S_Symbol($1)));}
|   NIL     {$$=A_NilExp(EM_tokPos); }
|   INT     {$$=A_IntExp(EM_tokPos,$1);}
|   STRING  {$$=A_StringExp(EM_tokPos,$1);}
|   ID LPAREN explist RPAREN    {$$=A_CallExp(EM_tokPos,S_Symbol($1),$3);}
|   exp PLUS exp        {$$=A_OpExp(EM_tokPos,A_plusOp,$1,$3);}
|   exp MINUS exp       {$$=A_OpExp(EM_tokPos,A_minusOp,$1,$3);}
|   exp TIMES exp       {$$=A_OpExp(EM_tokPos,A_timesOp,$1,$3);}
|   exp DIVIDE exp      {$$=A_OpExp(EM_tokPos,A_divideOp,$1,$3);}
|   exp EQ exp          {$$=A_OpExp(EM_tokPos,A_eqOp,$1,$3);}
|   exp NEQ exp         {$$=A_OpExp(EM_tokPos,A_neqOp,$1,$3);}
|   exp LT exp          {$$=A_OpExp(EM_tokPos,A_ltOp,$1,$3);}
|   exp GT exp          {$$=A_OpExp(EM_tokPos,A_gtOp,$1,$3);}
|   exp LE exp          {$$=A_OpExp(EM_tokPos,A_leOp,$1,$3);}
|   exp GE exp          {$$=A_OpExp(EM_tokPos,A_geOp,$1,$3);}
|   ID  ASSIGN exp      {$$=A_AssignExp(EM_tokPos,A_SimpleVar(EM_tokPos,S_Symbol($1)));}
|   IF exp THEN exp     {$$=A_IfExp(EM_tokPos,$2,$4,0);}
|   IF exp THEN exp ELSE exp        {$$=A_IfExp(EM_tokPos,$2,$4,$6);}
|   exp AND exp             {$$=A_IfExp(EM_tokPos,$1,$3,0);}
|   exp OR  exp             {$$=   A_IfExp(EM_tokPos,$1,1,$3);}
|   WHILE exp DO exp        {$$=A_WhileExp(EM_tokPos,$2,$4);}
|   BREAK  SEMICOLON                    {$$=A_BreakExp(EM_tokPos);}
|   MINUS INT %prec UMINUS              {$$=A_OpExp(EM_tokPos,A_minusOp,0,$2);}
|   LET declist IN exp END              {$$=A_LetExp(EM_tokPos,$2,$4);}
|   ID LBRACK exp RBRACK OF exp         {$$=A_ArrayExp(EM_tokPos,S_Symbol($1),$3,$6);}
|   FOR ID ASSIGN exp TO exp DO exp     {$$=A_AssignExp(EM_tokPos,S_Symbol($2),$4,$6,$8);}
;

expseq :    LPAREN  explist  RPAREN      {$$=A_SeqExp(EM_tokPos,$2);}
explist :   exp COMMA explist       {$$=A_ExpList($1,$3);}
|   exp SEMICOLON explist   {$$=A_ExpList($1,$3);}
|   exp                     {$$=A_ExpList($1,$3);}
;

dec :   fundeclist  {$$=A_FunctionDec(EM_tokPos,$1);}
|   nametylist  {$$=A_TypeDec(EM_tokPos,$1);}
|   VAR ID COLON ID ASSIGN exp   {$$=A_varDec(EM_tokPos,S_Symbol($2),S_Symbol($4),$6);}
|   VAR ID ASSIGN exp   {$$=A_varDec(EM_tokPos,S_Symbol($2),S_check_exp_type($4),$4);}
;
declist :   dec    {$$=A_DecList($1,NULL);}
|   dec declist     {$$=A_DecList($1,$2);}
;

fundec  :    FUNCTION ID LPAREN fieldlist RPARNE COLON ID EQ exp  {
    TAB_enter();
    $$=A_Fundec(EM_tokPos,$2,$7,$9);
};
fundeclist  :   fundec fundeclist  {$$=A_FundecList($1,$2);}
|   fundec      {$$=A_FundecList($1,NULL);}
;

ty  :   LBRACE fieldlist RBRACE  {$$=A_RecordTy(EM_tokPos,$5);}
|   ARRAY OF ID    {$$=A_arrayTy(EM_tokPos,$3);}
|   ID  { $$=A_NameTy(EM_tokPos,S_Symbol($1)); }
;
namety  :   TYPE ID EQ  ty  {$$=A_Namety($2,$4);};
nametylist  :   namety  {$$=A_NametyList($1,NULL);}
|   namety nametylist {$$=A_NametyList($1,$2);}
;

field   :   ID COLON ID   {$$=A_Field(EM_tokPos,$1,$3);} ;
fieldlist   : {$$=A_FieldList(NULL,NULL);}
| field COMMA fieldlist  {$$=A_FieldList($1,$3);}
| field {$$=A_FieldList($1,NULL) ;}
;

efield :  ID EQ exp {$$=A_Efield(S_Symbol($1),$3);} ;
efieldlist :
|    efield efieldlist   {$$=A_EfieldList($1,$2);}
;
