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

#include "../header/Statistics.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Calculates some basic statistics about input example data file                \n\
Usage: lemStat [-o <outfile>] [-t <textfile>] <infile> [<infile2>,...]        \n\
                                                                              \n\
  -t, --text <textfile>  some additional statistics that can be derived       \n\
                         from combination with text file (words matching %)   \n\
                                                                              \n\
  -o, --out <outfile>    output file, if not specified statistics are         \n\
                         writen to screen                                     \n\
  <infile>               input file (multext, one example per line)           \n\
                           format of each line: 'word[tab]lemma[tab]*'        \n\
  <infile2>              more input files, each is treated separatelly        \n\
                                                                              \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterStat::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;

	//parameters definition ------------------------------------------------------
	clParser.AddOption("textfile", optSwitch, optOptional, "-t|--text");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");

	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, -1, "", "");
	clParser.AddOption("outfile", optSwitch, optOptional, "-o|--out");
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
	string sTextfile = clParser.GetOption("textfile").vsArgVal[0];
	vector<string> vsInfile = clParser.GetOption("infile").vsArgVal;
	string sOutfile = clParser.GetOption("outfile").vsArgVal[0];
	bool bVerbose = clParser.GetOption("verbose").bSet;

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "      input file(s): " << vsInfile[0] << endl;
	for (int i=1; i<vsInfile.size(); ++i)
		cout << "                     " << vsInfile[i] << endl;
	cout << "        output file: " << (sOutfile==""?"none -> screen":sOutfile) << endl; 
	cout << "          text file: " << (sTextfile==""?"none -> additional statistics won't be done":sTextfile) << endl; 
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	cout << "processing..."  << endl;


	//OK, finnaly do the magic ---------------------------------------------------
	if (StatAll(vsInfile, sOutfile, sTextfile, bVerbose) != 0) return -1;

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}

int InterStat::StatAll(vector<string> vsInfile, string sOutfile, string sTextfile, bool bVerbose) {

	return Statistics::StatAll(vsInfile, sOutfile, sTextfile, bVerbose);
}

