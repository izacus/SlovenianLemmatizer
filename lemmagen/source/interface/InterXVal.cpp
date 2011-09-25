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

#include "../header/Xval.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Validates quality of learning according to chosen options:                    \n\
   if type of split is shallow or deep then it uses cross validation else     \n\
   it just tests test set on the rules that were learned on the train set     \n\
                                                                              \n\
Usage: lemXval -k <split> [-t <type>] [-a <alg>] [-o <outfile>] <infile>      \n\
                                                                              \n\
  -k, --split <split>    type 's','d': number of required subsets (cross)     \n\
                         type 'p','r': percent of words to be in test set     \n\
  -a, --algorithm <alg>  algorithm to be used in learning (default=cover)     \n\
     cover                 newly developed, faster and usually more accurate  \n\
     rdr                   original that strictly folows rdr metodology       \n\
     both                  both stated above                                  \n\
                                                                              \n\
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
  -n, --numruns <int>    number of repetitions of cross validation (default=1)\n\
                           useful for more accurate statistics                \n\
                                                                              \n\
  -o, --out <outfile>    output file (print statistics to the file            \n\
  <infile>               input file (multext format)                          \n\
                                                                              \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
  -s, --silent           print just basic information                         \n\
                                                                              \n\
"

int InterXval::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;

	//parameters definition ------------------------------------------------------
	clParser.AddOption("split", optSwitch, optMandatory, "-k|--split");
	clParser.LastAdded().SetArgType(argInteger, 1, 1, "", "");
	clParser.AddOption("type", optSwitch, optOptional, "-t|--type");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "s|d|p|r|shallow|deep|perc|percdeep", "deep");

	clParser.AddOption("algorithm", optSwitch, optOptional, "-a|--algorithm");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "cover|rdr|both", "cover");

	clParser.AddOption("randseed", optSwitch, optOptional, "-r|--randseed");
	clParser.LastAdded().SetArgType(argInteger, 1, 1, "", "");
	clParser.AddOption("numruns", optSwitch, optOptional, "-n|--numruns");
	clParser.LastAdded().SetArgType(argInteger, 1, 1, "", "");

	clParser.AddOption("silent", optSwitch, optOptional, "-s|--silent");

	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	clParser.AddOption("outfile", optSwitch, optOptional, "-o|--out");
	clParser.LastAdded().SetArgType(argOutFileApp, 1, 1, "", "");

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
	string sOutfile = clParser.GetOption("outfile").vsArgVal[0];

	int iRandSeed  = abs(clParser.GetOption("randseed").GetInteger());	
	int iNumRuns  = abs(clParser.GetOption("numruns").GetInteger());	

    bool bSilent = clParser.GetOption("silent").bSet;
	int iAlg = clParser.GetOption("algorithm").GetAllowedIndex();

	int iSplit = clParser.GetOption("split").GetInteger();
	int iType = clParser.GetOption("type").GetAllowedIndex()%4;
	bool bVerbose = clParser.GetOption("verbose").bSet;

	//check for logical errors in parameters -------------------------------------
	if ( (iType <2 && (iSplit<2 || iSplit>100000)) ||
		 (iType>=2 && (iSplit<0 || iSplit>99)) ) {
		cout << "error in argument 'split':\n  if 'type'=s|d then allowed values are 2..100000\n  if 'type'=p|r then allowed values are 0..99" << endl;
		return -1;
	}

	//output setting -------------------------------------------------------------
	if (!bSilent) {
		cout << "chosen options:"  << endl;
		cout << "         input file: " << sInfile << endl;
		cout << "        output file: " << (sOutfile=="" ? "none" : sOutfile) << endl; 
		cout << "      type of split: "; 
		if (iType == 0) cout << "shallow (" << iSplit << " sets)"; 
		if (iType == 1) cout << "deep (" << iSplit << " sets)";
		if (iType == 2) cout << "percent (" << iSplit << "% in test set)";
		if (iType == 3) cout << "percent deep (" << iSplit << "% in test set)";
		cout << endl;
		cout << "        random seed: " << iRandSeed << endl;
	    cout << "     number of runs: " << iNumRuns << endl;
		cout << "          algorithm: ";
			if (iAlg == 0) cout << "new covering";
			if (iAlg == 1) cout << "original rdr";
			if (iAlg == 2) cout << "new covering & original rdr";
		cout << endl;
		cout << "             silent: " << (string)(bSilent ? "yes" : "no") << endl;
		cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
		cout << "processing..."  << endl;
	}

	//OK, finnaly do the magic ---------------------------------------------------
	if (Xval(sInfile, sOutfile, iSplit, iType, iAlg, iRandSeed, bVerbose, bSilent, iNumRuns) != 0) return -1;

	//report success -------------------------------------------------------------
	if (!bSilent) cout << "finished successfully"  << endl;
	return 0;
}

int InterXval::Xval(string sInfile, string sOutfile, int iSplit, int iType, int iAlg, 
					int iRandSeed, bool bVerbose, bool bSilent, int iNumRuns) {
	return Xval::PerformXval(sInfile, sOutfile, iSplit, iType, iAlg, iRandSeed, bVerbose, bSilent, iNumRuns);
}



