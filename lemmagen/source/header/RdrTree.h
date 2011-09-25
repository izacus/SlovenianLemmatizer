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

#include "RdrRule.h"

//format specifier for print style
//[0]: Rule opening specifier
//[1]: Id opening specifier
//[2]: Id closing specifier
//[3]: Suffix opening specifier
//[4]: Suffix closing specifier
//[5]: Transformation rule opening specifier
//[6]: Transformation rule arrow specifier
//[7]: Transformation rule closing specifier
//[8]: Exceptions count opening specifier
//[9]: Exceptions count closing specifier
//[10]: Rule closing specifier
//[11]: default tabulator, (empty horizontal line)
//[12]: arrow, (full horizontal line)
//[13]: 3 byte flag for format =	#0(Y,N) draw lines 
//									#1(Y,N) empty first line of subgroup
//									#2(Y,N) empty last line of subgroup
//		1 not used =				#3		not used
//		4 char line midifier =		#4      vertical line 
//									#5      vertical line crosing all but last
//									#6      vertical line crosing with last
//									#7      vertical line after last (empty)
//[14]: suffix beeing equal to whole word flag
static const char *tblFormat[6][15] = { 
		{"Rule:(",	" Id(\"",	/*ID*/"\")",	" Suffix(\"",	/*END*/"\")",	" Transform(\"",	/*TRAN*/"\"-->\"",	"\")",	" Exceptions(",	/*EXC*/")",	" );",	"     ",	"",			"NNY     "," EntireWord"},
		{"rule:",	" n'",		/*ID*/"\"",		" i'",			/*END*/"'",		" t'",				/*TRAN*/"'->'",		"'",	"",				/*EXC*/"",	";",	"\t",		"",			"NNN     "," ew"},
		{"Rule:",	" id '",	/*ID*/"\"",		" end '",		/*END*/"'",		" trans '",			/*TRAN*/"'->'",		"'",	"",				/*EXC*/"",	";",	"    ",		"-- ",		"YNN ||` "," ew"},
		{"RULE:(",	" id(\"",	/*ID*/"\")",	" suffix(\"",	/*END*/"\")",	" transform(\"",	/*TRAN*/"\"-->\"",	"\")",	" except(",		/*EXC*/")",	" );",	"      ",	"---> ",	"YYY ||` "," entireword"},
		{"RULE:(",	" id(\"",	/*ID*/"\")",	" suffix(\"",	/*END*/"\")",	" transform(\"",	/*TRAN*/"\"-->\"",	"\")",	" except(",		/*EXC*/")",	" );",	"      ",	"---> ",	"YNY ||` "," entireword"},
		{"RULE:",	" name(\"",	/*ID*/"\")",	" if(\"",		/*END*/"\")",	" then(\"",			/*TRAN*/"\"-->\"",	"\")",	" exc(",		/*EXC*/")",	";",	"      ",	"--->:",	"YYY ::: "," ew"}
	};

enum PrintStyle {
	STAND = 0,
	MINI = 1,
	NICE_MIN = 2,
	NICE = 3,
	NICE_SHORT = 4,
	NICE_IF = 5
};

class RdrTree {

public:
	typedef deque<RdrTree *> deqrt;
	typedef map<int, int> mapii;
	typedef map<byte*,RdrRule*,Str::LowerByteStr> mapcr;

	byte *acId;
	byte *acEnding;
	int iEndingLen;
	int iNewSufxLen;

	//ending is actualy whole word
	bool bEntireWord;

	RdrRule *rrRule;		//this rule
	deqrt *deqSubNode;		//subnodes

	//data for usage when creating lemmatizer binary representation
	int iOrder;				//sequence number of this node trough whole tree
	int iAddress;			

public:
	RdrTree();
	~RdrTree();

public:
	void SetEnding(byte *acEnding);
	void SetNewSufxLen(int iParentEnding);

	void TreeStatistics(mapii &mapDepthIntrDist, mapii &mapDepthLeafDist, mapii &mapWidthDist, int iDepth=0) const;
	void PrintStat(ostream &os, int iType=0) const;

	char *NodeToString(PrintStyle ps = NICE) const;
	void Print(PrintStyle ps = NICE, ostream &os = cout, int iMaxLevel = -1,
		const char *acTab="@#$", const char *acTabCumul="",
		int iDepth = 0, bool bLast = true) const;
	
	char *Lemmatize(const char *acWord) const;

	void Sort();
	void CompactRules(mapcr *mapRules = NULL);
	void PatchTree();

	RdrLemmatizer *CreateLemmatizer();

	static RdrTree *GenFromLemmatizer(const RdrLemmatizer &lem, 
		dword iNodeAddr = DataStart, char *acParSufx = "", char cNewChar=NULL);

private:
	RdrTree *FindSubNode(const char *acWord) const;

	void EnumerateTree(int &iNum);
	void RemoveMultiCover();
	void OneCharDecision();

	int GetMinHash();
	int GetByteLength();
	void SetAddress(int &iAddr, bool bReset=false);

	void SetByteCodeThis(byte *abData);
	void SetByteCode(byte *abData);

private:

	//struct for usage in sort algorithms
	struct LowerTree {
		//returns true if s1<s2
		bool operator()(const RdrTree *x, const RdrTree *y) const {
			return (Str::bytecmpBack(x->acEnding, y->acEnding) < 0);
		}
	};
};





