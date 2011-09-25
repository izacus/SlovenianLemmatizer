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

#include "../header/RdrTree.h"
#include "../header/RdrScanner.h"
#include "Interface.h"

#define MSG_USAGE "\
                                                                              \n\
Lemamtizes text and produces new lemmatized file.                             \n\
Usage: lemmatize [-f <format>] [-l <datafile>] [-lm <lm>] <infile> <outfile>  \n\
                                                                              \n\
  -m, --mode <mode>      specify operation mode (default = lemmatize)         \n\
     l | lemmatize         standard operation - lematization of input files   \n\
     o | output            output mode - outputs current lemmatization rules  \n\
                                                                              \n\
  -f, --format <format>  specify input/output file format (default = text)    \n\
    mode = lemmatize:                                                         \n\
     text                  outfile = infile just words are lematized          \n\
     wpl                   word per line, outfile line = word + tab + lemma   \n\
    mode = output:                                                            \n\
     txt1, txt2, ... txt6  one of posible tree formatings                     \n\
                                                                              \n\
  -l, --lang <datafile>  language data file that defines lemmatization        \n\
                         rules, if not specified it lematizes by default rules\n\
                         that were build by compilation. If you did't make    \n\
                         your own compilation then default rules are empty.   \n\
  -lm, --langmode <lm>   specify language data file mode (default = binary)   \n\
     b | binary            binary data file (optimized lemmatization)         \n\
     t | text              textual data file (NOT optimized lemmatization!)   \n\
     o | optimtext         textual data file (optimized lemmatization)        \n\
                                                                              \n\
  -d, --delimit <delmt>  removes punctuations from <outfile>, one or more     \n\
                         punct. together are replaced with single <delmt>     \n\
                                                                              \n\
  <infile>               input file (file to be lemmatized)                   \n\
  <outfile>              output file (lemmatized file)                        \n\
                                                                              \n\
      --stat             print statistics about pareed tree                   \n\
  -v, --verbose          verbose messages                                     \n\
  -h, --help             print this help and exit                             \n\
      --version          print version information and exit                   \n\
                                                                              \n\
"

int InterLemtz::EntryPoint(int argc, char **argv){
	CmdLineParser clParser;
	
	//parameters definition ------------------------------------------------------
	clParser.AddOption("mode", optSwitch, optOptional, "-m|--mode");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "l|o|lemmatize|output", "lemmatize");

	clParser.AddOption("format", optSwitch, optOptional, "-f|--format");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "text|wpl|txt1|txt2|txt3|txt4|txt5|txt6", "text");

	clParser.AddOption("langfile", optSwitch, optOptional, "-l|--lang");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	clParser.AddOption("langmode", optSwitch, optOptional, "-lm|--langmode");
	clParser.LastAdded().SetArgType(argDefined, 1, 1, "b|t|o|binary|text|optimtext", "binary");

	clParser.AddOption("delimit", optSwitch, optOptional, "-d|--delimit");
	clParser.LastAdded().SetArgType(argAny, 1, 1, "", "");
	
	clParser.AddOption("infile", optDefault, optMandatory, "");
	clParser.LastAdded().SetArgType(argInFile, 1, 1, "", "");
	clParser.AddOption("outfile", optDefault, optMandatory, "");
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
	string sInfile = clParser.GetOption("infile").vsArgVal[0];
	string sOutfile = clParser.GetOption("outfile").vsArgVal[0];
	string sLangFile = clParser.GetOption("langfile").vsArgVal[0];
    int iLangMode = clParser.GetOption("langmode").GetAllowedIndex()%3;
	char cDelimit = clParser.GetOption("delimit").vsArgVal[0][0];
	int iMode = clParser.GetOption("mode").GetAllowedIndex()%2;
	int iFormat = clParser.GetOption("format").GetAllowedIndex();
	bool bVerbose = clParser.GetOption("verbose").bSet;
	bool bStat = clParser.GetOption("stat").bSet;

	if ((iMode == 0 && iFormat>1) || (iMode == 1 && iFormat<2)) {
		sError = "Format specifier wrong for this operation mode";
		cout << sError << "!\n" << MSG_HELP; 
		return -1;
	}

	if (iLangMode>0 && sLangFile=="") {
		sError = "When using --langmode switch 't' or 'o' also -l <datafile> must be specified";
		cout << sError << "!\n" << MSG_HELP; 
		return -1;
	}

	//output setting -------------------------------------------------------------
	cout << "chosen options:"  << endl;
	cout << "     operation mode: ";
		if (iMode == 0) cout << "lemmatization";
		if (iMode == 1) cout << "rules for lemmatization output";
	cout << endl;
	cout << " language data file: " << (string)(sLangFile!="" ? sLangFile : "none") << endl;
	cout << "     data file mode: ";
		if (iLangMode == 0) cout << "binary";
		if (iLangMode == 1) cout << "text (lemmatization NOT OPTIMIZED)";
		if (iLangMode == 2) cout << "text with lemmatizer optimization";
	cout << endl;
	cout << "         input file: " << sInfile << endl;
	cout << "        output file: " << sOutfile << endl;
	cout << "             format: ";
		if (iFormat == 0) cout << "plain text";
		if (iFormat == 1) cout << "one word per line";
		if (iFormat >  1) cout << "txt" << iFormat-1;
	cout << endl;
	cout << "            verbose: " << (string)(bVerbose ? "yes" : "no") << endl;
	stringstream ssDelimit; ssDelimit<<cDelimit; string sDelimit = ssDelimit.str();
	cout << "   output delimiter: " << (string)(cDelimit==NULL ? "not used" : "'" + sDelimit + "'") << endl;
	cout << "processing..."  << endl;


	//OK, finnaly do the magic ---------------------------------------------------
	if (Lemmatize(sInfile, sOutfile, sLangFile, iFormat, bVerbose, cDelimit, iMode, iLangMode ,bStat) != 0) return -1;

	//report success -------------------------------------------------------------
	cout << "finished successfully"  << endl;
	return 0;
}

//actual lematization function
int InterLemtz::Lemmatize(string sInfile, string sOutfile, string sLangFile, int iFormat, bool bVerbose, char cDelimit, int iMode, int iLangMode, bool bStat) {
	Timing timeLoadBinary;

	RdrLemmatizer *lemtz = NULL;
	RdrTree *tree = NULL;

	//create lemmatizer
	if (iLangMode == 0) {
		lemtz = new RdrLemmatizer();
		if (sLangFile != "") lemtz->LoadBinary(sLangFile.c_str());
		if (bVerbose && sLangFile != "") timeLoadBinary.Print(cout, "  Binary lemmatizer loaded in ");
	} else if (iLangMode > 0) {
		RdrScanner *scanner = new RdrScanner();

		if (bVerbose) cout << "  Starting to parse input file..." << endl;
		tree = scanner->scan(sLangFile.c_str(), &cout, 0);

		if(scanner->bSuccess){
			delete scanner;

			if (bVerbose) timeLoadBinary.Print(cout, "  Texual lemmatizer parsed & loaded in ");
			timeLoadBinary.Reset();

			if (iLangMode == 1) {
				cout << "  WARNING: Lemmatization with textal lemmatizers not optimized,"  << endl;
				cout << "           lemBuild should be used to convert it into binary format!"  << endl;
			} else {
				if (bVerbose) cout << "  Optimizing texual lemmatizer into binary form..." << endl;
				lemtz = tree->CreateLemmatizer();
				if (bVerbose) timeLoadBinary.Print(cout, "  Lemmatizer optimized in ");
				delete tree;
			}
		} else {
			cout << "  Error encourtered while parsing textual rdr rules. Program terminated!"  << endl;
			return -1;
		}
	}
	if (tree==NULL && lemtz==NULL) lemtz = new RdrLemmatizer(); 

	//lemmatize
	if (iMode == 0) {
		int         iWordNum = 0;
		int         iLine = 0;
		const int	MAX_LEN = 10000;
		char *acBuf = new char[MAX_LEN*2];
		char *acOut = new char[MAX_LEN*2];

		ifstream	ifs(sInfile.c_str());
		ofstream	ofs(sOutfile.c_str());

		Timing time;
		
		// read lines from file
		while (!ifs.eof()) {
			iLine++;
			ifs.getline(acBuf, MAX_LEN, '\n');

			//lemmatize one line
			char *acNewEnd = acOut;
			char *acStart = acBuf;
			
			for(int i=0; i==0 || (acBuf[i-1]!='\n' && acBuf[i-1]!=NULL); ++i) {
				if (((iFormat == 0) && WORDCHARS) ||
					((iFormat == 1) && NONSPACECHARS)) continue;

				if (acStart < &(acBuf[i])) {
					iWordNum++;
					char ch = acBuf[i];
					acBuf[i] = 0;

					//depends on a type of file witch option
					if (iFormat == 0) {
						//lemmatize word by word
						if (lemtz == NULL) {
                            char *acLemma = tree->Lemmatize(acStart);
							int iLenLemma = strlen(acLemma);
							memcpy(acNewEnd, acLemma, iLenLemma);
							acNewEnd[iLenLemma] = NULL;
							delete[] acLemma;
						}
						else
							lemtz->Lemmatize(acStart, acNewEnd);

						acNewEnd = acNewEnd + strlen(acNewEnd);
					} else {
						//lemmatize just first word and exit line
						strcpy(acNewEnd, acStart);
						acNewEnd = acNewEnd + strlen(acNewEnd);
						acNewEnd[0] = (cDelimit == 0 ? '\t' : cDelimit);
						acNewEnd++;

						if (lemtz == NULL) {
                            char *acLemma = tree->Lemmatize(acStart);
							int iLenLemma = strlen(acLemma);
							memcpy(acNewEnd, acLemma, iLenLemma);
							acNewEnd[iLenLemma] = NULL;
							delete[] acLemma;
						}
						else
							lemtz->Lemmatize(acStart, acNewEnd);

						acNewEnd = acNewEnd + strlen(acNewEnd);
						acNewEnd[0] = (cDelimit == 0 ? '\t' : cDelimit);
						acNewEnd++;

						acBuf[i] = ch;
						strcpy(acNewEnd, &(acBuf[i]));
						acNewEnd = acNewEnd + strlen(acNewEnd);
						acNewEnd++;

						break;
					}
					acBuf[i] = ch;
				}

				//delimit right
				if (cDelimit == 0 || acBuf[i] == '\n' || acBuf[i] == '\r' || acBuf[i] == NULL) {
					acNewEnd[0]=acBuf[i];
					acNewEnd++;
				} else {
					acNewEnd[0]=cDelimit;
					if (acNewEnd>acOut && acNewEnd[-1]!=cDelimit) acNewEnd++;
				}
				acStart = &(acBuf[i+1]);
			}
			
			acNewEnd[-1] = '\n';		
			ofs.write(acOut, acNewEnd-acOut);

		}
		ifs.close();
		ofs.close();
		delete[] acBuf;
		delete[] acOut;

		double dTimeNs = time.ElapSec(false) * 1000000000 / iWordNum;

		if (bStat) {
			cout << "  Lemmatized " 
				<< iLine << " lines, " 
				<< iWordNum << " words, " 
				<< setprecision(0) << fixed << dTimeNs << "ns per word."
				<< endl;
			
			cout << "  Time needed altogether " << setprecision(4) << fixed << time.ElapSec(false) << "s." << endl;
		}
	}


	if (iMode == 1) {

		Timing time;
		RdrTree *rt = NULL;

		if (lemtz == NULL) rt = tree;
		else rt = RdrTree::GenFromLemmatizer(*lemtz);

		if (bVerbose) time.Print(cout, "  Dissembling of lemmatization rules from binary code completed in ");

		//output statistics
		if (bStat) rt->PrintStat(cout, 0);
		
		//generate file
		if (bVerbose) cout << "  Generating output file..." << endl;
		ofstream ofs(sOutfile.c_str(), ios_base::out);

		time.Reset();
		rt->Print((PrintStyle)(iFormat-2),ofs);  //alg->PrintTree(&ofs);

		ofs.close();
		if (bVerbose) time.Print(cout, "  File generated successfully in ");
	}

	return 0;
}


