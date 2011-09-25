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

#include "MiscLib.h"



enum OptType {
	optDefault,
	optSwitch
};

enum OptStyle {
	optMandatory,
	optOptional
};

enum ArgType {
    argDefined,
	argInteger,
	argInFile,
	argOutFile,
	argOutFileApp,
	argAny
};

class Option{
public:
	string sName;
	OptType type;
	OptStyle style;
	vector<string> vsSwitch;
	
	ArgType argType;
	int iMinArgCount; 
	int iMaxArgCount;
	vector<string> vsAllowedArg;

	bool bSet;
	vector<string> vsArgVal;
	string sDefaultArg;

public:
	Option(string sName, OptType type, OptStyle style, string sSwitch);
	void SetArgType(ArgType argType, int iMinArgCount, int iMaxArgCount, string sAllowedArg, string sDefaultArg);
	
	string SetArg(string sValue);
	bool ArgFull();
	bool HasSwitch(string sSwitch);

	int GetAllowedIndex(string sValue = "");
	int GetInteger(string sValue = "");
};

class CmdLineParser{
private:
	vector<Option> voOption;
	string sError;

public:
	CmdLineParser();

	string Parse(int argc, char **argv);

	Option &AddOption(string sName, OptType type, OptStyle style, string sSwitch);
	Option &LastAdded();
	Option &GetOption(string sName);

	Option *FindSwitchOption(string sSwitch);
	Option *FindDefaultOption();
};

