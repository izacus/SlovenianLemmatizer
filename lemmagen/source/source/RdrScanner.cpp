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
#include "../header/RdrScanner.h"

RdrScanner::RdrScanner(): RdrParser(), RdrLexer(&YY_Parser_LVAL, &YY_Parser_LLOC){
	bSuccess = true;

	iMaxError = 99;
	iMaxErrorOut = 7;
	this->acInFileName = NULL;
}

RdrScanner::~RdrScanner() {
	lstError.clear();
}


RdrTree *RdrScanner::scan(const char *acInFileName, ostream *osErr, int iVerbose) {
	this->acInFileName = (char *) acInFileName;
	ifstream *ifsInFile = new ifstream(acInFileName);
	
	if (ifsInFile==NULL) {
		(*osErr) << "Error while opening file \n" << acInFileName << "\n. Parsing aborted!";
		bSuccess = false;
		return NULL;
	}

	RdrTree * rtReturn = scan(ifsInFile, osErr, iVerbose);
	ifsInFile->close();

	return rtReturn;
}

RdrTree *RdrScanner::scan(istream *isInput, ostream *osErr, int iVerbose) {
	yyin = isInput;

	RdrLexer::yyout = osErr;
	RdrParser::yyout = osErr;
	this->osErr = osErr;
	bSuccess = true;

	RdrTree *rt = NULL;

	Timing time;
	if (iVerbose>0) (*osErr) << "Starting tree parsing from the input stream..." << endl;

	do
		yyparse();
	while(!bEOF && !bSuccess && lstError.size()<=iMaxError);
	(*osErr).flush();

	if (iVerbose>0) time.Print((*osErr), "  Time needed for parsing ");

	if (bSuccess) {
		//no errors
		rt = rtTop;
		if (iVerbose>1) rt->PrintStat((*osErr));

	} else {
		//prepare error report
		getErrorReport(*isInput, *osErr);

		rt = NULL;
	}

	return rt;
}

void RdrScanner::yyerror_parser(char *acMsg) {
	bSuccess = false;
	lstError.push_back(new Error(*(yylloc.piLine), *(yylloc.piColm), *(yylloc.piLeng), (char*) *(yylloc.pacText), acMsg));
}

int RdrScanner::yylex_parser() {
	return yylex_lexer();
}

void RdrScanner::getErrorReport(istream &is, ostream &osErr) {
	istream *isThis = NULL;
	stringstream ss;
	if (typeid(is) == typeid(ss)) {
		isThis = &is;
		((stringstream *) isThis)->seekg(ios_base::beg);
	} else
	if (this->acInFileName != NULL) 
		isThis = new ifstream(this->acInFileName);

	int iOutputErrors = min((int) iMaxErrorOut, (int) lstError.size());

	osErr << "Parse unsuccessful!\n";
	osErr << "  Encountered ";

	if(lstError.size()>iMaxError)
		osErr << "more than " << iMaxError;
	else
		osErr << lstError.size();
		
	osErr << " error(s).\n";
	osErr << "  Output of first " << iOutputErrors << " error(s):\n";

	int iLine = 0;
	char *acLine;
	string sLine;
	
	for(int i=0; i<iOutputErrors; i++) {
		while (isThis != NULL && iLine < lstError.front()->iLine) {
			getline(*isThis, sLine);
			iLine++;
		}
		osErr << "  " <<
			"[#" << (i+1) << "] " <<
			lstError.front()->sMsg << "! (" <<
			"line:" << lstError.front()->iLine	<< ", " <<
			"col:" << lstError.front()->iColm << ", " <<
			"symb:\"" << lstError.front()->sSym << "\")\n";

		//if it is possible then report line containing error
		if (isThis != NULL) {
			string sLineNew = sLine;
			string sLinePos;
			int iPos = 0;
			for(string::iterator sIter = sLineNew.begin(); sIter!= sLineNew.end(); sIter++, iPos++)
				if ((*sIter)=='\t' || (*sIter)=='\f' ||	(*sIter)=='\b' ||
					(*sIter)=='\v' || (*sIter)=='\r' ||	(*sIter)=='\n') (*sIter) = ' ';
			sLinePos = string(max(lstError.front()->iColm,1)-1,' ') + string(lstError.front()->iLeng,'^');

			if (sLineNew.length()>=67) {
				int iCenter = lstError.front()->iColm + lstError.front()->iLeng/2;
				int iStart = max(0,iCenter-37);
				bool bStart = iStart!=0;
				int iEnd = min((int) sLineNew.length(),iStart+67+ (bStart?0:3));
				bool bEnd = iEnd!=sLineNew.length();

				sLineNew = sLineNew.substr(iStart, iEnd-iStart);
				sLinePos = sLinePos.substr(iStart);

				if (bStart) {
					sLineNew = "..." + sLineNew;
					sLinePos = "   " + sLinePos;
				}
				if (bEnd)
					sLineNew += "...";
			}

			osErr << "      " << sLineNew << "\n";
			osErr << "      " << sLinePos << "\n";
		}

		lstError.pop_front();
	}
	
	if (this->acInFileName != NULL) 
		((ifstream*) isThis)->close();

	lstError.clear();
}

