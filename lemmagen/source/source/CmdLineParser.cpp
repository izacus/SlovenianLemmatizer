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
#include "../header/CmdLineParser.h"

//-------------------------------------------------------------------------------------------
Option::Option(string sName, OptType type, OptStyle style, string sSwitch) {
	this->sName = sName;
	this->type = type;
	this->style = style;
	TrueStr::Tokenize(sSwitch, this->vsSwitch);
	
	SetArgType(argAny, 0, 0, "", "");
	
	bSet = false;
}

//-------------------------------------------------------------------------------------------
void Option::SetArgType(ArgType argType, int iMinArgCount, int iMaxArgCount, string sAllowedArg, string sDefaultArg) {
	this->argType = argType;
	this->iMinArgCount = iMinArgCount;
	this->iMaxArgCount = iMaxArgCount;
	TrueStr::Tokenize(sAllowedArg, this->vsAllowedArg);

	this->sDefaultArg = sDefaultArg;
	vsArgVal.clear();
	vsArgVal.push_back(sDefaultArg);
}

//-------------------------------------------------------------------------------------------
string Option::SetArg(string sValue){
	if (!bSet) {
		bSet = true;
		vsArgVal.clear();
	}

	if (sValue == "") return "";

	if (iMaxArgCount != -1 && vsArgVal.size() >= iMaxArgCount) 
		return "Too many arguments for option '" + sName + "'";


	switch (argType) {
		case argDefined :
			if (GetAllowedIndex(sValue)==-1) 
				return "Argument value '" + sValue + "' not supported in option '" + sName + "'";
			break;
		case argInteger : 
			int iTest;
			if(sscanf(sValue.c_str(), "%d", &iTest) == 0)
				return "Wong argument value in option '" + sName + "', '" + sValue + "' is not a number";
			break;		
		case argInFile : 
			if (!CheckFile::CanRead(sValue.c_str()))
				return "File '" + sValue + "' can not be opened for read. Check it's existance and locks";
			break;
		case argOutFile : 
			if (!CheckFile::CanWrite(sValue.c_str()))
				return "File '" + sValue + "' can not be opened for write. Check it's existance and locks";
			break;
		case argOutFileApp : 
			if (!CheckFile::CanWrite(sValue.c_str(), false))
				return "File '" + sValue + "' can not be opened for write. Check it's existance and locks";
			break;
		case argAny : 
			break;
	}

	vsArgVal.push_back(sValue);

	return "";
}

//-------------------------------------------------------------------------------------------
bool Option::ArgFull(){
    if (iMaxArgCount == -1 || vsArgVal.size() < iMaxArgCount || (!bSet && iMaxArgCount>0))
		return false;
	else
		return true;
}

//-------------------------------------------------------------------------------------------
bool Option::HasSwitch(string sSwitch){
	for (vector<string>::iterator it = vsSwitch.begin(); it != vsSwitch.end(); ++it)
		if (*it == sSwitch)
			return true;
	
	return false;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
int Option::GetAllowedIndex(string sValue) {
	if (sValue == "" && vsArgVal.size() > 0) sValue = vsArgVal.back();
		

	int index = 0;
	for (vector<string>::iterator it = vsAllowedArg.begin(); it != vsAllowedArg.end(); ++it, ++index)
		if (*it == sValue)
			return index;

	return -1;
}

int Option::GetInteger(string sValue) {
	if (sValue == "") sValue = vsArgVal.back();

	int iVal = -1;
	sscanf(sValue.c_str(), "%d", &iVal);
	return iVal;
}

//-------------------------------------------------------------------------------------------
//constructor
CmdLineParser::CmdLineParser() {
	sError = "";
}

//-------------------------------------------------------------------------------------------
//parsing
string CmdLineParser::Parse(int argc, char **argv) {
	Option *optNew = NULL;
	Option *optCurrent = NULL;

	//parse
	for (int i=1; i<argc; i++) {
		// state for accepting new options ------------------------------
		if (optCurrent == NULL || optCurrent->ArgFull()) {
			optCurrent = FindSwitchOption(argv[i]);
			//possible error when searching
			if (sError != "") return sError;
			
			//we didnt find no switch option, so it must be default option
			if (optCurrent == NULL) {
				optCurrent = FindDefaultOption();
				//is also not default option error
				if (optCurrent == NULL)
					return string("Can't recognize argument '") + string(argv[i]) + string("'");
				//we recognise as default option
				else {
					string sResult = optCurrent->SetArg(argv[i]); if (sResult != "") return sResult;
				}
			} 

			//we found matching switch option
			else {
				string sResult = optCurrent->SetArg(""); if (sResult != "") return sResult;
			}
		}
		// state for accepting new arguments to existing options ------------------------------
		else {
			optNew = FindSwitchOption(argv[i]); if (sError != "") return sError;
			
			//we didnt find new switch option, so we append to current
			if (optNew == NULL) {
				string sResult = optCurrent->SetArg(argv[i]); if (sResult != "") return sResult;
				optCurrent = NULL; //we add just one switch option at a time
			} 

			//we found switch option so add it
			else {
				optCurrent = optNew;
				string sResult = optCurrent->SetArg(""); if (sResult != "") return sResult;
			}
		}
	}

	//check for required mandatories, maximums and minimums
	for (vector<Option>::iterator it = voOption.begin(); it != voOption.end(); ++it) {
		if (!it->bSet && it->style == optMandatory)
			sError += string(sError!=""?"!\n":"") + "Option '" + it->sName + "' is mandatory";

		if (it->bSet) {
			if (it->iMaxArgCount!=-1 && it->vsArgVal.size() > it->iMaxArgCount) {
				stringstream sCount; sCount << it->iMaxArgCount;
				sError +=  string(sError!=""?"!\n":"") + 
					"Too much arguments for option '" + it->sName + "', it has maximum of " + sCount.str() + " argument(s)";
			}
			if (it->vsArgVal.size() < it->iMinArgCount) {
				stringstream sCount; sCount << it->iMinArgCount;
				sError +=  string(sError!=""?"!\n":"") + 
					"Too few arguments for option '" + it->sName + "', it has minimum of " + sCount.str() + " argument(s)";
			}
		}
	}

	//no errors
	return sError;
}

//-------------------------------------------------------------------------------------------
//adding new options
Option &CmdLineParser::AddOption(string sName, OptType type, OptStyle style, string sSwitch) {
	voOption.push_back(Option(sName, type, style, sSwitch));
	return voOption.back();
}
//-------------------------------------------------------------------------------------------
//last added option
Option &CmdLineParser::LastAdded() {
	return voOption.back();
}
//-------------------------------------------------------------------------------------------
//return option by name
Option &CmdLineParser::GetOption(string sName) {
	for (vector<Option>::iterator it = voOption.begin(); it != voOption.end(); ++it)
		if (it->sName == sName)
			return *it;
	
	return *((Option*) NULL);
}

//-------------------------------------------------------------------------------------------
Option *CmdLineParser::FindSwitchOption(string sSwitch) {
	for (vector<Option>::iterator it = voOption.begin(); it != voOption.end(); ++it)
		if (it->type == optSwitch && it->HasSwitch(sSwitch)) {
			if (it->bSet && it->ArgFull()) {
				if (it->iMaxArgCount == 0) {
					sError =  "Switch option '" + it->sName + "' already set";
					return NULL;
				} else  {
					stringstream sCount; sCount << it->iMaxArgCount;
					sError = "Too much arguments for option '" + it->sName + "', it has maximum of " + sCount.str() + " arguments";
					return NULL;
				}
			}
			return &(*it);
		}
	
	return NULL;
}

//-------------------------------------------------------------------------------------------
Option *CmdLineParser::FindDefaultOption() {
	for (vector<Option>::iterator it = voOption.begin(); it != voOption.end(); ++it)
		if (it->type == optDefault && !it->ArgFull() ) {
			return &(*it);
		}
	
	return NULL;
}

