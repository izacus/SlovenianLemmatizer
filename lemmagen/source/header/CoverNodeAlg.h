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

#include "WordList.h"
#include "RdrTree.h"

class CoverNodeAlg {
	
//code
public:
	//-------------------------------------------------------------------------------------------	
	//static aux 'constructor'
	static CoverNodeAlg *BuildTree(WordList &wl, int iSet = -1, bool bSetElem=false, int iVerbose=0, ostream &os=cout);

	//-------------------------------------------------------------------------------------------	
	//constructor & destructor
	CoverNodeAlg(WordList::iterator &itStart, WordList::iterator &itEnd, int iWordNum,
	     const CoverNodeAlg *nParent=NULL, const 
		 char chNewChar=NULL);
	~CoverNodeAlg();

	//-------------------------------------------------------------------------------------------	
	//static aux functions for building node tree
	static Rule *GetBestRule(WordList::iterator &itStart, WordList::iterator &itEnd,
					  const int &iEndingLen, Rule *&rParentRule, int &iWordsThisPos);
	static int CreateSubnodes(WordList::iterator &itStart, WordList::iterator &itEnd,
						const int &iEndingLen, Rule *&rRule,
						int &iWordsThisPos, int &iWordsThisNeg, CoverNodeAlg *apSubNode[], CoverNodeAlg *nParent);
	static CoverNodeAlg *TryReduceSubnode(int &iWordsThisPos, int &iWordsThisNeg, CoverNodeAlg *&nSub);

	//-------------------------------------------------------------------------------------------	
	//change data structure of this tree
	RdrTree *GenRdrTree(bool bFirst = true) const;

	//-------------------------------------------------------------------------------------------	
	//output functions
	/* code working  but not needed any more because GenRdrTree()->ToString() function
	void PrintNode(ostream &os, WordList &wl, int iVerbose = 0) const;
	void PrintTree(ostream &os, WordList &wl, int iVerbose = 0, int iMaxLevel=-1, int iDepth=0) const;
	*/
	
	//-------------------------------------------------------------------------------------------	
	//lematization
	Rule *FindRule(char *acWord, char *acWordEnd);
	double TestAccurancy(WordList::iterator &itStart, WordList::iterator &itEnd, 
		                 int iVerbose=0, ostream &os=cout, ostream *osErr=NULL);

	//-------------------------------------------------------------------------------------------	
	//tree statistics
	void PrintStat(ostream &os, int iType=0) const;
	void TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
					int &iWordDepth, int &iMaxDepth, int &iLeaveNum, int iDepth = 0) const;
	
//data
public:
	//-------------------------------------------------------------------------------------------
	//ending, together with all parents ending
	char *acEnding;
	int iEndingLen;
	//how much characters were added in this node
	int iEndingNew;
	//ending is actualy whole word
	bool bEntireWord;
	
	//-------------------------------------------------------------------------------------------
	//rule for lematization of this 
	Rule *rRule;

	//-------------------------------------------------------------------------------------------
	//number of words coverd by this node.
	int iWordsAllNum;
	int iWordsThisPos;
	int iWordsThisNeg;

	//-------------------------------------------------------------------------------------------
	//subnodes
	int iSubNodesNum;
	CoverNodeAlg *apSubNode[256];

};