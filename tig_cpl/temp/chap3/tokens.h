typedef union  {
	int pos;
	int ival;
	string sval;
	} YYSTYPE;
extern YYSTYPE yylval;

# define ID 257			//identifier
# define STRING 258		//string
# define INT 259	//integer
# define COMMA 260	//,
# define COLON 261	//:
# define SEMICOLON 262	//;
# define LPAREN 263	//(
# define RPAREN 264	//)
# define LBRACK 265	//[
# define RBRACK 266	//]
# define LBRACE 267	//{
# define RBRACE 268	//}
# define DOT 269	//.
# define PLUS 270	//+
# define MINUS 271	//-
# define TIMES 272	//*
# define DIVIDE 273	///
# define EQ 274		//=
# define NEQ 275	//<>
# define LT 276		//<
# define LE 277		//<=
# define GT 278		//>
# define GE 279		//>=
# define AND 280	//&
# define OR 281		//|
# define ASSIGN 282	//:=
# define ARRAY 283	//array of
# define IF 284		//if (case-sensitive)
# define THEN 285	//then
# define ELSE 286	//else
# define WHILE 287	//while
# define FOR 288	//for
# define TO 289		//to
# define DO 290		//do
# define LET 291	//let
# define IN 292		//in
# define END 293	//end
# define OF 294		//of
# define BREAK 295	//break
# define NIL 296	//nil
# define FUNCTION 297	//
# define VAR 298	//var
# define TYPE 299	//type

