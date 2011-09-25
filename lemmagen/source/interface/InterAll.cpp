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

#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Wrapper of complete lemmagen library funcionalty.                             \n\
Usage: lemmagen <subproc> [<subproc switches>]                                \n\
                                                                              \n\
  <subproc>              specify subporcess to run, possible choiches:        \n\
     lemLearn              Learns RDR tree from examples in multext format    \n\
     lemBuild              Builds lemmatizer data from the rdr tree def. file \n\
     lemmatize             Lemamtizes text and produces new lemmatized file   \n\
     lemXval               Validates accurancy of learning algorithm          \n\
     lemTest               Calculates lemmatization accurancy from two files  \n\
     lemStat               Calculates statistics about input example data file\n\
     lemSplit              Splits example file into subsets for validation    \n\
                                                                              \n\
  <subproc switches>     try help of the subprocesses for switches definition \n\
                                                                              \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterAll::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;

	//parameters definition ------------------------------------------------------
	clParser.AddOption("subproc", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "lemLearn|lemBuild|lemmatize|lemXval|lemTest|lemStat|lemSplit", "");

	clParser.AddOption("help", optSwitch, optOptional, "-h|--help");
	clParser.AddOption("version", optSwitch, optOptional, "--version");

	//parsing --------------------------------------------------------------------
	string sError = clParser.Parse(min(argc,2), argv);

	//errors & messages ----------------------------------------------------------
	if (clParser.GetOption("help").bSet) { cout << MSG_USAGE; return 0; }      //print usage
	if (clParser.GetOption("version").bSet) { cout << MSG_VERSION; return 0; } //print version
	if (sError != "") { cout << sError << "!\n" << MSG_HELP; return -1; }      //print error

	//ok arguments ok, read required options -------------------------------------
	string sSubprocces = clParser.GetOption("subproc").vsArgVal[0];

	//output setting -------------------------------------------------------------
    //cout << "     submodule: " << sSubprocces << endl;

	//OK, finnaly do the magic ---------------------------------------------------
	const int iCount = 7;
	Interface *(ifaceAll[iCount]);
	ifaceAll[0] = new InterLearn();
	ifaceAll[1] = new InterBuild();
	ifaceAll[2] = new InterLemtz();
	ifaceAll[3] = new InterXval();
	ifaceAll[4] = new InterTest();
	ifaceAll[5] = new InterStat();
	ifaceAll[6] = new InterSplit();

	for (int i = 0; i< iCount; i++)
		if (ifaceAll[i]->sInterface == sSubprocces) {
			ifaceAll[i]->EntryPoint(argc-1, &(argv[1]));
			return 0;
		}

	return 1;
}


