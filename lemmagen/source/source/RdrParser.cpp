#define YY_Parser_h_included

/*  A Bison++ parser, made from definition\RdrParser.def  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */


#line 1 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
//Added by MJ for compatibility with VisualStudio2005 Compiler
#define _WINDOWS

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
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
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

/* #line 76 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 88 "source\\RdrParser.cpp"
#line 23 "definition\\RdrParser.def"
 
	#include "../header/RdrTree.h"
	
	struct yy_token_detail
	{ int *piLine, *piColm, *piLeng;
	  byte **pacText;
	};
		
#line 32 "definition\\RdrParser.def"

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

#line 76 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
/* %{ and %header{ and %union, during decl */
#define YY_Parser_BISON 1
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
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_Parser_STYPE 
#define YY_Parser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_Parser_DEBUG
#define  YY_Parser_DEBUG YYDEBUG
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

/* #line 120 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 182 "source\\RdrParser.cpp"

#line 120 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
/*  YY_Parser_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */

/* #line 124 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 191 "source\\RdrParser.cpp"

#line 124 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
/* prefix */
#ifndef YY_Parser_DEBUG

/* #line 126 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 198 "source\\RdrParser.cpp"

#line 126 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
/* YY_Parser_DEBUG */
#endif


#ifndef YY_Parser_LSP_NEEDED

/* #line 131 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 208 "source\\RdrParser.cpp"

#line 131 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
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
      /* We used to use `unsigned long' as YY_Parser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

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
#if YY_Parser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_Parser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_Parser_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_Parser_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_Parser_PURE
#ifndef YYPURE
#define YYPURE YY_Parser_PURE
#endif
#endif
#ifdef YY_Parser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_Parser_DEBUG 
#endif
#endif
#ifndef YY_Parser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_Parser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_Parser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_Parser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */

/* #line 248 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 330 "source\\RdrParser.cpp"
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


#line 248 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
 /* #defines tokens */
#else
/* CLASS */
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
#ifndef YY_Parser_CONSTRUCTOR_CODE
#define YY_Parser_CONSTRUCTOR_CODE
#endif
#ifndef YY_Parser_CONSTRUCTOR_INIT
#define YY_Parser_CONSTRUCTOR_INIT
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

/* #line 292 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 395 "source\\RdrParser.cpp"
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


#line 292 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
 /* decl const */
#else
enum YY_Parser_ENUM_TOKEN { YY_Parser_NULL_TOKEN=0

/* #line 295 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 419 "source\\RdrParser.cpp"
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


#line 295 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_Parser_PARSE (YY_Parser_PARSE_PARAM);
 virtual void YY_Parser_ERROR(char *msg) YY_Parser_ERROR_BODY;
#ifdef YY_Parser_PURE
#ifdef YY_Parser_LSP_NEEDED
 virtual int  YY_Parser_LEX (YY_Parser_STYPE *YY_Parser_LVAL,YY_Parser_LTYPE *YY_Parser_LLOC) YY_Parser_LEX_BODY;
#else
 virtual int  YY_Parser_LEX (YY_Parser_STYPE *YY_Parser_LVAL) YY_Parser_LEX_BODY;
#endif
#else
 virtual int YY_Parser_LEX() YY_Parser_LEX_BODY;
 YY_Parser_STYPE YY_Parser_LVAL;
#ifdef YY_Parser_LSP_NEEDED
 YY_Parser_LTYPE YY_Parser_LLOC;
#endif
 int   YY_Parser_NERRS;
 int    YY_Parser_CHAR;
#endif
#if YY_Parser_DEBUG != 0
 int YY_Parser_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_Parser_CLASS(YY_Parser_CONSTRUCTOR_PARAM);
public:
 YY_Parser_MEMBERS 
};
/* other declare folow */
#if YY_Parser_USE_CONST_TOKEN != 0

/* #line 326 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 471 "source\\RdrParser.cpp"
const int YY_Parser_CLASS::RULE_START=258;
const int YY_Parser_CLASS::RULE_END=259;
const int YY_Parser_CLASS::EXC_START=260;
const int YY_Parser_CLASS::EXC_END=261;
const int YY_Parser_CLASS::LBRAC=262;
const int YY_Parser_CLASS::RBRAC=263;
const int YY_Parser_CLASS::TRANSF=264;
const int YY_Parser_CLASS::ID=265;
const int YY_Parser_CLASS::IF=266;
const int YY_Parser_CLASS::THEN=267;
const int YY_Parser_CLASS::EXC=268;
const int YY_Parser_CLASS::ENTWRD=269;
const int YY_Parser_CLASS::ERROR=270;
const int YY_Parser_CLASS::INT=271;
const int YY_Parser_CLASS::STRING=272;


#line 326 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
 /* const YY_Parser_CLASS::token */
#endif
/*apres const  */
YY_Parser_CLASS::YY_Parser_CLASS(YY_Parser_CONSTRUCTOR_PARAM) YY_Parser_CONSTRUCTOR_INIT
{
#if YY_Parser_DEBUG != 0
YY_Parser_DEBUG_FLAG=0;
#endif
YY_Parser_CONSTRUCTOR_CODE;
};
#endif

/* #line 337 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 503 "source\\RdrParser.cpp"


#define	YYFINAL		47
#define	YYFLAG		32768
#define	YYNTBASE	18

#define YYTRANSLATE(x) ((unsigned)(x) <= 272 ? yytranslate[x] : 32)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17
};

#if YY_Parser_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     6,     8,    12,    16,    19,    22,    25,    30,
    33,    34,    37,    40,    43,    46,    49,    51,    55,    57,
    61,    64,    65,    69,    70,    73,    75,    77
};

static const short yyrhs[] = {    17,
     0,     7,    17,     8,     0,    16,     0,     7,    16,     8,
     0,    17,     9,    17,     0,    10,    18,     0,    11,    18,
     0,    12,    20,     0,    12,     7,    20,     8,     0,    13,
    19,     0,     0,    25,    21,     0,    25,    22,     0,    25,
    23,     0,    25,    24,     0,    25,    14,     0,    25,     0,
     7,    25,     8,     0,     1,     0,     3,    26,     4,     0,
    27,    29,     0,     0,     5,    30,     6,     0,     0,    30,
    28,     0,     1,     0,    28,     0,     1,     0
};

#endif

#if YY_Parser_DEBUG != 0
static const short yyrline[] = { 0,
   103,   104,   106,   107,   110,   116,   118,   120,   121,   123,
   126,   128,   137,   146,   155,   158,   165,   167,   169,   174,
   195,   214,   217,   222,   225,   232,   235,   239
};

static const char * const yytname[] = {   "$","error","$illegal.","RULE_START",
"RULE_END","EXC_START","EXC_END","LBRAC","RBRAC","TRANSF","ID","IF","THEN","EXC",
"ENTWRD","ERROR","INT","STRING","string","int","thenrule","id","if","then","exc",
"proplist","propert","rule","rdr","except","rdrlist","file",""
};
#endif

static const short yyr1[] = {     0,
    18,    18,    19,    19,    20,    21,    22,    23,    23,    24,
    25,    25,    25,    25,    25,    25,    26,    26,    26,    27,
    28,    29,    29,    30,    30,    30,    31,    31
};

static const short yyr2[] = {     0,
     1,     3,     1,     3,     3,     2,     2,     2,     4,     2,
     0,     2,     2,     2,     2,     2,     1,     3,     1,     3,
     2,     0,     3,     0,     2,     1,     1,     1
};

static const short yydefact[] = {     0,
    28,     0,    22,    27,    19,    11,    17,     0,     0,    21,
     0,     0,     0,     0,     0,    16,    12,    13,    14,    15,
    20,    26,     0,    18,     0,     1,     6,     7,     0,     0,
     8,     0,     3,    10,    23,    25,     0,     0,     0,     0,
     2,     9,     5,     4,     0,     0,     0
};

static const short yydefgoto[] = {    27,
    34,    31,    17,    18,    19,    20,     7,     8,     3,     4,
    10,    23,    45
};

static const short yypact[] = {    34,
-32768,    -1,    -4,-32768,-32768,-32768,    20,     4,    23,-32768,
     9,    -3,    -3,    -2,     0,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,    22,-32768,   -15,-32768,-32768,-32768,     1,    18,
-32768,    24,-32768,-32768,-32768,-32768,    28,    30,    25,    31,
-32768,-32768,-32768,-32768,    41,    43,-32768
};

static const short yypgoto[] = {    32,
-32768,    15,-32768,-32768,-32768,-32768,    40,-32768,-32768,    26,
-32768,-32768,-32768
};


#define	YYLAST		49


static const short yytable[] = {     5,
     9,    37,   -11,    25,    29,     6,    32,    21,   -11,   -11,
   -11,   -11,   -11,    26,    30,    33,    24,    30,    12,    13,
    14,    15,    16,    22,     2,   -24,    39,    35,   -24,    12,
    13,    14,    15,    16,     1,    41,     2,    42,    44,    40,
    46,    43,    47,    38,    28,    11,     0,     0,    36
};

static const short yycheck[] = {     1,
     5,    17,     4,     7,     7,     7,     7,     4,    10,    11,
    12,    13,    14,    17,    17,    16,     8,    17,    10,    11,
    12,    13,    14,     1,     3,     3,     9,     6,     6,    10,
    11,    12,    13,    14,     1,     8,     3,     8,     8,    16,
     0,    17,     0,    29,    13,     6,    -1,    -1,    23
};

#line 337 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_Parser_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_Parser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_Parser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_Parser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_Parser_CHAR = (token), YY_Parser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_Parser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_Parser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_Parser_PURE
/* UNPURE */
#define YYLEX           YY_Parser_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_Parser_CHAR;                      /*  the lookahead symbol        */
YY_Parser_STYPE      YY_Parser_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_Parser_NERRS;                 /*  number of parse errors so far */
#ifdef YY_Parser_LSP_NEEDED
YY_Parser_LTYPE YY_Parser_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_Parser_LSP_NEEDED
#define YYLEX           YY_Parser_LEX(&YY_Parser_LVAL, &YY_Parser_LLOC)
#else
#define YYLEX           YY_Parser_LEX(&YY_Parser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_Parser_DEBUG != 0
int YY_Parser_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_Parser_CLASS::
#endif
     YY_Parser_PARSE(YY_Parser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_Parser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_Parser_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_Parser_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_Parser_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_Parser_LSP_NEEDED
  YY_Parser_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_Parser_LTYPE *yyls = yylsa;
  YY_Parser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_Parser_PURE
  int YY_Parser_CHAR;
  YY_Parser_STYPE YY_Parser_LVAL;
  int YY_Parser_NERRS;
#ifdef YY_Parser_LSP_NEEDED
  YY_Parser_LTYPE YY_Parser_LLOC;
#endif
#endif

  YY_Parser_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    YY_Parser_OUTPUT_START "Starting parse\n" YY_Parser_OUTPUT_END;
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_Parser_NERRS = 0;
  YY_Parser_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_Parser_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_Parser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_Parser_LSP_NEEDED
      YY_Parser_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_Parser_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_Parser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_Parser_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_Parser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_Parser_LSP_NEEDED
      yyls = (YY_Parser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_Parser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_Parser_DEBUG != 0
      if (YY_Parser_DEBUG_FLAG)
	YY_Parser_OUTPUT_START "Stack size increased to %d\n", yystacksize YY_Parser_OUTPUT_END;
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    YY_Parser_OUTPUT_START "Entering state %d\n", yystate YY_Parser_OUTPUT_END;
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_Parser_CHAR == YYEMPTY)
    {
#if YY_Parser_DEBUG != 0
      if (YY_Parser_DEBUG_FLAG)
	YY_Parser_OUTPUT_START "Reading a token: " YY_Parser_OUTPUT_END;
#endif
      YY_Parser_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_Parser_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_Parser_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_Parser_DEBUG != 0
      if (YY_Parser_DEBUG_FLAG)
	YY_Parser_OUTPUT_START "Now at end of input.\n" YY_Parser_OUTPUT_END;
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_Parser_CHAR);

#if YY_Parser_DEBUG != 0
      if (YY_Parser_DEBUG_FLAG)
	{
	  YY_Parser_OUTPUT_START "Next token is %d (%s", YY_Parser_CHAR, yytname[yychar1] YY_Parser_OUTPUT_END;
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_Parser_CHAR, YY_Parser_LVAL);
#endif
	  YY_Parser_OUTPUT_START ")\n" YY_Parser_OUTPUT_END;
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    YY_Parser_OUTPUT_START "Shifting token %d (%s), ", YY_Parser_CHAR, yytname[yychar1] YY_Parser_OUTPUT_END;
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_Parser_CHAR != YYEOF)
    YY_Parser_CHAR = YYEMPTY;

  *++yyvsp = YY_Parser_LVAL;
#ifdef YY_Parser_LSP_NEEDED
  *++yylsp = YY_Parser_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    {
      int i;

      YY_Parser_OUTPUT_START "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn] YY_Parser_OUTPUT_END;

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	YY_Parser_OUTPUT_START "%s ", yytname[yyrhs[i]] YY_Parser_OUTPUT_END;
      YY_Parser_OUTPUT_START " -> %s\n", yytname[yyr1[yyn]] YY_Parser_OUTPUT_END;
    }
#endif


/* #line 823 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 1124 "source\\RdrParser.cpp"

  switch (yyn) {

case 1:
#line 103 "definition\\RdrParser.def"
{	yyval.acVal = yylval.acVal; ;
    break;}
case 2:
#line 104 "definition\\RdrParser.def"
{	yyval.acVal = yylval.acVal; ;
    break;}
case 3:
#line 106 "definition\\RdrParser.def"
{	yyval.iVal = yylval.iVal; ;
    break;}
case 4:
#line 107 "definition\\RdrParser.def"
{	yyval.iVal = yylval.iVal; ;
    break;}
case 5:
#line 110 "definition\\RdrParser.def"
{	int iEqLen = Str::eqPrefixLen((const char*) yyvsp[-2].acVal, (const char*) yyvsp[0].acVal);
										yyval.rrVal = new RdrRule(strlen((const char*) yyvsp[-2].acVal)-iEqLen, Str::copy(&yyvsp[0].acVal[iEqLen]));
										delete[] yyvsp[-2].acVal;
										delete[] yyvsp[0].acVal;
									;
    break;}
case 6:
#line 116 "definition\\RdrParser.def"
{	yyval.acVal = yyvsp[0].acVal; ;
    break;}
case 7:
#line 118 "definition\\RdrParser.def"
{	yyval.acVal = yyvsp[0].acVal; ;
    break;}
case 8:
#line 120 "definition\\RdrParser.def"
{	yyval.rrVal = yyvsp[0].rrVal; ;
    break;}
case 9:
#line 121 "definition\\RdrParser.def"
{	yyval.rrVal = yyvsp[-1].rrVal; ;
    break;}
case 10:
#line 123 "definition\\RdrParser.def"
{	yyval.iVal = yyvsp[0].iVal; ;
    break;}
case 11:
#line 126 "definition\\RdrParser.def"
{	yyval.rtVal = new RdrTree(); 
									;
    break;}
case 12:
#line 128 "definition\\RdrParser.def"
{	//check if this field isn't filed already
										yyval.rtVal = yyvsp[-1].rtVal;
										if (yyval.rtVal->acId == NULL) yyval.rtVal->acId = yyvsp[0].acVal;
										else {
											YY_Parser_ERROR("property 'name' was already set");
											delete yyval.rtVal;
											YYERROR;
										};
									;
    break;}
case 13:
#line 137 "definition\\RdrParser.def"
{	//check if this field isn't filed already
										yyval.rtVal = yyvsp[-1].rtVal;
										if (yyval.rtVal->acEnding == NULL) yyval.rtVal->SetEnding(yyvsp[0].acVal);
										else {
											YY_Parser_ERROR("property 'if' was already set");
											delete yyval.rtVal;
											YYERROR;
										};
									;
    break;}
case 14:
#line 146 "definition\\RdrParser.def"
{	//check if this field isn't filed already
										yyval.rtVal = yyvsp[-1].rtVal;
										if (yyval.rtVal->rrRule == NULL) yyval.rtVal->rrRule = yyvsp[0].rrVal;
										else {
											YY_Parser_ERROR("property 'then' was already set");
											delete yyval.rtVal;
											YYERROR;	
										};	
									;
    break;}
case 15:
#line 155 "definition\\RdrParser.def"
{	/*nothing to do, ignore number of exceptions*/
										yyval.rtVal = yyvsp[-1].rtVal;
									;
    break;}
case 16:
#line 158 "definition\\RdrParser.def"
{	/*save flag entireword */
										yyval.rtVal = yyvsp[-1].rtVal;
										yyval.rtVal->bEntireWord = true;
									;
    break;}
case 17:
#line 165 "definition\\RdrParser.def"
{	yyval.rtVal = yyvsp[0].rtVal;
									;
    break;}
case 18:
#line 167 "definition\\RdrParser.def"
{	yyval.rtVal = yyvsp[-1].rtVal;
									;
    break;}
case 19:
#line 169 "definition\\RdrParser.def"
{	//error recovery (signalize error)
										yyval.rtVal = NULL;
									;
    break;}
case 20:
#line 175 "definition\\RdrParser.def"
{	//check if we got regular RdrTree and that 
										//all requierd fields are filed
										yyerrok;
										yyval.rtVal = yyvsp[-1].rtVal;
										if (yyval.rtVal==NULL) break;
										
										if (yyval.rtVal->acEnding == NULL || yyval.rtVal->rrRule == NULL) {
											if (yyval.rtVal->acEnding == NULL)
												YY_Parser_ERROR("property 'if' is mandatory");
											if (yyval.rtVal->rrRule == NULL)
												YY_Parser_ERROR("property 'then' is mandatory");
												
											delete yyval.rtVal; 
											yyval.rtVal = NULL;
											YYERROR;
											break;
										}
									;
    break;}
case 21:
#line 195 "definition\\RdrParser.def"
{	//check if we got regular RdrTree
										yyerrok;
										yyval.rtVal = yyvsp[-1].rtVal;
										
										if (yyval.rtVal==NULL) {
											if (yyvsp[0].deqVal!=NULL) delete yyvsp[0].deqVal;
											break;
										}
										
										if (yyvsp[0].deqVal==NULL) {
											yyval.rtVal->deqSubNode = NULL;
											break;
										}
											
										//create exception table if exceptions exist
										yyval.rtVal->deqSubNode = yyvsp[0].deqVal;
									;
    break;}
case 22:
#line 214 "definition\\RdrParser.def"
{	//no exceptions
										yyval.deqVal = NULL;
									;
    break;}
case 23:
#line 218 "definition\\RdrParser.def"
{	yyval.deqVal = yyvsp[-1].deqVal;
									;
    break;}
case 24:
#line 222 "definition\\RdrParser.def"
{	//no exceptions
										yyval.deqVal = NULL;
									;
    break;}
case 25:
#line 225 "definition\\RdrParser.def"
{	//create exception list if it doesn't exist
										if(yyvsp[-1].deqVal == NULL) yyval.deqVal = new RdrTree::deqrt();
										else yyval.deqVal = yyvsp[-1].deqVal;
										
										//add new rdr to list 
										if (yyvsp[0].rtVal!=NULL) yyval.deqVal->push_back(yyvsp[0].rtVal);
									;
    break;}
case 26:
#line 232 "definition\\RdrParser.def"
{   YY_Parser_ERROR("error parsing list of rdr rules!"); ;
    break;}
case 27:
#line 235 "definition\\RdrParser.def"
{	//return top rule
										rtTop = yyvsp[0].rtVal;
										bEOF = true;
									;
    break;}
case 28:
#line 239 "definition\\RdrParser.def"
{   bEOF = true;
	                                ;
    break;}
}

#line 823 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc"
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_Parser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      YY_Parser_OUTPUT_START "state stack now" YY_Parser_OUTPUT_END;
      while (ssp1 != yyssp)
	YY_Parser_OUTPUT_START " %d", *++ssp1 YY_Parser_OUTPUT_END;
      YY_Parser_OUTPUT_START "\n" YY_Parser_OUTPUT_END;
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_Parser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_Parser_LLOC.first_line;
      yylsp->first_column = YY_Parser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_Parser_NERRS;

#ifdef YY_Parser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_Parser_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_Parser_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_Parser_ERROR_VERBOSE */
	YY_Parser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_Parser_CHAR == YYEOF)
	YYABORT;

#if YY_Parser_DEBUG != 0
      if (YY_Parser_DEBUG_FLAG)
	YY_Parser_OUTPUT_START "Discarding token %d (%s).\n", YY_Parser_CHAR, yytname[yychar1] YY_Parser_OUTPUT_END;
#endif

      YY_Parser_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_Parser_LSP_NEEDED
  yylsp--;
#endif

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      YY_Parser_OUTPUT_START "Error: state stack now" YY_Parser_OUTPUT_END;
      while (ssp1 != yyssp)
	YY_Parser_OUTPUT_START " %d", *++ssp1 YY_Parser_OUTPUT_END;
      YY_Parser_OUTPUT_START "\n" YY_Parser_OUTPUT_END;
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_Parser_DEBUG != 0
  if (YY_Parser_DEBUG_FLAG)
    YY_Parser_OUTPUT_START "Shifting error token, " YY_Parser_OUTPUT_END;
#endif

  *++yyvsp = YY_Parser_LVAL;
#ifdef YY_Parser_LSP_NEEDED
  *++yylsp = YY_Parser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */

/* #line 1022 "Q:\\01 Dokumenti\\IJS\\Projects\\LemmaGen\\support\\lex-bison\\binary\\win32\\bison\\bison.cc" */
#line 1530 "source\\RdrParser.cpp"
#line 243 "definition\\RdrParser.def"

//-- Additional C code  -------------------------------------------------------

