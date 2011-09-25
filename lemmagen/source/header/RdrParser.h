#ifndef YY_Parser_h_included
#define YY_Parser_h_included

#line 1 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
/* before anything */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#endif
#include <stdio.h>

/* #line 14 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 21 "header\\RdrParser.h"
#line 23 "definition\\RdrParser.def"
 
	#include "../header/RdrTree.h"
	
	struct yy_token_detail
	{ int *piLine, *piColm, *piLeng;
	  byte **pacText;
	};
		
#define YY_Parser_CLASS  RdrParser
#define YY_Parser_LEX  yylex_parser
#define YY_Parser_LEX_BODY  =0
#define YY_Parser_ERROR  yyerror_parser
#define YY_Parser_ERROR_BODY  =0
#define YY_Parser_ENUM_TOKEN  yytoken
#define YY_Parser_NULL_TOKEN  EOFNULL
#define YY_Parser_LTYPE  yy_token_detail
#define YY_Parser_LLOC  yylloc
#define YY_Parser_OUTPUT_START  {sprintf(msgBuffer, 
#define YY_Parser_OUTPUT_END  ); (*yyout) << msgBuffer;}
#define YY_Parser_MEMBERS  \
	public: \
	protected: \
		YY_Parser_LTYPE YY_Parser_LLOC; \
		ostream *yyout; \
		bool bEOF;  \
		char msgBuffer[1000]; \
		RdrTree *rtTop; \
	private:
#define YY_Parser_CONSTRUCTOR_CODE  \
		bEOF = false; \
		//yydebug = true;
#define YY_Parser_DEBUG  0

#line 69 "definition\\RdrParser.def"
typedef union yy_parser_val {
	int iVal;
	byte *acVal;
	RdrRule *rrVal;
	RdrTree *rtVal;
	RdrTree::deqrt *deqVal;
} yy_Parser_stype;
#define YY_Parser_STYPE yy_Parser_stype

#line 14 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_Parser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_Parser_COMPATIBILITY 1
#else
#define  YY_Parser_COMPATIBILITY 0
#endif
#endif

#if YY_Parser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_Parser_LTYPE
#define YY_Parser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_Parser_STYPE 
#define YY_Parser_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_Parser_DEBUG
#define  YY_Parser_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
#endif
#endif
#ifdef YY_Parser_STYPE
#ifndef yystype
#define yystype YY_Parser_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_Parser_USE_GOTO
#define YY_Parser_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_Parser_USE_GOTO
#define YY_Parser_USE_GOTO 0
#endif

#ifndef YY_Parser_PURE

/* #line 63 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 118 "header\\RdrParser.h"

#line 63 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
/* YY_Parser_PURE */
#endif

/* #line 65 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 125 "header\\RdrParser.h"

#line 65 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
/* prefix */
#ifndef YY_Parser_DEBUG

/* #line 67 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 132 "header\\RdrParser.h"

#line 67 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
/* YY_Parser_DEBUG */
#endif
#ifndef YY_Parser_LSP_NEEDED

/* #line 70 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 140 "header\\RdrParser.h"

#line 70 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
 /* YY_Parser_LSP_NEEDED*/
#endif
/* DEFAULT LTYPE*/
#ifdef YY_Parser_LSP_NEEDED
#ifndef YY_Parser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_Parser_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
#ifndef YY_Parser_STYPE
#define YY_Parser_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_Parser_PARSE
#define YY_Parser_PARSE yyparse
#endif
#ifndef YY_Parser_LEX
#define YY_Parser_LEX yylex
#endif
#ifndef YY_Parser_LVAL
#define YY_Parser_LVAL yylval
#endif
#ifndef YY_Parser_LLOC
#define YY_Parser_LLOC yylloc
#endif
#ifndef YY_Parser_CHAR
#define YY_Parser_CHAR yychar
#endif

//Added by MJ for better output support
#ifndef YY_Parser_OUTPUT_START
#define YY_Parser_OUTPUT_START fprintf (stderr,
#endif
#ifndef YY_Parser_OUTPUT_END
#define YY_Parser_OUTPUT_END )
#endif

#ifndef YY_Parser_NERRS
#define YY_Parser_NERRS yynerrs
#endif
#ifndef YY_Parser_DEBUG_FLAG
#define YY_Parser_DEBUG_FLAG yydebug
#endif
#ifndef YY_Parser_ERROR
#define YY_Parser_ERROR yyerror
#endif

#ifndef YY_Parser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_Parser_PARSE_PARAM
#ifndef YY_Parser_PARSE_PARAM_DEF
#define YY_Parser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_Parser_PARSE_PARAM
#define YY_Parser_PARSE_PARAM void
#endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_Parser_PURE
extern YY_Parser_STYPE YY_Parser_LVAL;
#endif


/* #line 152 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 227 "header\\RdrParser.h"
#define	RULE_START	258
#define	RULE_END	259
#define	EXC_START	260
#define	EXC_END	261
#define	LBRAC	262
#define	RBRAC	263
#define	TRANSF	264
#define	ID	265
#define	IF	266
#define	THEN	267
#define	EXC	268
#define	ENTWRD	269
#define	ERROR	270
#define	INT	271
#define	STRING	272


#line 152 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
#ifndef YY_Parser_CLASS
#define YY_Parser_CLASS Parser
#endif

#ifndef YY_Parser_INHERIT
#define YY_Parser_INHERIT
#endif
#ifndef YY_Parser_MEMBERS
#define YY_Parser_MEMBERS 
#endif
#ifndef YY_Parser_LEX_BODY
#define YY_Parser_LEX_BODY  
#endif
#ifndef YY_Parser_ERROR_BODY
#define YY_Parser_ERROR_BODY  
#endif
#ifndef YY_Parser_CONSTRUCTOR_PARAM
#define YY_Parser_CONSTRUCTOR_PARAM
#endif
/* choose between enum and const */
#ifndef YY_Parser_USE_CONST_TOKEN
#define YY_Parser_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_Parser_USE_CONST_TOKEN != 0
#ifndef YY_Parser_ENUM_TOKEN
#define YY_Parser_ENUM_TOKEN yy_Parser_enum_token
#endif
#endif

class YY_Parser_CLASS YY_Parser_INHERIT
{
public: 
#if YY_Parser_USE_CONST_TOKEN != 0
/* static const int token ... */

/* #line 191 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 287 "header\\RdrParser.h"
static const int RULE_START;
static const int RULE_END;
static const int EXC_START;
static const int EXC_END;
static const int LBRAC;
static const int RBRAC;
static const int TRANSF;
static const int ID;
static const int IF;
static const int THEN;
static const int EXC;
static const int ENTWRD;
static const int ERROR;
static const int INT;
static const int STRING;


#line 191 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
 /* decl const */
#else
enum YY_Parser_ENUM_TOKEN { YY_Parser_NULL_TOKEN=0

/* #line 194 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 311 "header\\RdrParser.h"
	,RULE_START=258
	,RULE_END=259
	,EXC_START=260
	,EXC_END=261
	,LBRAC=262
	,RBRAC=263
	,TRANSF=264
	,ID=265
	,IF=266
	,THEN=267
	,EXC=268
	,ENTWRD=269
	,ERROR=270
	,INT=271
	,STRING=272


#line 194 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_Parser_PARSE(YY_Parser_PARSE_PARAM);
 virtual void YY_Parser_ERROR(char *msg) YY_Parser_ERROR_BODY;
#ifdef YY_Parser_PURE
#ifdef YY_Parser_LSP_NEEDED
 virtual int  YY_Parser_LEX(YY_Parser_STYPE *YY_Parser_LVAL,YY_Parser_LTYPE *YY_Parser_LLOC) YY_Parser_LEX_BODY;
#else
 virtual int  YY_Parser_LEX(YY_Parser_STYPE *YY_Parser_LVAL) YY_Parser_LEX_BODY;
#endif
#else
 virtual int YY_Parser_LEX() YY_Parser_LEX_BODY;
 YY_Parser_STYPE YY_Parser_LVAL;
#ifdef YY_Parser_LSP_NEEDED
 YY_Parser_LTYPE YY_Parser_LLOC;
#endif
 int YY_Parser_NERRS;
 int YY_Parser_CHAR;
#endif
#if YY_Parser_DEBUG != 0
public:
 int YY_Parser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_Parser_CLASS(YY_Parser_CONSTRUCTOR_PARAM);
public:
 YY_Parser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_Parser_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_Parser_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_Parser_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_Parser_DEBUG 
#define YYDEBUG YY_Parser_DEBUG
#endif
#endif

#endif
/* END */

/* #line 245 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.h" */
#line 383 "header\\RdrParser.h"
#endif
