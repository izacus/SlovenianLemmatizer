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

#include "../header/RdrScanner.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Builds lemmatizer structure from the rdr tree definition file                 \n\
Usage: lemBuild [-t <type> [-s ...]] [-l <language>] [-o <outfile>] <infile>  \n\
                                                                              \n\
  -t, --type <type>      specify output type (default = bin), valid options:  \n\
     bin                   binary file that can be imported into 'lemmatize'  \n\
     head                  header file for libraries that are using lemmagen  \n\
     src                   class file that can be compiled to 'lemmatize',    \n\
                           must specify directory where skeleton files reside \n\
     human                 human readable lemmatizer structure                \n\
     txt1, txt2, ... txt6  output is not lemmatizer but just an optimized tree\n\
                           the number besides 'txt' specifies format of tree  \n\
  -s, --skelet <skeldir> directory where program can find skeleton files      \n\
  -l, --lang <language>  specify language for output file naming and          \n\
                           lemmatizer definition (default is 'unknown-lang')  \n\
                                                                              \n\
  -o, --out <outfile>    output file (default is <language>.<type>)           \n\
  <infile>               input file (rdr tree definition file)                \n\
                                                                              \n\
      --stat             print statistics about parsed tree                   \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterBuild::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;

	//parameters definition ------------------------------------------------------
	clParser.AddOption("type", optSwitch, optOptional, "-t|--type");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "bin|head|src|human|txt1|txt2|txt3|txt4|txt5|txt6", "bin");
	clParser.AddOption("skeleton", optSwitch, optOptional, "-s|--skelet");
	clParser.LastAdded().SetArgType(argAny, 1, -1, "", "");
	clParser.AddOption("language", optSwitch, optOptional, "-l|--lang");
	clParser.LastAdded().SetArgType(argAny, 1, 1, "", "unknown-lang");


	clParser.AddOption("outfile", optSwitch, optOptional, "-o|--out");
	clParser.LastAdded().SetArgType(argOutFile, 1, 1, "", "");
	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	
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
	string sInfile = clParser.GetOption("infile").vsArgVal[0];
	int iType = clParser.GetOption("type").GetAllowedIndex();
	string sLanguage = clParser.GetOption("language").vsArgVal[0];
	vector<string> vsSkelDir = clParser.GetOption("skeleton").vsArgVal;

	string sOutfile;
	if (clParser.GetOption("outfile").bSet)
		sOutfile = clParser.GetOption("outfile").vsArgVal[0];
	else {
		sOutfile = sLanguage;
		if (iType == 0) sOutfile += ".bin";
		if (iType == 1) sOutfile += ".h";
		if (iType == 2) sOutfile += ".cpp";
		if (iType >= 3) sOutfile += ".txt";
	}

	bool bVerbose = clParser.GetOption("verbose").bSet;
	bool bStat = clParser.GetOption("stat").bSet;

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "         input file: " << sInfile << endl;
	cout << "        output file: " << sOutfile << endl;
	cout << " language specifier: " << sLanguage << endl;
	cout << "        output type: ";
		if (iType == 0) cout << "binary";
		if (iType == 1) cout << "header";
		if (iType == 2) cout << "source";
		if (iType == 3) cout << "human readable";
	cout << endl;
	if (iType == 2) {
		cout << " skeleton folder(s): " << vsSkelDir[0] << endl;
		for (int i=1; i<vsSkelDir.size(); ++i) {
			cout << "                     " << vsSkelDir[i] << endl;
		}
	}
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	cout << "         statistics: " << (string)(bStat ? "yes" : "no") << endl;
	cout << "processing..."  << endl;

	//OK, finnaly do the magic ---------------------------------------------------
	if (Parse(sInfile, sOutfile, sLanguage, iType, vsSkelDir, bVerbose, bStat) != 0) {
		cout << "completed with errors"  << endl;
		return -1;
	}

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}

int InterBuild::Parse(string sInfile, string sOutfile, string sLanguage, int iType, vector<string> vsSkelDir, bool bVerbose, bool bStat) {

	Timing timeAll;

	const int iSkelNum = 6;
	string asSkeletonFiles[iSkelNum] = {
		"MiscLib.h",
		"MiscLib.cpp",
		"CmdLineParser.h",
		"CmdLineParser.cpp",
		"RdrLemmatizer.h",
		"RdrLemmatizer.cpp"
	};
	string asFiles[iSkelNum];

	//check for skeleton files before start
	if (iType == 2) {
		bool bSuccess = true;
		ifstream ifs;

		for (int iFile = 0; iFile < iSkelNum; ++iFile) {
			asFiles[iFile] = "";
			for (int i=0; i<vsSkelDir.size(); ++i) {

				string sFileName = vsSkelDir[i] + asSkeletonFiles[iFile];
				ifs.open(sFileName.c_str(), ifstream::in);		
				ifs.close();

				if(!ifs.fail()) {
					asFiles[iFile] = sFileName;
					break;
				}
			}

			if (asFiles[iFile] == "") {
				ifs.clear(ios::failbit);
				cout << "  Can't find skeleton file: '" + asSkeletonFiles[iFile] + "'" << endl;
				bSuccess = false;
			} else if (bVerbose) 
				cout << "  Skeleton file funund in: '" + asFiles[iFile] + "'" << endl;
		}

		if (!bSuccess) return -1;
	}

	Timing time;
	RdrTree *tree = NULL;
	RdrScanner *scanner = new RdrScanner();

	if (bVerbose) cout << "  Starting to parse input file..." << endl;
	tree = scanner->scan(sInfile.c_str(), &cout, 0);

	if(scanner->bSuccess){
		if (bVerbose) time.Print(cout, "  Parsing successfull in ");
		if (bStat) tree->PrintStat(cout, 0);

		if (bVerbose) cout << "  Starting optimization & lemmatizer generation..." << endl;
		RdrLemmatizer *lemtz = tree->CreateLemmatizer();
		if (bVerbose) time.Print(cout, "  Optimization & RdrLemmatizer creation complete in ");
		if (bStat) cout << "  Lemmatizer size : " << lemtz->SizeOfTree() << "B" << endl;
		if (bStat) tree->PrintStat(cout, 0);

		if (bVerbose) cout << "  Generating output file..." << endl;
		if (iType == 0)
			lemtz->SaveBinary(sOutfile.c_str());
		if (iType != 0) {
			ofstream ofsOutFile(sOutfile.c_str(), ios_base::out);
			
			if (iType == 1)
				lemtz->ToStringHex(ofsOutFile);
			if (iType == 2) {
				stringstream ssHex;
				lemtz->ToStringHex(ssHex);
			}
			if (iType == 3)
				lemtz->ToString(ofsOutFile);
			if (iType > 3) 
				RdrTree::GenFromLemmatizer(*lemtz)->Print((PrintStyle)(iType-4),ofsOutFile);

			ofsOutFile.close();
		}
		if (bVerbose) time.Print(cout, "  File generated successfully in ");

		delete lemtz;
		if (bVerbose) timeAll.Print(cout, "  Time needed altogether ");
		return 0;
	}
	else
		return -1;




}


