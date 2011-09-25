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

class JoelTree{
public:
	string			cond;
	string			conc;

	typedef vector<JoelTree *> lcrule;
	typedef vector<string> lstring;

	lstring words;
	lcrule exceps;

	int iWordPos;
	int iWordNeg;

	JoelTree(string w, string cd, string cc, bool startrule = false);
	~JoelTree();

	int getnum();

	bool operator==(const JoelTree &y) const;
	bool operator<(const JoelTree &y) const;

	void TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
							 int &iWordDepth, int &iMaxDepth, int &iLeaveNum, int iDepth=0) const;

	struct LessThanBackward;
	struct LessThanBackwardP;

};

struct JoelTree::LessThanBackward  {
	bool operator()(const JoelTree &w1, const JoelTree &w2 ) const {
		byte *acw1 = (byte *) w1.cond.c_str() + w1.cond.length()-1;
		byte *acw2 = (byte *) w2.cond.c_str() + w2.cond.length()-1;
		
		int i=0;
		while (acw1[i]==acw2[i] && acw2[i]!=NULL) i--;

		if (acw1[i]<acw2[i]) return true;
		else return false;
	}
};

struct JoelTree::LessThanBackwardP  {
	bool operator()(const JoelTree *w1, const JoelTree *w2 ) const {
		return LessThanBackward()(*w1,*w2);
	}
};

