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
#include "../header/JoelTree.h"

//-------------------------------------------------------------------------------------------
JoelTree::JoelTree(string w, string cd, string cc, bool startrule) {
	iWordPos = 0;
	iWordNeg = 0;

	if (!startrule) iWordPos++;
	
	if (w != "")
		words.push_back(w);
	cond = cd;
	conc = cc;
}

//-------------------------------------------------------------------------------------------
JoelTree::~JoelTree() {
	words.clear();
	lcrule::iterator i;

	for (i = exceps.begin(); i != exceps.end(); ++i)
		delete *i;
}

//-------------------------------------------------------------------------------------------
int JoelTree::getnum() {
	int iReturn = 1;

	words.clear();
	lcrule::iterator i;

	for (i = exceps.begin(); i != exceps.end(); ++i)
		iReturn += (*i)->getnum();

	return iReturn;
}

//-------------------------------------------------------------------------------------------
bool JoelTree::operator==(const JoelTree &y) const {
	return (LessThanBackward()(*this,y) == false && LessThanBackward()(y,*this) == false );
}

//-------------------------------------------------------------------------------------------
bool JoelTree::operator<(const JoelTree &y) const {
	return LessThanBackward()(*this,y) == false;
}

//-------------------------------------------------------------------------------------------
//returns tree statistic parameters
void JoelTree::TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
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


