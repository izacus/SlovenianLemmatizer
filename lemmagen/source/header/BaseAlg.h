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

#include  "BaseNode.h"

class BaseAlg {
public:
	BaseNode			*defaultRule;

	BaseAlg();
	~BaseAlg();

	void train(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose=0, ostream &os=cout);
	double lemmatize(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose=0, ostream &os=cout);

	BaseNode *findRule(BaseNode *r, string &w, string &cond, string &conc, bool learn = false);

	void getTransfo(string &w, string &lem, string &cond, string &conc);
	void updateRule(BaseNode *r, string &w, string &cond, string &conc);
	void createNewExcep(BaseNode *r, string &w, string &cd, string &cc);

	bool diff(BaseNode *r, string cd);
	bool hasSuf(string &w, string &suf);
	void applyRule(BaseNode *r, string &w, string &result);

	RdrTree *GenRdrTree(bool bFirst = true) const;

	/* code working  but not needed any more because GenRdrTree()->ToString() function
	void PrintTree(ostream *fdRules, string tab=string(""), BaseNode *r = NULL, int depth = 0);
	*/

	void PrintStat(ostream &os, int iType) const;

};

