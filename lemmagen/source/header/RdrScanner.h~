/******************************************************************************
This file is part of the lemmagen library. It gives support for lemmatization.
Copyright (C) 2006-2007 Matjaz Jursic <matjaz@gmail.com>

The lemmagen library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/
#pragma once

#include "RdrLexer.h"

class RdrScanner:  private RdrParser, private RdrLexer{

//struct-----------------------------------------------
private:
	struct Error{
		int iLine;
		int iColm;
		int iLeng;
		string sSym;
		string sMsg;
		
		Error(int iLine, int iColm, int iLeng, char *acSym, char *acMsg) {
			this->iLine = iLine;
			this->iColm = iColm-iLeng;
			this->iLeng = iLeng;
			this->sSym = string(acSym);
			this->sMsg = string(acMsg);
		}
	};

//typedef----------------------------------------------
public:
	typedef map<byte*,RdrRule*,Str::LowerByteStr> mapcr;
private:
	typedef list<Error *> liste;

//variables--------------------------------------------
public:
	bool bSuccess;
	int iMaxError;
	int iMaxErrorOut;
private:
	ostream *osErr;

	liste lstError;
	char *acInFileName;

//code-------------------------------------------------
public:
	RdrScanner();
	~RdrScanner();

	RdrTree *scan(const char *acInFileName, ostream *osErr = &cerr, int iVerbose = 0);
	RdrTree *scan(istream *isInput, ostream *osErr = &cerr, int iVerbose = 0);

private:
	void yyerror_parser(char *s);
	int yylex_parser();
	void getErrorReport(istream &is, ostream &osErr);
};
