%{
#include <string.h>
#include "y.tab.h"
#include "errvormsg.h"

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}

void adjust(void)
 {
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}

%%
"if"    {adjust(); return IF;}
"then"  {adjust(); return THEN;}
"else"  {adjust(); return ELSE;}
"for"   {adjust(); return FOR;}
"to"    {adjust(); return TO;}
"do"    {adjust(); return DO;}
"while" {adjust(); return WHILE;}
"let"   {adjust(); return LET;}
"in"    {adjust(); return IN;}
"end"   {adjust(); return END;}
"of"    {adjust(); return OF;}
"break" {adjust(); return BREAK;}
"nil"   {adjust(); return NIL;}
"function"  {adjust(); return FUNCTION;}
"var"       {adjust(); return VAR;}
"type"      {adjust(); return TYPE;}
[a-zA-Z][0-9a-zA-Z_]*       {adjust(); yylval.sval=String(yytext);return ID;}
"/*"([^\*]*"*")+"/"         {adjust(); continue;}
[0-9]+	                    {adjust(); yylval.ival=atoi(yytext); return INT;}
"\""[^\"]*"\""              {adjust(); yylval.sval=String(&yytext[1]); yylval.sval[strlen(yylval.sval)-1] = '\0'; return STRING;}
":"     {adjust(); return COLON;}
","     {adjust(); return COMMA;}
";"     {adjust(); return SEMICOLON;}
"\("    {adjust(); return LPAREN;}
"\)"    {adjust(); return RPAREN;}
"\["    {adjust(); return LBRACK;}
"\]"    {adjust(); return RBRACK;}
"\{"    {adjust(); return LBRACE;}
"\}"    {adjust(); return RBRACE;}
"\."    {adjust(); return DOT;}
"\+"    {adjust(); return PLUS;}
"\-"    {adjust(); return MINUS;}
"\*"    {adjust(); return TIMES;}
"\/"    {adjust(); return DIVIDE;}
"="     {adjust(); return EQ;}
"<>"    {adjust(); return NEQ;}
":="    {adjust(); return ASSIGN;}
"\<"    {adjust(); return LT;}
"\>"    {adjust(); return GT;}
"\<="   {adjust(); return LE;}
"\>="   {adjust(); return GE;}
"&"     {adjust(); return AND;}
"\|"     {adjust(); return OR;}

(" "|\t)+ 	{adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}

%%
