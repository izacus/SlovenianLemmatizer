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
#include "../header/BaseNode.h"

//-------------------------------------------------------------------------------------------
BaseNode::BaseNode(string w, string cd, string cc, bool startrule) {
	iWordPos = 0;
	iWordNeg = 0;

	if (!startrule) iWordPos++;
	
	if (w != "")
		words.push_back(w);
	cond = cd;
	conc = cc;
}

//-------------------------------------------------------------------------------------------
BaseNode::~BaseNode() {
	words.clear();
	lcrule::iterator i;

	for (i = exceps.begin(); i != exceps.end(); ++i)
		delete *i;
}

//-------------------------------------------------------------------------------------------
int BaseNode::getnum() {
	int iReturn = 1;

	words.clear();
	lcrule::iterator i;

	for (i = exceps.begin(); i != exceps.end(); ++i)
		iReturn += (*i)->getnum();

	return iReturn;
}

//-------------------------------------------------------------------------------------------
bool BaseNode::operator==(const BaseNode &y) const {
	return (LessThanBackward()(*this,y) == false && LessThanBackward()(y,*this) == false );
}

//-------------------------------------------------------------------------------------------
bool BaseNode::operator<(const BaseNode &y) const {
	return LessThanBackward()(*this,y) == false;
}

//-------------------------------------------------------------------------------------------
//Cut a line into tokens given a delimiter.
void BaseNode::tokenize(const string& str, vector<string> &tokens, const string& delimiters)
{
	string::size_type	lastPos = 0;
	string::size_type	pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}

//-------------------------------------------------------------------------------------------
//Generates RdrTree from BaseAlg structure - it's the same tree but other data structure.
RdrTree *BaseNode::GenRdrTree(bool bFirst) const 
{	//create current node
	RdrTree *rt = new RdrTree();
	
	//fill it with data
	rt->SetEnding(Str::copy((byte*) cond.c_str()));

	//--- get condition from and condition to strings for rule generation
	vector<string>	tokens;
	string			sufFrom;
	string			sufTo;
	tokenize(conc, tokens, "\t");
	(tokens.size() > 0) ? sufFrom = tokens[0] : sufFrom = "";
	(tokens.size() > 1) ? sufTo = tokens[1] : sufTo = "";

	//--- generate rule
	rt->rrRule = new RdrRule(strlen((char*)sufFrom.c_str()), Str::copy((byte*)sufTo.c_str()));
	
	//entireword is always false
	rt->bEntireWord = false;

	//create subnodes
	if (exceps.size()>0) {
		rt->deqSubNode = new RdrTree::deqrt();

		for (lcrule::const_iterator i = exceps.begin(); i != exceps.end(); ++i) 
			rt->deqSubNode->push_back((*i)->GenRdrTree(false));
	}

	//correct the data on the whole tree structure
	if (bFirst) 
		rt->SetNewSufxLen(0);

	return rt;
}

//-------------------------------------------------------------------------------------------
//returns tree statistic parameters
void BaseNode::TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
						 int &iWordDepth, int &iMaxDepth, int &iLeaveNum, int iDepth) const{

	iPosNum=iWordPos;
	iNegNum=iWordNeg;
	iTreeSize=1;
	iSumWidth=exceps.size();
	iWordDepth=(iPosNum + iNegNum)*iDepth;
	iMaxDepth=iDepth;
	iLeaveNum=iSumWidth==0?1:0;

	int iPosNumSub;
	int iNegNumSub;
	int iTreeSizeSub;
	int iSumWidthSub;
	int iWordDepthSub;
	int iMaxDepthSub;
	int iLeaveNumSub;

	lcrule::const_iterator i;
	for (i = exceps.begin(); i != exceps.end(); ++i) {
			(*i)->TreeStatistics(iPosNumSub, iNegNumSub, iTreeSizeSub, iSumWidthSub, 
				                     iWordDepthSub, iMaxDepthSub, iLeaveNumSub, iDepth + 1);
			iPosNum += iPosNumSub;
			iNegNum += iNegNumSub;
			iTreeSize += iTreeSizeSub;
			iSumWidth += iSumWidthSub;
			iWordDepth += iWordDepthSub;
			iMaxDepth = max(iMaxDepthSub,iMaxDepth);
			iLeaveNum += iLeaveNumSub;
		}
}


