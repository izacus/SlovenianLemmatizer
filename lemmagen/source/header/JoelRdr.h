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

#include  "../header/JoelTree.h"

class JoelRdr {
public:
	JoelTree			*defaultRule;

	JoelRdr();
	~JoelRdr();

	void train(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose=0, ostream &os=cout);
	double lemmatize(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose=0, ostream &os=cout);

	JoelTree *findRule(JoelTree *r, string &w, string &cond, string &conc, bool learn = false);

	void getTransfo(string &w, string &lem, string &cond, string &conc);
	void updateRule(JoelTree *r, string &w, string &cond, string &conc);
	void createNewExcep(JoelTree *r, string &w, string &cd, string &cc);

	bool diff(JoelTree *r, string cd);
	bool hasSuf(string &w, string &suf);
	void tokenize(const string& str, vector<string> &tokens, const string& delimiters);
	void applyRule(JoelTree *r, string &w, string &result);

	void PrintTree(ostream *fdRules, string tab=string(""), JoelTree *r = NULL, int depth = 0);
	void PrintStat(ostream &os, int iType) const;
};

