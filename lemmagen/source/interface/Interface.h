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

#include "../header/CmdLineParser.h"

#define MSG_HELP "\
Try '-h' or `--help' for more information.                                  \n\
"
#define MSG_VERSION "\
program version 2.0, part of lemmagen library                                 \n\
  Author: Matjaz Jursic <matjaz@@gmail.com>                                   \n\
  Report bugs!                                                                \n\
"

class Interface {
public:
	string sInterface;
	virtual int EntryPoint(int argc, char **argv) = 0;
};

class InterBuild: public Interface {
public:
	InterBuild() {sInterface = "lemBuild";}
	int EntryPoint(int argc, char **argv);
	int Parse(string sInfile, string sOutfile, string sLanguage, int iType, vector<string> vsSkelDir, bool bVerbose, bool bStat);
};

class InterLearn: public Interface {
public:
	InterLearn() {sInterface = "lemLearn";}
	int EntryPoint(int argc, char **argv);
	int Learn(vector<string> vsInfile, string sOutfile, int iFormat, int iAlg,  bool bVerbose, bool bStat);
};
class InterLemtz: public Interface {
public:
	InterLemtz() {sInterface = "lemmatize";}
	int EntryPoint(int argc, char **argv);
	int Lemmatize(string sInfile, string sOutfile, string sLangFile, int iFormat, bool bVerbose, char cDelimit, int iMode, int iLangMode, bool bStat);
};
class InterSplit: public Interface {
public:
	InterSplit() {sInterface = "lemSplit";}
	int EntryPoint(int argc, char **argv);
	int Split(string sInfile, string sOutfileMask, int iSplit, int iType, int iRandSeed, bool bVerbose);
};
class InterTest: public Interface {
public:
	InterTest() {sInterface = "lemTest";}
	int EntryPoint(int argc, char **argv);
	int Compare(string sCorfile, string sLemfile, bool bVerbose);
};
class InterXval: public Interface {
public:
	InterXval() {sInterface = "lemXval";}
	int EntryPoint(int argc, char **argv);
	int Xval(string sInfile, string sOutfile, int iSplit, int iType, int iAlg, 
					int iRandSeed, bool bVerbose, bool bSilent, int iNumRuns);
};
class InterStat: public Interface {
public:
	InterStat() {sInterface = "lemStat";}
	int EntryPoint(int argc, char **argv);
	int StatAll(vector<string> vsInfile, string sOutfile, string sTextfile, bool bVerbose);
};
class InterAll: public Interface {
public:
	InterAll() {sInterface = "lemmagen";}
	int EntryPoint(int argc, char **argv);
};
