#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "errormsg.h"
#include "tokens.h"

YYSTYPE yylval;

int yylex(void); /* prototype for the lexing function */


string toknames[] = {
        "ID", "STRING", "INT", "COMMA", "COLON", "SEMICOLON", "LPAREN",
        "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT", "PLUS",
        "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT", "GE",
        "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE", "FOR",
        "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
        "VAR", "TYPE"
};


string tokname(int tok) {
    return tok<257 || tok>299 ? "BAD_TOKEN" : toknames[tok-257];
}

int main(int argc, char **argv) {
    string fname;
    int tok;
    if (argc!=2) {fprintf(stderr,"usage: a.out filename\n"); exit(1);}
    fname=argv[1];
    EM_reset(fname);
    for(;;) {
        tok=yylex();
        if (tok==0) break;
        switch(tok) {
            case ID: case STRING:
                printf("%10s %4d %s\n",tokname(tok),EM_tokPos,yylval.sval);
                break;
            case INT:
                printf("%10s %4d %d\n",tokname(tok),EM_tokPos,yylval.ival);
                break;
            case COMMA:
                printf("COMMA\n");
                break;
            case COLON:
                printf("COLON\n");
                break;
            case SEMICOLON:
                printf("SEMICOLON\n");
                break;
            case LPAREN:
                printf("LPAREN\n");
                break;
            case RPAREN:
                printf("RPAREN\n");
                break;
            case LBRACE:
                printf("LBRACE\n");
                break;
            case RBRACE:
                printf("RBRACE\n");
                break;
            case LBRACK:
                printf("LBRACK\n");
                break;
            case RBRACK:
                printf("RBRACK\n");
                break;
            case DOT:
                printf("DOT\n");
                break;
            case PLUS:
                printf("PLUS\n");
                break;
            case MINUS:
                printf("MINUS\n");
                break;
            case TIMES:
                printf("TIMES\n");
                break;
            case DIVIDE:
                printf("DIVIDE\n");
                break;
            case EQ:
                printf("EQ\n");
                break;
            case NEQ:
                printf("NEQ\n");
                break;
            case LT:
                printf("LT\n");
                break;
            case LE:
                printf("LE\n");
                break;
            case GT:
                printf("GT\n");
                break;
            case GE:
                printf("GE\n");
                break;
            case AND:
                printf("AND\n");
                break;
            case OR:
                printf("OR\n");
                break;
            case ASSIGN:
                printf("ASSIGN\n");
                break;
            case ARRAY:
                printf("ARRAY\n");
                break;
            case IF:
                printf("IF\n");
                break;
            case THEN:
                printf("THEN\n");
                break;
            case ELSE:
                printf("ELSE\n");
                break;
            case FOR:
                printf("FOR\n");
                break;
            case TO:
                printf("TO\n");
                break;
            case DO:
                printf("DO\n");
                break;
            case WHILE:
                printf("WHILE\n");
                break;
            case LET:
                printf("LET\n");
                break;
            case IN:
                printf("IN\n");
                break;
            case END:
                printf("END\n");
                break;
            case OF:
                printf("OF\n");
                break;
            case BREAK:
                printf("BREAK\n");
                break;
            case VAR:
                printf("VAR\n");
                break;
            case NIL:
                printf("NIL\n");
                break;
            case FUNCTION:
                printf("FUNCTION\n");
                break;
            case TYPE:
                printf("TYPE\n");
                break;
            default:
                printf("%10s %4d\n",tokname(tok),EM_tokPos);
        }
    }
    return 0;
}


