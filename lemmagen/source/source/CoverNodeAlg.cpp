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
#include "../header/CoverNodeAlg.h"

//-------------------------------------------------------------------------------------------	
//static aux 'constructor'
CoverNodeAlg *CoverNodeAlg::BuildTree(WordList &wl, int iSet, bool bSetElem, int iVerbose, ostream &os){
	Timing time;
	
	WordList::iterator *itStart = wl.begin(iSet,bSetElem);
	WordList::iterator *itEnd = wl.end(iSet,bSetElem);

	CoverNodeAlg *nNew = NULL;
	if (!(*itStart>=*itEnd))
		nNew = new CoverNodeAlg(*itStart, *itEnd, wl.GetWordNum(iSet));
		
	if (iVerbose==1) 
		if (nNew != NULL) 
			time.Print(os, "Learning algorithm (tree building) time ");
		else
			os << "Error while learning tree, no words given!";
	if (iVerbose==2)
		os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";

	delete itStart;
	delete itEnd;

	return nNew;
}

//-------------------------------------------------------------------------------------------
//Constructor
CoverNodeAlg::CoverNodeAlg(WordList::iterator &itStart, WordList::iterator &itEnd, int iWordNum,
		   const CoverNodeAlg *nParent, const char chNewChar) {

	//initialization of basic member variables
	for(int i=0; i<256; i++) apSubNode[i] = NULL;
	iSubNodesNum = 0;
	iWordsAllNum = iWordNum;
	bEntireWord = false;

	//suplementary variables 
	int iPlusLen = chNewChar == NULL ? 0 : 1;
	Rule *rParentRule = nParent==NULL ? NULL : nParent->rRule;

	//first aproximate to iEndingLen. One more than parent
	int iParentEndingLen = nParent==NULL ? 0 : (int)nParent->iEndingLen;
	iEndingLen = iParentEndingLen + iPlusLen;

	//are we looking at whole word and not the ending
	if (iEndingLen > 0 && iPlusLen == 0) bEntireWord = true;

	//if there is only one appliable rule than do't bother going on.
	Rule *rOneRule = GetBestRule(itStart, itEnd, -1,rParentRule, iWordsThisPos);

	//if there is only one rule in this set of words, then we just need to find shortest posible
	//ending that is >= parentEndigLen + 1 and >= rule.leftlen;
	//else there is more rules, we have to distinguish between them. Find max iEndingLen so our
	//subnodes will effectively distinguish different casses
	if (rOneRule!=NULL)
		iEndingLen = max(iEndingLen, (int)rOneRule->iLeftLen);
	else {
		//if word is not deterministic (many forms for same word) then here you can change algorithm behaviour
		//in this cases. If you comment next lines then shortest possible suffixes will be used for those words,
		//else longest possible will be used.
		WordList::iterator itPrevEnd = itEnd-1;
		iEndingLen = Str::eqEndLenFast(itStart->acWordEnd, itPrevEnd->acWordEnd, iEndingLen, min(itStart->iWordLen, itPrevEnd->iWordLen));
	}

	//now we have best posible endinglength, we are completley shure that 
	//all words have equal ending for at least last iEndingLen characters 
	//so we generate ending from first word in set
	acEnding = strcpy(new char[iEndingLen + 1], itStart->acWordEnd - iEndingLen);
	iEndingNew = iEndingLen - iParentEndingLen;

	//if we allready know that we are in leaf node than we don't continue
	//if we dont then find most apropriate node and create subnodes
	if (rOneRule!=NULL)	{
		iWordsThisNeg = iWordNum - iWordsThisPos;
		rRule = rOneRule;
	} 
	else {
		//find best rule for current ending 
		//ending is important because the longer it is, the more rules apples to it
		rRule = GetBestRule(itStart, itEnd, iEndingLen, rParentRule, iWordsThisPos);

		//we have best rule and ending, we know that there are exceptions so we look them
		iSubNodesNum = CreateSubnodes(itStart, itEnd, iEndingLen, 
									  rRule, iWordsThisPos, iWordsThisNeg, apSubNode, this);
	}

}

//-------------------------------------------------------------------------------------------
//Destructor
CoverNodeAlg::~CoverNodeAlg() {
	for(int i=0; i<256; i++) 
		if (apSubNode[i] != NULL)
			delete apSubNode[i];
	delete[] acEnding;
}

//-------------------------------------------------------------------------------------------
//Finds best posible rule for this node. 
//Also sets iWordsThisPos to the number of words that this best rule covers
//If iEndingLen == -1 then we are previwing words searching for one rule or all equal words
Rule *CoverNodeAlg::GetBestRule(WordList::iterator &itStart, WordList::iterator &itEnd,
						const int &iEndingLen, Rule *&rParentRule, int &iWordPositive) {
	
	bool bPreview = iEndingLen == -1;
							
	Rule *rule;
	Rule *rBestRule = NULL;
	int iRuleNum = 0;
	iWordPositive = 0;
	bool bSameWords = true;
	if (bPreview) bSameWords = strcmp(itStart->acWord, (itEnd-1)->acWord)==0;

	//reset rules to notseen
	for(WordList::iterator it=itStart; it!=itEnd; ++it)
		it.GetRule().bSeen = false;

	//find the strongest rule that can be applied
	for(WordList::iterator it=itStart; it!=itEnd; ++it) {
		rule = &it.GetRule();
		
		//if this is the first time we see this rule then initialize it's variables
		if(!rule->bSeen) {
			rule->bSeen = true;
			rule->iWordCount = 0;
			iRuleNum++;
			if (rule->iLeftLen <= iEndingLen) 
				rule->bAppliable = true;
			else
				rule->bAppliable = false;
		}

		if(bPreview || rule->bAppliable) {
			rule->iWordCount++;
			if(rule->iWordCount >= iWordPositive) {
				//if two rules have equal count give priority to the one that covers more words
				if(rule->iWordCount > iWordPositive || rule->iSupportCount > rBestRule->iSupportCount) {
					iWordPositive = rule->iWordCount;
					rBestRule = rule;
				}
			}
		}

		//if words are not the same and we have many rules than this function exits
		//active only when not preview (look at bSameWords assigment)
		if (!bSameWords && iRuleNum>1) return NULL;
	}

	if(bPreview) {
		//if we have more than one word and more than one rule than return nonsuccess
		if (!bSameWords && iRuleNum > 1) rBestRule = NULL;
	}
	else {
		//if no appropriate rule found use parent's rule
		if (rBestRule == NULL) rBestRule = rParentRule;
	}

	return rBestRule;
}

//-------------------------------------------------------------------------------------------
//creates subnodes and returns theirs count
int CoverNodeAlg::CreateSubnodes(WordList::iterator &itStart, WordList::iterator &itEnd,
						const int &iEndingLen, Rule *&rRule,
						int &iWordsThisPos, int &iWordsThisNeg, CoverNodeAlg *apSubNode[], CoverNodeAlg *nParent) {
	
	iWordsThisPos = 0;
	iWordsThisNeg = 0; //we are splitting to subnodes so negatives can't occure

	int iSubNodesNum = 0;
	int iWordsSub = 0;
	int iLookChar = -1-iEndingLen;
	bool iNeedNewSub = false;

	WordList::iterator itSubStart = itStart;
	WordList::iterator itSubEnd = itStart;

	byte subCharNext;
	if (iEndingLen < itStart->iWordLen)
		subCharNext = itStart->acWordEnd[iLookChar];
	else
		subCharNext = NULL;
	byte subCharThis = subCharNext;

	CoverNodeAlg *nSub;

	//look thorugh all the words
	while(itSubEnd!=itEnd) {
		//if rule is not the same as in this node we need new subgorup
		if (&itSubEnd.GetRule() != rRule) iNeedNewSub = true;
		++itSubEnd;
		++iWordsSub;

		//peek at the next word
		if (itSubEnd != itEnd)
			if (iEndingLen < itSubEnd->iWordLen)
				subCharNext = itSubEnd->acWordEnd[iLookChar];
			else
				subCharNext = NULL;

		//if we came to the end or next char is diferent then close the group
		if (itSubEnd == itEnd || subCharThis != subCharNext) {
			//closing subgroup, if neeed new group then add subnode
			if (iNeedNewSub){
				nSub = new CoverNodeAlg(itSubStart, itSubEnd, iWordsSub, nParent, subCharThis);
				if (nSub->rRule == rRule)
					nSub = TryReduceSubnode(iWordsThisPos, iWordsThisNeg, nSub);
				if (nSub!=NULL) {
					apSubNode[subCharThis] = nSub;
					iSubNodesNum ++;
				}
			} else {
				iWordsThisPos += iWordsSub;

			}

			//set the parameters for new subgroup
			subCharThis = subCharNext;
			itSubStart = itSubEnd;
			iNeedNewSub = false;
			iWordsSub = 0;
		}
	}

	return iSubNodesNum;
}

//-------------------------------------------------------------------------------------------
//if subnode has the same rule as this node, then maybe child can be reduced
CoverNodeAlg *CoverNodeAlg::TryReduceSubnode(int &iWordsThisPos, int &iWordsThisNeg, CoverNodeAlg *&nSub) {

	switch (nSub->iSubNodesNum) {
		//if subnode has the same rule and no children than eliminate it
		case 0:
			iWordsThisPos += nSub->iWordsThisPos;
			iWordsThisNeg += nSub->iWordsThisNeg;

			delete nSub;
			return NULL;

		//if subnode has the same rule and one child than point to that child
		case 1: 
			iWordsThisPos += nSub->iWordsThisPos;
			iWordsThisNeg += nSub->iWordsThisNeg;

			//find child's child and correct its ending new
			CoverNodeAlg *nSubSub = NULL;
			int iSub;
			for(iSub=0; nSubSub == NULL; iSub++) nSubSub = nSub->apSubNode[iSub];
			nSub->apSubNode[--iSub] = NULL;
			nSubSub->iEndingNew += nSub->iEndingNew;

			delete nSub;
			return nSubSub;
	}

	return nSub;
}

//-------------------------------------------------------------------------------------------
//Generates RdrTree from BaseAlg structure - it's the same tree but other data structure.
RdrTree *CoverNodeAlg::GenRdrTree(bool bFirst) const 
{	//create current node
	RdrTree *rt = new RdrTree();
	
	//fill it with data
	rt->acEnding = Str::copy((byte*) acEnding);
	rt->iEndingLen = iEndingLen;
	rt->rrRule = new RdrRule(strlen((char*)rRule->acLeft), Str::copy((byte*)rRule->acRight));
	rt->bEntireWord = bEntireWord;

	//create subnodes
	if (iSubNodesNum>0) {
		rt->deqSubNode = new RdrTree::deqrt();
		for (int i=0; i<256; ++i)
			if(apSubNode[i]!=NULL)
				rt->deqSubNode->push_back(apSubNode[i]->GenRdrTree(false));
	}

	//correct the data on the whole tree structure
	if (bFirst) 
		rt->SetNewSufxLen(0);

	return rt;
}

/* code working  but not needed any more because GenRdrTree()->ToString() function
//-------------------------------------------------------------------------------------------
//creates text representation of this node
void CoverNodeAlg::PrintNode(ostream &os, WordList &wl, int iVerbose) const {
	char *acRule = rRule->ToStirng("\"->\"");

	switch (iVerbose) {
		case 0:
			os <<
				"RULE:" <<  
				" i\"" <<  acEnding << "\"" <<
				" t\"" <<  acRule << "\"" <<
				(bEntireWord ? " ew" : "") << ";";
			break;
		case 1:
			os <<
				"RULE: (" <<
				" suffix('" << acEnding << "')" <<
				" transform('" << acRule << "')" <<
				(bEntireWord ? " entireword" : "") <<
				" );" <<
				" new char='" << acEnding[iEndingNew- (iEndingNew == 0 ? 0 : 1)] << "'" <<
				" new suffix(" << iEndingNew << " of " << iEndingLen << ")" <<
				" (rule '" << acRule << "')" <<
				" (words " << iWordsAllNum << "," <<
				" pos " << iWordsThisPos << "," <<
				" neg " << iWordsThisNeg << ")" <<
				" (sub " << iSubNodesNum << ")";
			break;
	}	
	delete[] acRule;
}

//-------------------------------------------------------------------------------------------
//creates text representation of this node
void CoverNodeAlg::PrintTree(ostream &os, WordList &wl, int iVerbose, int iMaxLevel, int iDepth) const {
	PrintNode(os, wl, iVerbose);
	
	if (iMaxLevel!=0 && iSubNodesNum>0) { 
		os << endl;
		os << setfill('\t') << setw(iDepth) << "" << "{:";

		for(int i=0; i<256; i++) 
			if (apSubNode[i] != NULL) {
				os << "\t";
				apSubNode[i]->PrintTree(os, wl, iVerbose, iMaxLevel-1, iDepth + 1);
				os << endl << setfill('\t') << setw(iDepth) << "";
			}

		os << ":}";
		os << endl;
	}
			
}

*/

//-------------------------------------------------------------------------------------------
//creates text representation of statistics of this nodes tree/subtree
void CoverNodeAlg::PrintStat(ostream &os, int iType) const {
	int iPosNum, iNegNum, iTreeSize, iSumWidth;
	int iWordDepth, iMaxDepth, iLeaveNum;

	TreeStatistics(iPosNum, iNegNum, iTreeSize, iSumWidth, 
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

//-------------------------------------------------------------------------------------------
//finds appropriate rule given word
Rule *CoverNodeAlg::FindRule(char *acWord, char *acWordEnd) {
	//if this rule applies only to whole words then chack if whole word is equal to ending else wrong rule
	if (bEntireWord)
		if (strcmp(acEnding,acWord)==0) 
			return rRule;
		else
			return NULL;

	char *acWordNewEnd = acWordEnd - iEndingNew;

	//check if ending is in limits of this word else signalize wrong rule
	if (acWordNewEnd<acWord) return NULL;

	//check if word complies with this rule else signalize wrong rule
	if (iEndingNew>1) if (strncmp(acEnding,acWordNewEnd,iEndingNew-1)!=0) return NULL;
	
	Rule *rReturnRule = NULL;
	
	byte bNextLetter;
	if (acWordNewEnd!=acWord)
		bNextLetter = (byte)(acWordNewEnd[-1]);
	else
		bNextLetter = NULL;

	if (apSubNode[bNextLetter] != NULL)
		rReturnRule = apSubNode[bNextLetter]->FindRule(acWord, acWordNewEnd);

	return (rReturnRule==NULL) ? rRule : rReturnRule;
}

//-------------------------------------------------------------------------------------------
//test accurancy on test word set given
double CoverNodeAlg::TestAccurancy(WordList::iterator &itStart, WordList::iterator &itEnd, int iVerbose, ostream &os, ostream *osErr) {
	
	if (iVerbose==1)
		os << "  Starting test lemmatization on " << itStart.GetWordNum() << " words." << endl;

	int iTrue = 0;
	Rule *rCurrRule = 0;
	char *acLemmTest;

	Timing time;
	for (WordList::iterator it = itStart; it != itEnd; ++it) {
		
		rCurrRule = FindRule(it->acWord, it->acWordEnd);
		
		acLemmTest = rCurrRule->Lematize(it->acWord);
		
		if (strcmp(it->acLemm, acLemmTest) == 0)
			iTrue++;
		else
			if (osErr != NULL) (*osErr) << it->acWord << " --> " << acLemmTest << " (" << it->acLemm << ")" << endl;

		delete[] acLemmTest;
	}

	double dAccurancy = -1;
	double dTimeNs = -1;

	if (itStart.GetWordNum() > 0) {
		dAccurancy = (double) iTrue*100 / itStart.GetWordNum();
		dTimeNs = time * 1000000000 / itStart.GetWordNum();
	}

	if (iVerbose==1) {
		os << "    Lematization completed with " << iTrue << " positives and " << itStart.GetWordNum()-iTrue << " errors." << endl;
		os << setprecision(4) << fixed << "    Accuracy: " << dAccurancy << "%." << endl;
		os << "    Time needed for lemmatization " << -time;
		os << ", per word " << fixed << setprecision(0) << dTimeNs << "ns (not optimized)." << endl;
	}
	if (iVerbose==2) {
		os << "#" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dAccurancy << "% ";
		os << "|" << setw(10) << setfill(' ') << right << iTrue << " ";
		os << "|" << setw(8) << setfill(' ') << right << fixed << setprecision(0) << dTimeNs << " ";
	}	

	return dAccurancy;
}

//-------------------------------------------------------------------------------------------
//returns tree statistic parameters
void CoverNodeAlg::TreeStatistics(int &iPosNum, int &iNegNum, int &iTreeSize, int &iSumWidth, 
						 int &iWordDepth, int &iMaxDepth, int &iLeaveNum, int iDepth) const{
	iPosNum=iWordsThisPos;
	iNegNum=iWordsThisNeg;
	iTreeSize=1;
	iSumWidth=iSubNodesNum;
	iWordDepth=(iPosNum + iNegNum)*iDepth;
	iMaxDepth=iDepth;
	iLeaveNum=iSubNodesNum==0?1:0;

	int iPosNumSub;
	int iNegNumSub;
	int iTreeSizeSub;
	int iSumWidthSub;
	int iWordDepthSub;
	int iMaxDepthSub;
	int iLeaveNumSub;

	for(int i=0; i<256; i++) 
		if (apSubNode[i] != NULL) {
			apSubNode[i]->TreeStatistics(iPosNumSub, iNegNumSub, iTreeSizeSub, iSumWidthSub, 
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