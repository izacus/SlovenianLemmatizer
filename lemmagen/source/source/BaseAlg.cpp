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
#include  "../header/BaseAlg.h"

//-------------------------------------------------------------------------------------------
BaseAlg::BaseAlg() {}

//-------------------------------------------------------------------------------------------
BaseAlg::~BaseAlg() {delete defaultRule;}

//-------------------------------------------------------------------------------------------
//Train a model from a file.
void BaseAlg::train(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose, ostream &os)
{	int iWordNum = itStart.GetWordNum();

	string **w = new string*[iWordNum];
	string **l = new string*[iWordNum];
	string **cond = new string*[iWordNum];
	string **conc = new string*[iWordNum];

	BaseNode			*r;
	defaultRule = new BaseNode(string(""), string(""), string(""), true);
	
	int i=0;
	for(WordList::iterator it = itStart; it != itEnd; ++it, ++i) {
		w[i] = new string(it->acWord);
		l[i] = new string(it->acLemm);
		cond[i] = new string();
		conc[i] = new string();
		getTransfo(*w[i], *l[i], *cond[i], *conc[i]);
	}


	if (iVerbose==1) cout << "Rdr algorithm: Starting train..." << endl;
	clock_t	clkStart = clock();

	Timing time;

	for(int i=0; i<iWordNum; i++) {
		r = findRule(defaultRule, *w[i], *cond[i], *conc[i]);
		updateRule(r, *w[i], *cond[i], *conc[i]);
	}

	if (iVerbose==2)
		os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";

	int iTreeSize = defaultRule->getnum();

	if (iVerbose==1) {
		os << "  Train completed. Words: " << iWordNum << " TreeSize: " << iTreeSize << endl;
		double dTime = (double(clock())-double(clkStart))/CLOCKS_PER_SEC;
		os << "  Time needed for train " << dTime << endl;
		os.flush();
	}


	for(int i=0; i<iWordNum; i++) {	delete w[i]; delete cond[i]; delete conc[i]; delete l[i];}
	delete[] w; delete[] cond; delete[] conc; delete[] l;
}

//-------------------------------------------------------------------------------------------
//Lemmatize words from a file in tabular form.
double BaseAlg::lemmatize(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose, ostream &os)
{	int iWordNum = itStart.GetWordNum();

	string **w = new string*[iWordNum];
	string **l = new string*[iWordNum];
	string **cond = new string*[iWordNum];
	string **conc = new string*[iWordNum];

	string result;
	BaseNode *r;

	int i=0;
	for(WordList::iterator it = itStart; it != itEnd; ++it, ++i) {
		w[i] = new string(it->acWord);
		l[i] = new string(it->acLemm);
		cond[i] = new string();
		conc[i] = new string();
		getTransfo(*w[i], *l[i], *cond[i], *conc[i]);
	}

	int iTrue=0;

	if (iVerbose==1) os << "  Starting test lemmatization..." << endl;
	Timing time;

	for(int i=0; i<iWordNum; i++) {
		r = findRule(defaultRule, *w[i], *cond[i], *conc[i]);
		applyRule(r, *w[i], result);
		if (result == *l[i]) iTrue++;
	}

	double dAccurancy = -1;
	double dTimeNs = -1;

	if (itStart.GetWordNum() > 0) {
		dAccurancy = (double) iTrue*100 / itStart.GetWordNum();
		dTimeNs = time * 1000000000 / itStart.GetWordNum();
	}

	if (iVerbose==1) {
		os << "    Lematization completed with " << iTrue << " positives and " << iWordNum-iTrue << " errors." << endl;
		os << setprecision(4) << fixed << "    Accuracy: " << dAccurancy << "%." << endl;
		os << "    Time needed for lemmatization " << -time;
		os << ", per word " << fixed << setprecision(0) << dTimeNs << "ns (not optimized)." << endl;
	} 
	if (iVerbose==2) {
		os << "#" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dAccurancy << "% ";
		os << "|" << setw(10) << setfill(' ') << right << iTrue << " ";
		os << "|" << setw(8) << setfill(' ') << right << fixed << setprecision(0) << dTimeNs << " ";

	}		

	for(int i=0; i<iWordNum; i++) {	delete w[i]; delete cond[i]; delete conc[i]; delete l[i];}
	delete[] w; delete[] cond; delete[] conc; delete[] l;

	return dAccurancy;
}

//-------------------------------------------------------------------------------------------
//Returns a rule that fires for this word.
BaseNode *BaseAlg::findRule(BaseNode *r, string &w, string &cond, string &conc, bool learn)
{
	vector<BaseNode *>::iterator		e;
	BaseNode						*er;
	
	if (!hasSuf(w, r->cond))
		return (NULL);
	if (r->exceps.empty())
		return (r);
	for (e = r->exceps.begin(); e != r->exceps.end(); ++e)
	{
		er = findRule(*e, w, cond, conc);
		if (er != NULL)
			return (er);
	}
	return (r);
}

//-------------------------------------------------------------------------------------------
//Given two words: w (the word form) and lem (the lemma), it finds the 
//transformation to apply in order to get lem from w. The transformation is 
//expressed in the following format: suf1 "\t" suf2 which means remove suffix1 
//from w and replace it by suffix2 to get lem.
void BaseAlg::getTransfo(string &w, string &lem, string &cond, string &conc)
{
	string::const_iterator	wi;
	string::const_iterator	li;
	int						i;

	wi = w.begin();
	li = lem.begin();
	for (i = 0; wi != w.end() && li != lem.end() && (*wi == *li); ++wi, ++li, ++i);
	cond = w.substr(i, w.size() - i);
	conc = cond + "\t" + lem.substr(i, lem.size() - i);
	if (cond == "")
		cond = w[w.length() - 1];
}

//-------------------------------------------------------------------------------------------
//Tries to create an exception if the class of the new sample is different
//than the one of the rule that cover the sample.
void BaseAlg::updateRule(BaseNode *r, string &w, string &cond, string &conc)
{
	if (r->conc == conc) {
		r->iWordPos++;
		r->words.push_back(w);
	}
	else
		createNewExcep(r, w, cond, conc);
}

//-------------------------------------------------------------------------------------------
//Creates a new exception.
void BaseAlg::createNewExcep(BaseNode *r, string &w, string &cd, string &cc)
{
	BaseNode			*rule;
	dword	lc;

	lc = (dword) cd.length();
	//Find a condition for the new rule that differentiate it from the 
	//previous rule.
	for (; lc <= w.length() && !diff(r, w.substr((w.length() - lc), lc)); ++lc);
	if (lc <= w.length())
	{	
		rule = new BaseNode(w, w.substr((w.length() - lc), lc), cc);
		r->exceps.push_back(rule);
	} else {
		r->iWordNeg++;
	}
}

//-------------------------------------------------------------------------------------------
//When a new exception is created for one word, it should be chosen so that 
//the words covered by the previous rule are not also covered (by mistake) 
//by the new rule. It looks at the vector of words covered by the previous rule 
//and choose a condition (suffix) that excludes all the words covered.
bool BaseAlg::diff(BaseNode *r, string cd)
{
	vector<string>::iterator	i;

	for (i = r->words.begin(); (i != r->words.end()) && !hasSuf(*i, cd); ++i);
	if (i == r->words.end())
		return (true);
	return (false);
}

//-------------------------------------------------------------------------------------------
//Determines weither the word w has the suffix suf.
bool BaseAlg::hasSuf(string &w, string &suf)
{
	if (w.length() < suf.length())
		return (false);
	if (w.rfind(suf, w.length()) == (w.length() - suf.length()))
		return (true);
	return (false);
}

//-------------------------------------------------------------------------------------------
//Given two words: w (the word form) and lem (the lemma), it finds the 
//transformation to apply in order to get lem from w. The transformation is 
//expressed in the following format: suf1 "\t" suf2 which means remove suffix1 
//from w and replace it by suffix2 to get lem.
void BaseAlg::applyRule(BaseNode *r, string &w, string &result)
{
	vector<string>	tokens;
	string			suf1;
	string			suf2;

	BaseNode::tokenize(r->conc, tokens, "\t");
	(tokens.size() > 0) ? suf1 = tokens[0] : suf1 = "";
	(tokens.size() > 1) ? suf2 = tokens[1] : suf2 = "";
	result = w.substr(0, (w.length() - suf1.length())) + suf2;
}	

//-------------------------------------------------------------------------------------------
//Generates RdrTree from BaseAlg structure - it's the same tree but other data structure.
RdrTree *BaseAlg::GenRdrTree(bool bFirst) const 
{	return defaultRule->GenRdrTree(bFirst);
}

/* code working  but not needed any more because GenRdrTree()->ToString() function
//-------------------------------------------------------------------------------------------
//Writes the rules to a file.
void BaseAlg::PrintTree(ostream *fdRules, string tab, BaseNode *r, int depth)
{	if (r == NULL) r = defaultRule;

	vector<BaseNode *>::iterator		i;

	vector<string>	tokens;
	string			suf1;
	string			suf2;

	BaseNode::tokenize(r->conc, tokens, "\t");
	(tokens.size() > 0) ? suf1 = tokens[0] : suf1 = "";
	(tokens.size() > 1) ? suf2 = tokens[1] : suf2 = "";

	string conc = "\"" + suf1 + "\" --> \"" + suf2 + "\"";

	(*fdRules) << tab;
	(*fdRules) << "RULE:";
	(*fdRules) << " if(\"" << r->cond << "\")";
	(*fdRules) << " then(" << conc << ")";
	if (r->exceps.size()>0) (*fdRules) << " except(" << r->exceps.size() << ")";
	(*fdRules) << ";";

	if (r->exceps.size()>0) {
		(*fdRules) << "{:\n";
		for (i = r->exceps.begin(); i != r->exceps.end(); ++i)
			PrintTree(fdRules, tab + "\t", *i, depth+1);
		(*fdRules) << tab + "\t" << ":}";
	}

	(*fdRules) << endl;
}
*/

//-------------------------------------------------------------------------------------------
//creates text representation of statistics of this nodes tree/subtree
void BaseAlg::PrintStat(ostream &os, int iType) const {
	int iPosNum, iNegNum, iTreeSize, iSumWidth;
	int iWordDepth, iMaxDepth, iLeaveNum;

	defaultRule->TreeStatistics(iPosNum, iNegNum, iTreeSize, iSumWidth, 
				iWordDepth, iMaxDepth, iLeaveNum);
	
	int iInternal = iTreeSize - iLeaveNum;
	double dBranching = (double) iSumWidth / iInternal;
	int iWords = iPosNum + iNegNum;
	double dAccurancy = (double) iPosNum*100 / iWords;
	double dError = (double) iNegNum*100 / iWords;
	double dAvgDepth = (double) iWordDepth / iWords;

	switch(iType) {
		case 0:	//user
			os << "  Tree statistics:" << endl <<
				"    Nodes       " << iTreeSize << endl <<
				"      Leaves    " << iLeaveNum << endl <<
				"      Internal  " << iInternal << endl <<
				"      Max Depth " << iMaxDepth << endl <<
				"      Branching " << fixed << setprecision(2) << dBranching << endl <<
				"    Words       " << iWords << endl <<
				"      Positives " << iPosNum << endl <<
				"      Negatives " << iNegNum << endl <<
				"      Accurancy " << fixed << setprecision(4) << dAccurancy << "%" << endl <<
				"      Error     " << fixed << setprecision(4) << dError << "%" << endl <<
				"      Avg Depth " << dAvgDepth << endl;
			break;
		case 1: //title lines
			os << "|" << setw(7)  << setfill(' ') << right << "Nodes" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Leaves" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Inter" << " ";
			os << "|" << setw(6)  << setfill(' ') << right << "Depth" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Branch" << " ";
			os << "#" << setw(7)  << setfill(' ') << right << "Words" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Pos" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Neg" << " ";
			os << "|" << setw(11) << setfill(' ') << right << "Acc" << " ";
			os << "|" << setw(11) << setfill(' ') << right << "Err" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "AvgDep" << " ";
			break;
		case 2: //data lines
			os << "|" << setw(7)  << setfill(' ') << right << iTreeSize << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iLeaveNum << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iInternal << " ";
			os << "|" << setw(6)  << setfill(' ') << right << iMaxDepth << " ";
			os << "|" << setw(7)  << setfill(' ') << right << fixed << setprecision(2) << dBranching << " ";
			os << "#" << setw(7)  << setfill(' ') << right << iWords << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iPosNum << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iNegNum << " ";
			os << "|" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dAccurancy << "% ";
			os << "|" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dError << "% ";
			os << "|" << setw(7)  << setfill(' ') << right << fixed << setprecision(2) << dAvgDepth << " ";
			break;
	}	
}


