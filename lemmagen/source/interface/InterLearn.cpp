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

#include "../header/CoverNodeAlg.h"
#include "../header/BaseAlg.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Learns RDR tree from file of examples formated in multext standard            \n\
Usage: lemLearn [-f <format>] [-a <alg>-] -o <outfile> <infile> [<infile2>,..]\n\
                                                                              \n\
  -f, --format <format>  specify output tree formating (default=4)            \n\
     1,2,3,4,5,6           6 possible formats, try them to find the one that  \n\
                           suits your needs best, all are valid for 'lemBuild'\n\
  -a, --algorithm <alg>  algorithm to be used in learning (default=cover)     \n\
     cover                 newly developed, faster and usually more accurate  \n\
     rdr                   original that strictly folows rdr metodology       \n\
                                                                              \n\
  -o, --out <outfile>    output file (human readable rdr tree)                \n\
  <infile>               input file (multext, one example per line)           \n\
                           format of each line: 'word[tab]lemma[tab]*'        \n\
  <infile2>              more input files = equal to the one containg all     \n\
                         the data                                             \n\
                                                                              \n\
      --stat             print statistics about generated tree                \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterLearn::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;
	
	//parameters definition ------------------------------------------------------
	clParser.AddOption("format", optSwitch, optOptional, "-f|--format");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "1|2|3|4|5|6", "4");
	clParser.AddOption("algorithm", optSwitch, optOptional, "-a|--algorithm");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "cover|rdr", "cover");
	
	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, -1, "", "");
	clParser.AddOption("outfile", optSwitch, optMandatory, "-o|--out");
	clParser.LastAdded().SetArgType(argOutFile, 1, 1, "", "");

	clParser.AddOption("stat", optSwitch, optOptional, "--stat");
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
	vector<string> vsInfile = clParser.GetOption("infile").vsArgVal;
	string sOutfile = clParser.GetOption("outfile").vsArgVal[0];
	int iFormat = clParser.GetOption("format").GetAllowedIndex();
	int iAlg = clParser.GetOption("algorithm").GetAllowedIndex();
	bool bVerbose = clParser.GetOption("verbose").bSet;
	bool bStat = clParser.GetOption("stat").bSet;

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "      input file(s): " << vsInfile[0] << endl;
	for (int i=1; i<vsInfile.size(); ++i)
		cout << "                     " << vsInfile[i] << endl;
	cout << "        output file: " << sOutfile << endl; 
	cout << "             format: " << iFormat+1 << endl; 
	cout << "          algorithm: ";
		if (iAlg == 0) cout << "new covering";
		if (iAlg == 1) cout << "original rdr";
	cout << endl;
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	cout << "         statistics: " << (string)(bStat ? "yes" : "no") << endl;
	cout << "processing..."  << endl;

	//OK, finnaly do the magic ---------------------------------------------------
	if (Learn(vsInfile, sOutfile, iFormat, iAlg,  bVerbose, bStat) != 0) return -1;

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}


int InterLearn::Learn(vector<string> vsInfile, string sOutfile, int iFormat, int iAlg,  bool bVerbose, bool bStat) {
	Timing timeAll;
	WordList::iterator *itBegin, *itEnd;

	//importing files
	WordList wls(vsInfile[0].c_str(), NULL,bVerbose);
	for (int i=1; i<vsInfile.size(); ++i) 
		wls.Import(vsInfile[i].c_str(),NULL,bVerbose);

	//new algorithm --------------------------
	if (iAlg == 0) {
		//sorting
		wls.Sort(bVerbose);
		Timing time;

		//learn
		if (bVerbose) cout << "  Starting learn process..." << endl;
		CoverNodeAlg *alg = CoverNodeAlg::BuildTree(wls);
		if (bVerbose) time.Print(cout, "  Learning completed in ");
		if (bStat) alg->PrintStat(cout, 0);

		//generate file
		if (bVerbose) cout << "  Generating output file..." << endl;
		ofstream ofs(sOutfile.c_str(), ios_base::out);
		RdrTree *rt = alg->GenRdrTree();
		rt->Print((PrintStyle)iFormat,ofs);  //alg->PrintTree(ofs, wls, 0);
		ofs.close();
		if (bStat) time.Print(cout, "  File generated successfully in ");

		//test accurancy
		if (bStat) {
			alg->TestAccurancy(*(itBegin = wls.begin()),*(itEnd = wls.end()),1);
			delete itBegin;	delete itEnd;
		}
	} 

	//original algorithm -------------------
	if (iAlg == 1) {
		Timing time;

		//learn
		if (bVerbose) cout << "  Starting learn process..." << endl;
		BaseAlg *alg = new BaseAlg();
		alg->train(*(itBegin = wls.begin()),*(itEnd = wls.end()));
		delete itBegin;	delete itEnd;
		if (bVerbose) time.Print(cout, "  Learning completed in ");
		if (bStat) alg->PrintStat(cout, 0);

		//generate file
		if (bVerbose) cout << "  Generating output file..." << endl;
		ofstream ofs(sOutfile.c_str(), ios_base::out);

		RdrTree *rt = alg->GenRdrTree();
		rt->Print((PrintStyle)iFormat,ofs);  //alg->PrintTree(&ofs);

		ofs.close();
		if (bStat) time.Print(cout, "  File generated successfully in ");

		//test accurancy
		if (bStat) {
			alg->lemmatize(*(itBegin = wls.begin()),*(itEnd = wls.end()),1);
			delete itBegin;	delete itEnd;
		}
	} 

	if (bVerbose) timeAll.Print(cout, "  Time needed altogether ");
	return 0;
}
