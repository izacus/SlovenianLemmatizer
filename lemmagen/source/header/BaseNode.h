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

class BaseNode{
public:
	string			cond;
	string			conc;

	typedef vector<BaseNode *> lcrule;
	typedef vector<string> lstring;

	lstring words;
	lcrule exceps;

	int iWordPos;
	int iWordNeg;

	BaseNode(string w, string cd, string cc, bool startrule = false);
	~BaseNode();

	int getnum();

	bool operator==(const BaseNode &y) const;
	bool operator<(const BaseNode &y) const;

	static void tokenize(const string& str, vector<string> &tokens, const string& delimiters);
	RdrTree *GenRdrTree(bool bFirst = true) const;

	void TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
							 int &iWordDepth, int &iMaxDepth, int &iLeaveNum, int iDepth=0) const;

	struct LessThanBackward;
	struct LessThanBackwardP;

};

struct BaseNode::LessThanBackward  {
	bool operator()(const BaseNode &w1, const BaseNode &w2 ) const {
		byte *acw1 = (byte *) w1.cond.c_str() + w1.cond.length()-1;
		byte *acw2 = (byte *) w2.cond.c_str() + w2.cond.length()-1;
		
		int i=0;
		while (acw1[i]==acw2[i] && acw2[i]!=NULL) i--;

		if (acw1[i]<acw2[i]) return true;
		else return false;
	}
};

struct BaseNode::LessThanBackwardP  {
	bool operator()(const BaseNode *w1, const BaseNode *w2 ) const {
		return LessThanBackward()(*w1,*w2);
	}
};

