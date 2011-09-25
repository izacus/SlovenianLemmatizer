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

#include "Word.h"
#include "Rule.h"

using namespace std;

//container for word list
class WordList  {

public:
	Word *aoWord;		//table of words (size = iWordNum)
	int iWordNum;		//number of words

	int iSetNum;		//number of diferent sets
	int *aiSetNum;		//numbers of words per set (size = iSetNum)
	int **aaiSetMap;	//mapings of words divided by sets into aoWord (size = iSetNum X aiSetNum[i])

	Rule *aoRule;		//table of rules (size = iRuleNum)
	int iRuleNum;		//number of rules

public:
	//itterator class for moving trough words
	class iterator;

public:
	//constructors/destructors
	WordList();
	WordList(const char *acFileName, const char cAddPrefix = NULL, int iVerbose = 0);
	~WordList();

	//copy constructors and assigment operator
	WordList(const WordList &wl);
	WordList(const WordList &wl, int iSet, bool bSetElem, bool bDummy);
	WordList &operator=(const WordList &wl);


	//initialization & delition
	void Initialize();
	void ClearSets();
	void Clear();

	//copy function
	void Copy(const WordList &wl);
	void SubsetCopy(const WordList &wl, int iSet, bool bSetElem);

	//import export
	void Import(const char *acFileName, const char cAddPrefix = NULL, int iVerbose = 0);
	void Export(const char *acFileName, const char cRemPrefix = NULL, int iVerbose = 0, bool bExtended = false);

	//generate rules
	void GenerateRules(int iVerbose = 0);

	//printing functions
	const char *ToString(const char cRemPrefix, bool bExtended = false);
	void Print(ostream &os, int &iLineCount, int &iByteCount, const char cRemPrefix, bool bExtended = false);

public:
	//returns word number
	int GetWordNum(int iSet=-1, bool bSetElem=true);
	//returns rule
	Rule &GetRule(int iRuleId) const;

	//iterator manipulation
	iterator *begin(int iSet=-1, bool bSetElem=true);
	iterator *rbegin(int iSet=-1, bool bSetElem=true);

	iterator *end(int iSet=-1, bool bSetElem=true);
	iterator *rend(int iSet=-1, bool bSetElem=true);
public:
	//sorting and splitting for crossvalidation
	void Sort(int iVerbose = 0, Word::LessThan::Type ltType = Word::LessThan::back);
	void SplitCrossValid(int iKFold, bool bDeepSplit = false, int iVerbose = 0);
	void SplitSubset(int iSubsetWordCount, bool bDeepSplit = false, int iVerbose = 0);
	void RecreateSets(int iVerbose = 0);
};

//-------------------------------------------------------------------------------------------
//iterator for word list
class WordList::iterator : public std::iterator<bidirectional_iterator_tag, Word>{
	friend class WordList;

	int iSet;
	bool bSetElem;
	WordList *wl;

	int iWordPos;
	int iMax;
private:
	//increment - decrement
	void Increment(bool bInc=true);

public:
	//returns attributes of current word
	int GetWordPos() const;
	int GetSet() const;
	int GetRuleId() const;
	Rule &GetRule() const;
	int GetWordNum() const;

public:
	//constructor
	iterator(WordList *wl, bool bForw, int iSet, bool bSetElem);

	//ins/dec operators
	iterator &operator++();
	iterator operator++(int dummy);
	iterator &operator--();
	iterator operator--(int dummy);
	iterator operator-(int iDec);

	//equality operator
	bool operator==(const iterator &y) const;
	bool operator!=(const iterator &y) const;
	bool operator>=(const iterator &y) const;

	//element pointer operator
	Word &operator*() const;
	Word *operator->();

};