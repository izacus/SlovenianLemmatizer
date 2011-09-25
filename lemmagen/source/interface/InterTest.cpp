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

#include "../header/WordList.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Calculates and outputs lemmatization accurancy from two files                 \n\
Usage: lemTest <correctfile> <lemmatizedfile>                                 \n\
                                                                              \n\
  <correctfile>           usually file containing test set that               \n\
                           learning algorithm didn't consult                  \n\
  <lemmatizedfile>       <correctfile> that was lemmatized                    \n\
                                                                              \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
  NOTE:                                                                       \n\
    Both files are in approximate multext format. Each word in separate line  \n\
    in format 'word[tab]lemma[tab]*'. <correctfile> can be created using      \n\
    'lemXval' to split larger example set. <lemmatizedfile> can be derived    \n\
    from <correctfile> lemmatizing it with 'lemmatize' option '-t wpl'.       \n\
                                                                              \n\
"

int InterTest::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;
	
	//parameters definition ------------------------------------------------------
	clParser.AddOption("correctfile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	clParser.AddOption("lemmatizedfile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");

	clParser.AddOption("verbose", optSwitch, optOptional, "-v|--verbose");
	clParser.AddOption("help", optSwitch, optOptional, "-h|--help");
	clParser.AddOption("version", optSwitch, optOptional, "--version");

	//parsing --------------------------------------------------------------------
	string sError = clParser.Parse(argc, argv);

	//errors & messages ----------------------------------------------------------
	if (clParser.GetOption("help").bSet) { cout << MSG_USAGE; return 0; }      //print usage
	if (clParser.GetOption("version").bSet) { cout << MSG_VERSION; return 0; } //print version
	if (sError != "") { cout << sError << "!\n" << MSG_HELP; return -1; }      //print error

	//ok arguments ok, read required options -------------------------------------
	string sCorfile = clParser.GetOption("correctfile").vsArgVal[0];
	string sLemfile = clParser.GetOption("lemmatizedfile").vsArgVal[0];
	bool bVerbose = clParser.GetOption("verbose").bSet;

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "       correct file: " << sCorfile << endl;
	cout << "    lemmatized file: " << sLemfile << endl;
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	cout << "processing..."  << endl;


	//OK, finnaly do the magic ---------------------------------------------------
	if (Compare(sCorfile, sLemfile, bVerbose) != 0) return -1;

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}

int InterTest::Compare(string sCorfile, string sLemfile, bool bVerbose) {
	Timing time;

	//import files	
	WordList wlstCor(sCorfile.c_str(), NULL, bVerbose);
	WordList wlstLem(sLemfile.c_str(), NULL, bVerbose);

	WordList::iterator *itBeginCor, *itEndCor;
	WordList::iterator *itBeginLem, *itEndLem;
	WordList::iterator itCor = *(itBeginCor = wlstCor.begin());
	WordList::iterator itLem = *(itBeginLem = wlstLem.begin());

	int iCorrect = 0;
	int iWrong = 0;
	while(itCor != *(itEndCor = wlstCor.end()) && itLem != *(itEndLem = wlstLem.end()) ) {
		if (strcmp(itCor->acLemm,itLem->acLemm) == 0) 
			iCorrect++;
		else
		    iWrong++;

		++itCor;
		++itLem;
	}
	delete itBeginCor; delete itEndCor;
	delete itBeginLem; delete itEndLem;

	double dAccurancy = (double) iCorrect*100 / (iCorrect + iWrong);
	cout << "findings:" << setprecision(4) << fixed << endl ;
	cout << "    correct = " << iCorrect << "" << endl;
	cout << "      wrong = " << iWrong << "" << endl;
	cout << "  accurancy = " << dAccurancy << "%" << endl;
	cout << "      error = " << 100-dAccurancy << "%" << endl;

	if (bVerbose) {
		time.Print(cout, "  Time needed ");
	}

	return 0;
}
