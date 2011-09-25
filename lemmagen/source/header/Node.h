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

#define _WORDSEQ
#ifdef WORDSEQ
	typedef deque<int> dequei;
#endif

class Node {
	
//code
public:
	//-------------------------------------------------------------------------------------------	
	//static aux 'constructor'
	static Node *BuildTree(WordList &wl, int iSet = -1, bool bSetElem=false, int iVerbose=0, ostream &os=cout);

	//-------------------------------------------------------------------------------------------	
	//constructor & destructor
	Node(WordList::iterator &itStart, WordList::iterator &itEnd, int iWordNum,
	     const Node *nParent=NULL, const 
		 char chNewChar=NULL);
	~Node();

	//-------------------------------------------------------------------------------------------	
	//static aux functions for building node tree
	static Rule *GetBestRule(WordList::iterator &itStart, WordList::iterator &itEnd,
					  const int &iEndingLen, Rule *&rParentRule, int &iWordsThisPos);
#ifndef WORDSEQ
	static
#endif
	int CreateSubnodes(WordList::iterator &itStart, WordList::iterator &itEnd,
						const int &iEndingLen, Rule *&rRule,
						int &iWordsThisPos, int &iWordsThisNeg, Node *apSubNode[], Node *nParent);
#ifndef WORDSEQ
	static
#endif
	Node *TryReduceSubnode(int &iWordsThisPos, int &iWordsThisNeg, Node *&nSub);

#ifdef WORDSEQ
	//-------------------------------------------------------------------------------------------	
	dequei *GetWordSeq(dequei *deqGlobWordSeq=NULL);
	
#endif

	//-------------------------------------------------------------------------------------------	
	//output functions
	void PrintNode(ostream &os, WordList &wl, int iVerbose = 0) const;
	void PrintTree(ostream &os, WordList &wl, int iVerbose = 0, int iMaxLevel=-1, int iDepth=0) const;
	void PrintStat(ostream &os, int iType=0) const;

	//-------------------------------------------------------------------------------------------	
	//lematization
	Rule *FindRule(char *acWord, char *acWordEnd);
	double TestAccurancy(WordList::iterator &itStart, WordList::iterator &itEnd, 
		                 int iVerbose=0, ostream &os=cout, ostream *osErr=NULL);

	//-------------------------------------------------------------------------------------------	
	//tree statistics
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
	bool bWholeWord;
	
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
	Node *apSubNode[256];

#ifdef WORDSEQ
	dequei *deqWordSeq;
#endif

};