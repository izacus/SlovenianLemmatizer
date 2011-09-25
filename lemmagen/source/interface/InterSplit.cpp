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
Splits example data file into smaller subsets for cross validation            \n\
Usage: lemSplit -k <split> [-t <type>] <infile> <outfilemask>                 \n\
                                                                              \n\
  -k, --split <split>    type 's','d': number of required subsets (cross)     \n\
                         type 'p','r': percent of words to be in test set     \n\
  -t, --type <type>      specify type of split (default = deep), valid:       \n\
     s|shallow             split set according to example lines               \n\
     d|deep                split set according to example contents, all examp.\n\
                           with same word and lemma are in the same subset    \n\
     p|perc                split on 2 subsets using percent for test set      \n\
     r|percdeep            combination of percent and deep functionality      \n\
                                                                              \n\
  -r, --randseed <int>   seed for random number genarator (default=1), if     \n\
                           this switch is used with (<int>=0), time is used to\n\
                           generate seed, random is used for for spliting sets\n\
                                                                              \n\
  <infile>               input file (multext format)                          \n\
  <outfilemask>          output file mask ('#' replaced with sequence number) \n\
                                                                              \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterSplit::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;

	//parameters definition ------------------------------------------------------
	clParser.AddOption("split", optSwitch, optMandatory, "-k|--split");
	clParser.LastAdded().SetArgType(argInteger, 1, 1, "", "");
	clParser.AddOption("type", optSwitch, optOptional, "-t|--type");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "s|d|p|r|shallow|deep|perc|percdeep", "deep");

	clParser.AddOption("randseed", optSwitch, optOptional, "-r|--randseed");
	clParser.LastAdded().SetArgType(argInteger, 1, 1, "", "");

	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	clParser.AddOption("outfilemask", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argOutFile, 1, 1, "", "");

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
	string sInfile = clParser.GetOption("infile").vsArgVal[0];
	string sOutfileMask = clParser.GetOption("outfilemask").vsArgVal[0];
	int iSplit = clParser.GetOption("split").GetInteger();
	int iType = clParser.GetOption("type").GetAllowedIndex()%4;
	int iRandSeed  = abs(clParser.GetOption("randseed").GetInteger());	
	bool bVerbose = clParser.GetOption("verbose").bSet;

	//check for logical errors in parameters -------------------------------------
	if ( (iType <2 && (iSplit<2 || iSplit>500)) ||
		 (iType>=2 && (iSplit<1 || iSplit>99)) ) {
		cout << "error in argument 'split':\n  if 'type'=s|d then allowed values are 2..200\n  if 'type'=p|r then allowed values are 1..99" << endl;
		return -1;
	}

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "         input file: " << sInfile << endl;
	cout << "   output file mask: " << sOutfileMask << endl;
	cout << "      type of split: "; 
	if (iType == 0) cout << "shallow (" << iSplit << " sets)"; 
	if (iType == 1) cout << "deep (" << iSplit << " sets)";
	if (iType == 2) cout << "percent (" << iSplit << "% in test set)";
	if (iType == 3) cout << "percent deep (" << iSplit << "% in test set)";
	cout << endl;
	cout << "        random seed: " << iRandSeed << endl;
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	cout << "processing..."  << endl;


	//OK, finnaly do the magic ---------------------------------------------------
	if (Split(sInfile, sOutfileMask, iSplit, iType, iRandSeed, bVerbose) != 0) return -1;

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}


int InterSplit::Split(string sInfile, string sOutfileMask, int iSplit, int iType, int iRandSeed, bool bVerbose) {
	//randomization
	Rand::seed(iRandSeed);
	
	//timing
	Timing time;

	//import files	
	WordList wlst(sInfile.c_str(), NULL, bVerbose);
	if (iType<2)
		wlst.SplitCrossValid(iSplit, iType, bVerbose*2);
	else
		wlst.SplitSubset(100-iSplit, iType%2, bVerbose*2);


	for (int iFile = 0; iFile < (iType<2 ? iSplit : 2); ++iFile) {
		stringstream ssFileNum;
		if (iType<2)
			ssFileNum << (iFile+1) << "of" << iSplit;
		else 
			ssFileNum << (iFile==0 ? "train" : "test");

		string sOutpuFile = sOutfileMask;
		int iFound = sOutfileMask.find_first_of('#');
		if (iFound == string::npos)
			sOutpuFile += "." + ssFileNum.str();
		else
			sOutpuFile.replace(iFound,1, ssFileNum.str());

		if (bVerbose) cout << "  Generating file '" << sOutpuFile << "' ..." << endl;

		ofstream ofs(sOutpuFile.c_str(), ios_base::out);

		WordList::iterator *itBegin, *itEnd;

		for(WordList::iterator it = *(itBegin = wlst.begin(iFile,true)); it != *(itEnd = wlst.end(iFile,true)); ++it)
			ofs << it->ToString(false, NULL) << "\n";
		ofs.close();
		delete itBegin;	delete itEnd;
	}

	if (bVerbose) {
		time.Print(cout, "  Time needed altogether ");
	}

	return 0;
}

