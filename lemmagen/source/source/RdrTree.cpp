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
#include "../header/RdrTree.h"

//-------------------------------------------------------------------------------------------
//constructor
RdrTree::RdrTree() {
	acId = NULL;
	acEnding = NULL;
	rrRule = NULL;
	deqSubNode = NULL;
	bEntireWord = false;
	iOrder = 0;
	iEndingLen = 0;
	iNewSufxLen = 0;
	iAddress = 0;
}

//-------------------------------------------------------------------------------------------
//destructor
RdrTree::~RdrTree() {
	delete[] acId;
	delete[] acEnding;
	if (--(rrRule->iObjectCounter) == 0) delete rrRule;
	
	if (deqSubNode!= NULL)
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
			delete (*it);

	delete deqSubNode;
}
//-------------------------------------------------------------------------------------------
//sets ending and endinglen
void RdrTree::SetEnding(byte *acEnding){
	this->acEnding = acEnding;
	iEndingLen = strlen((char*)acEnding);
}

//-------------------------------------------------------------------------------------------
//creates text representation of this node
char *RdrTree::NodeToString(PrintStyle ps) const {
	ostringstream oo;

	//rule start
	oo << tblFormat[ps][0];
	
	//rule id
	if (acId != NULL && *tblFormat[ps][1]!=NULL) 
		oo<< tblFormat[ps][1] << (char*) acId << tblFormat[ps][2];
	
	//suffix
	oo << tblFormat[ps][3] << acEnding << tblFormat[ps][4];
	
	//entire word
	if (bEntireWord) oo << tblFormat[ps][14];
	
	//rule transformation
	char *acRule = rrRule->ToString((char*) acEnding, tblFormat[ps][6]);
	oo << tblFormat[ps][5] << acRule << tblFormat[ps][7];
	delete[] acRule;
	
	//exceptions
	if(deqSubNode!=NULL && *tblFormat[ps][8]!=NULL)
		oo << tblFormat[ps][8] << (deqSubNode!=NULL ? deqSubNode->size() : 0) << tblFormat[ps][9];
	
	//rule end
	oo << tblFormat[ps][10];

	string s = oo.str();
	return strcpy(new char[s.length()+1],s.c_str());
}

//-------------------------------------------------------------------------------------------
//prints node and all its subnodes to output stream
void RdrTree::Print(PrintStyle ps, ostream &os, int iMaxLevel,
	const char *acTab, const char *acTabCumul, int iDepth, bool bLast) const {

		//set acTab to defatul
		if (acTab[0]=='@' && acTab[1]=='#' && acTab[2]=='$' && acTab[3]==NULL) acTab = tblFormat[ps][11];

		//new tabulator is emptz string with one empty character space at the end
		char *acTabNew = new char[2];
		acTabNew[0] = NULL;

		//if user wants to draw lines
		if (tblFormat[ps][13][0]=='Y') {
			int iLenCum = strlen(acTabCumul);

			//nastavim predlogo tega
			char *acThis = strcpy(new char[iLenCum + strlen(tblFormat[ps][12]) + 2], acTabCumul);
			strcat(acThis, "#");
			strcat(acThis, tblFormat[ps][12]);
			if (bLast) acThis[iLenCum] = tblFormat[ps][13][6];
			else acThis[iLenCum] = tblFormat[ps][13][5];

			//nastavim predlogo otrok (with one empty character space at the end)
			delete[] acTabNew;
			acTabNew = strcpy(new char[iLenCum + strlen(acTab) + 3], acTabCumul);
			strcat(acTabNew, "#");
			strcat(acTabNew, acTab);
			if (bLast) acTabNew[iLenCum] = tblFormat[ps][13][7];
			else acTabNew[iLenCum] = tblFormat[ps][13][4];

			//output and clean
			os << acThis;
			delete[] acThis;
		} 
		//no lines
		else {
			for(int i=0; i<iDepth; i++)
				os << acTab;
		}

		//print this rule
		char *acNodeStr = NodeToString(ps);
		os << acNodeStr;
		delete[] acNodeStr;

		if (deqSubNode != NULL && iMaxLevel != iDepth && deqSubNode->size()>0) {
			deqrt::iterator it=deqSubNode->begin();
			//section start sign
			os << " {:\n";

			//empty line before subsection start
			if (tblFormat[ps][13][1]=='Y') {
				int iLen = strlen(acTabNew);
				acTabNew[iLen] = tblFormat[ps][13][4];
				acTabNew[iLen+1] = NULL;
				os << acTabNew << "\n";
				acTabNew[iLen] = NULL;
			}

			//subsection
			for(; (it+1)!=deqSubNode->end(); ++it)
				(*it)->Print(ps, os, iMaxLevel, acTab, acTabNew, iDepth + 1, false);
			(*it)->Print(ps, os, iMaxLevel, acTab, acTabNew, iDepth + 1, true);

			//section end sign
			os << " :}";

			//empty line after subsection end
			if (tblFormat[ps][13][2]=='Y') os << "\n" << acTabNew;
		}

		//clean variable
		delete[] acTabNew;

		//if last line in section don't append newline, parent will do that
		if (!bLast)	os << "\n";
}


//-------------------------------------------------------------------------------------------
//creates text representation of statistics of this nodes tree/subtree
void RdrTree::PrintStat(ostream &os, int iType) const {
	mapii mapDepthIntrDist;
	mapii mapDepthLeafDist;
	mapii mapWidthDist;

	TreeStatistics(mapDepthIntrDist, mapDepthLeafDist, mapWidthDist);

	ostringstream ooDepth, ooWidth1, ooWidth2; 

	int iAvgDepth = 0;
	int iAllInternal = 0;
	int iAllLeaf = 0;
	int iAvgWidth = 0;

	ooDepth << "    Distribution of internal and leaf nodes by depth:" << endl <<
		"      " << "|Depth|Internal|  Leaf |Together|" << endl;
	int iMaxDepth = max(mapDepthIntrDist.rbegin()->first, mapDepthLeafDist.rbegin()->first);
	for (int iDepth = 0; iDepth<=iMaxDepth; iDepth++) {
		int iNum = mapDepthIntrDist[iDepth] + mapDepthLeafDist[iDepth];
		iAvgDepth += iDepth * iNum;
		iAllInternal += mapDepthIntrDist[iDepth];
		iAllLeaf += mapDepthLeafDist[iDepth];

		ooDepth << "      " << right <<
			"|" << setw(4) << iDepth <<
			" |" << setw(7) << mapDepthIntrDist[iDepth] << 
			" |" << setw(6) << mapDepthLeafDist[iDepth] << 
			" |" << setw(7) << iNum <<
			" |" << endl;
	}

	ooWidth1 << "    Distribution of first 10 most frequent branchings:" << endl;
	ooWidth1 << "      Branc ";
	ooWidth2 << "      Numbr ";

	int iWidthIt = 0;
	for(mapii::iterator it = mapWidthDist.begin(); it != mapWidthDist.end(); ++it) {
		if(iWidthIt++<11) {
			ooWidth1 << setw(6) << it->first;
			ooWidth2 << setw(6) << it->second;
		}
		iAvgWidth += it->first * it->second;
	}
	ooWidth1 << endl;
	ooWidth2 << endl;

	int iNodesNum = iAllInternal + iAllLeaf;
	double dAvgDepth = (float)iAvgDepth/(iNodesNum);
	double dAvgBranch = (float)iAvgWidth/(iAllInternal);


	switch(iType) {
		case 0:	//user
			os << "  Tree statistics: " << setfill('-') << right << setw(59) << "-" << endl;
			os << "    Numer of nodes: " << iNodesNum << endl;
			os << "      Internal:     " << iAllInternal << endl;
			os << "      Leaf:         " << iAllLeaf << endl;
			os << "    Average depth:  " << setprecision(2) << fixed << dAvgDepth << endl;
			os << "    Avg. branching: " << setprecision(2) << fixed << dAvgBranch << endl;
			os << ooDepth.str();
			os << ooWidth1.str();
			os << ooWidth2.str();
			os << "  " << setfill('-') << setw(76) << ""  << endl;
			break;
		case 1: //title lines
			os << "#" << setw(7)  << setfill(' ') << right << "Nodes" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Leaves" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Inter" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "AvgDep" << " ";
			os << "|" << setw(7)  << setfill(' ') << right << "Branch" << " ";
			break;
		case 2: //data lines
			os << "#" << setw(7)  << setfill(' ') << right << iNodesNum << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iAllLeaf << " ";
			os << "|" << setw(7)  << setfill(' ') << right << iAllInternal << " ";
			os << "|" << setw(7)  << setfill(' ') << right << fixed << setprecision(2) << dAvgDepth << " ";
			os << "|" << setw(7)  << setfill(' ') << right << fixed << setprecision(2) << dAvgBranch << " ";
			break;
	}	
}

//-------------------------------------------------------------------------------------------
//returns tree statistic parameters
void RdrTree::TreeStatistics(mapii &mapDepthIntrDist, mapii &mapDepthLeafDist, mapii &mapWidthDist, int iDepth) const{
	int iWidth = (deqSubNode == NULL) ? 0 : deqSubNode->size();

	mapii *mapDepthDist = (iWidth==0) ? &mapDepthLeafDist : &mapDepthIntrDist;

	mapWidthDist[iWidth]++;
	(*mapDepthDist)[iDepth]++;

	if(iWidth!=0)
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
			(*it)->TreeStatistics(mapDepthIntrDist, mapDepthLeafDist, mapWidthDist, iDepth+1);
}

//-------------------------------------------------------------------------------------------
//lematizes word according to this tree and subtree
char *RdrTree::Lemmatize(const char *acWord) const{
	RdrTree *rtSub = FindSubNode(acWord);

	if (rtSub!=NULL) 
		return rtSub->Lemmatize(acWord);
	else
		if (rrRule!=NULL)
			return rrRule->Lemmatize(acWord);
		else
			return Str::copy((char *) acWord);
}
//-------------------------------------------------------------------------------------------
//finds subnode that better describes this word than current node
RdrTree *RdrTree::FindSubNode(const char *acWord) const{
	if (deqSubNode == NULL) return NULL;

	int iWordLen = strlen(acWord);

	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it) {
		int iWordStart = iWordLen - (*it)->iEndingLen;
		//if ending of the word maches current subnode
		if (strcmp(&acWord[iWordStart], (char*)(*it)->acEnding)==0)
			//check in case when subnode is flaged entireWord, then we need to chech equal lengths
			if (!(*it)->bEntireWord || (*it)->iEndingLen == iWordLen)
				return (*it);
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------
//adds all rules from this subtree to compact rules representation table
void RdrTree::CompactRules(mapcr *mapRules) {

	//if first call of this function generate carier object
	bool bFirst = false;
	if (mapRules == NULL) {
		mapRules = new mapcr();
		bFirst = true;
	}
	const char *acEwAppend = bEntireWord?"ew:":"nw:";

	//get rule id (append sth if entireword)
	byte *acRuleId = (byte*) rrRule->UniqueStringId((char*) acEwAppend);
	mapcr::iterator itRule = mapRules->find(acRuleId);
	delete[] acRuleId;

	//if not insert new rule and it's key into map
	if (itRule == mapRules->end()) {
		byte *acRuleId = (byte*) rrRule->UniqueStringId((char*) acEwAppend);
		mapRules->insert(mapcr::value_type(acRuleId, rrRule));
	}
	else if (itRule->second != rrRule){
		if (--(rrRule->iObjectCounter) == 0) delete rrRule;
		rrRule = itRule->second;
		rrRule->iObjectCounter++;
	}

	//if there are some siblibgs go to them
	if (deqSubNode != NULL)
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
			(*it)->CompactRules(mapRules);

	//delete maprules
	if (bFirst) {
		for(mapcr::iterator it = mapRules->begin(); it != mapRules->end(); ++it)
			delete[] it->first;
		delete mapRules;
	}
}

//-------------------------------------------------------------------------------------------
//sorts subnodes alphabetically
void RdrTree::Sort() {
	if (deqSubNode==NULL || deqSubNode->size()==0) return;
	sort(deqSubNode->begin(), deqSubNode->end(), LowerTree());

	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
		(*it)->Sort();
}

//-------------------------------------------------------------------------------------------
//repairs tree so it's more like decisin tree than rdr but maintains rdr structure
void RdrTree::PatchTree() {
	CompactRules();
	int iNum = 0;
	EnumerateTree(iNum);
	RemoveMultiCover();
	OneCharDecision();
	SetNewSufxLen(0);
}

//-------------------------------------------------------------------------------------------
//sets order numbers as they shoud be (depth first manner)
void RdrTree::EnumerateTree(int &iNum) {
	iOrder = ++iNum;
	if (deqSubNode==NULL || deqSubNode->size()==0) return;

	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
		(*it)->EnumerateTree(iNum);
}

//-------------------------------------------------------------------------------------------
//removes cases where one case can be covered with two or more exceptions
void RdrTree::RemoveMultiCover() {
	if (deqSubNode==NULL || deqSubNode->size()==0) return;

	//order subnodes
	sort(deqSubNode->begin(), deqSubNode->end(), LowerTree());

	//for the sake of efficiency we create new deque and dispose the old
	deqrt *deqOld = deqSubNode;
	deqSubNode = new deqrt;

	//need two iterators one for this and one for next that can move independly
	deqrt::iterator itThis = deqOld->begin();
	deqrt::iterator itNext = deqOld->begin();
	RdrTree *rtThis = *itThis;
	RdrTree *rtNext = *itNext;

	while(itThis != deqOld->end()) {
		//if next is after last subnode then exit
		if (itNext == deqOld->end() ) {
			//if this is valid node just add it to new deque
			if (itThis != deqOld->end()) deqSubNode->push_back(*(itThis));
			break; 
		}

		//else we need to compare this and next node;
		rtThis = *itThis;
		rtNext = *itNext;

		int iEqLen = Str::eqEndLen((char*) rtThis->acEnding, (char*) rtNext->acEnding);
		int iThisEndingLen = rtThis->iEndingLen;
		int iNextEndingLen = rtNext->iEndingLen;
		
		//take into account that sufixes flaged entireword behave like with additional letter
		if (iEqLen == iThisEndingLen && iEqLen == iNextEndingLen && rtThis->bEntireWord && rtNext->bEntireWord)	iEqLen++;
		if (rtThis->bEntireWord) iThisEndingLen++;
		if (rtNext->bEntireWord) iNextEndingLen++;

		//nodes covering same cases (next node is equal or longer (but equal) than this node
		//one node should be eliminated or subnoded
		if(iThisEndingLen == iEqLen) {
			bool bRightOrder = rtThis->iOrder <= rtNext->iOrder; //discard or subnode one of nodes
			bool bSameNode = iThisEndingLen == iNextEndingLen; //don't subnode, just discard one

			if (bRightOrder) {//discard next
				//move next
				itNext++; 
			}
			else
				if (bSameNode) { //discard this
					//move both
					itThis = itNext;
					itNext++;
				} 
				else { //subnode next
					if (rtThis->deqSubNode == NULL) rtThis->deqSubNode = new deqrt();
					rtThis->deqSubNode->push_back(rtNext);
					//move next
					itNext++;
				}
		}
		//else if these two nodes dont cover same cases just add this and go on
		else {
			deqSubNode->push_back(rtThis);
			//move both
			itThis = itNext;
			itNext++;
		}
	}

	delete deqOld;


	//do it on subnodes
	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
		(*it)->RemoveMultiCover();

}
//-------------------------------------------------------------------------------------------
//converts tree in such way that in each node we can decide witch subnode to folow just by 
//looking at one (next) letter of suffix
void RdrTree::OneCharDecision() {
	if (deqSubNode==NULL || deqSubNode->size()==0) return;

	if (deqSubNode->size()>1) {
		//order subnodes
		sort(deqSubNode->begin(), deqSubNode->end(), LowerTree());

		//for the sake of efficiency we create new deque and dispose the old
		deqrt *deqOld = deqSubNode;
		deqSubNode = new deqrt;

		//need two iterators one for this and one for next that can move independly
		deqrt::iterator itThis = deqOld->begin();
		deqrt::iterator itNext = deqOld->begin();
		itNext++;
		RdrTree *rtThis = *itThis;
		RdrTree *rtNext = *itNext;
		RdrTree *rtNew = NULL;
		int iEqLen=0, iExLenOld;

		while(itThis != deqOld->end()) {
			//if next is after last subnode then exit
			if (itNext == deqOld->end() ) {
				//if this is valid node just...
				if (itThis != deqOld->end()) {
					rtThis = *itThis;
					//close current node
					if (rtNew != NULL) {
						byte *acNewEnding = new byte[iEqLen+1];
						strcpy((char*)acNewEnding, (char*) &(rtThis->acEnding)[rtThis->iEndingLen - iEqLen]);
						rtNew->SetEnding(acNewEnding);

						rtNew->rrRule = this->rrRule;
						rtNew->rrRule->iObjectCounter++;
						acId = this->acId;

						this->deqSubNode->push_back(rtNew);
						rtNew = NULL;
					} 
					//add this to this subnodes list 
					else {
						this->deqSubNode->push_back(rtThis);
					}
				}
				itThis = itNext;
				break; 
			}

			//else we need to compare this and next node;
			rtThis = *itThis;
			rtNext = *itNext;

			iExLenOld = iEqLen; 
			iEqLen = Str::eqEndLen((char*) rtThis->acEnding, (char*) rtNext->acEnding);

			//if equal length is longer than parents suffix then it's time for new node
			if(iEqLen>iEndingLen) {
				//create new subtree
				if (rtNew == NULL) {
					rtNew = new RdrTree();
					rtNew->deqSubNode = new deqrt();
					rtNew->deqSubNode->push_back(rtThis);
					rtNew->deqSubNode->push_back(rtNext);
				}
				//subtree already created
				else {
					rtNew->deqSubNode->push_back(rtNext);
				}
			}
			else {
				//close current subnode and add it to this subnodes list
				if (rtNew != NULL) {
					iEqLen = iExLenOld;
					byte *acNewEnding = new byte[iEqLen+1];
					strcpy((char*)acNewEnding, (char*) &(rtThis->acEnding)[rtThis->iEndingLen - iEqLen]);
					rtNew->SetEnding(acNewEnding);

					rtNew->rrRule = this->rrRule;
					rtNew->rrRule->iObjectCounter++;
					acId = this->acId;

					this->deqSubNode->push_back(rtNew);
					rtNew = NULL;
				} 
				//add this to this subnodes list 
				else {
					this->deqSubNode->push_back(rtThis);
				}
				//move on (this);
				itThis = itNext;
			}

			//move on (next);
			itNext++;
		}

		delete deqOld;
	}

	//do it on subnodes
	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
		(*it)->OneCharDecision();

}


//-------------------------------------------------------------------------------------------
//sets new ending length acording to parent
void RdrTree::SetNewSufxLen(int iParentEnding) {
	iNewSufxLen = iEndingLen - iParentEnding - 1;
	if (iNewSufxLen<0) iNewSufxLen = 0;

	if (deqSubNode==NULL || deqSubNode->size()==0) return;

	for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
		(*it)->SetNewSufxLen(iEndingLen);
}

//-------------------------------------------------------------------------------------------
//returns length minimal hash table that splits this characters
int RdrTree::GetMinHash(){
	if (deqSubNode==NULL || deqSubNode->size()==0) return 0;

	int iHashTbl[256];
	int iMinHash;
	for(iMinHash = deqSubNode->size(); iMinHash<256; iMinHash++){
		
		for(int j=0; j<iMinHash; j++) iHashTbl[j] = 0;

		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it) {
			byte c = 0;
			if ((*it)->iEndingLen > iEndingLen)
				c = (*it)->acEnding[(*it)->iEndingLen - iEndingLen - 1];

			iHashTbl[c%iMinHash]++;
		}

		int iError = 0;	
		for(int i=0; i<iMinHash; i++) if (iHashTbl[i] > 1) iError++;
		if (iError == 0) break;
	}
	
	//cout << "deqSubNode->size() = " << deqSubNode->size() << " iMinHash = " << iMinHash << endl;
	return iMinHash;
}

//-------------------------------------------------------------------------------------------
//returns length of this node structure in bytes
int RdrTree::GetByteLength() {
	bool bNoSub = deqSubNode==NULL || deqSubNode->size()==0;
	bool bOneLett = iNewSufxLen == 0;

	if (bNoSub)
		if (bOneLett)	// [1']
			return 0;
		else			// [1]
			return FlagLen + AddrLen + LenSpecLen + iNewSufxLen;
	else
		if (bOneLett)	// [2]
			return FlagLen + AddrLen + ModLen + (AddrLen+CharLen) * GetMinHash();
		else			// [3]
			return FlagLen + AddrLen + LenSpecLen + iNewSufxLen + ModLen + (AddrLen+CharLen) * GetMinHash();
}
//-------------------------------------------------------------------------------------------
//sets address of this subnode in byte array
void RdrTree::SetAddress(int &iAddr, bool bReset) {
	iAddress = iAddr;
	if (!bReset) iAddr += GetByteLength();

	//set address of this rule if not set
	 rrRule->SetAddress(iAddr, bReset);

	//set adressess of all siblings
	if (deqSubNode!=NULL && deqSubNode->size()!=0)
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
			(*it)->SetAddress(iAddr, bReset);
}
//-------------------------------------------------------------------------------------------
//creates and returns very optimized (speed and size) lematizer
RdrLemmatizer *RdrTree::CreateLemmatizer() {
	//optimizes and prepares tree
	PatchTree();

	//reset adresses
	int iDataLen = -1;
	SetAddress(iDataLen,true);

	//sets addresses and cumulative dala length round to 8 because of import/export
	iDataLen = DataStart;
	SetAddress(iDataLen);
	iDataLen += (RoundDataLen - (iDataLen % RoundDataLen)) % RoundDataLen;
	
	//"format" the memory for the data
	byte *abData = new byte[iDataLen];
	memset(abData, NULL, iDataLen);

	//sets data code for tree
	SetByteCode(abData);

	//return lemmatizer
	return new RdrLemmatizer(abData, iDataLen);
}
//-------------------------------------------------------------------------------------------
//sets just the code of this node on the addres that was predefined
void RdrTree::SetByteCodeThis(byte *abData) {
	int iAddr = iAddress;

	bool bNoSub = deqSubNode==NULL || deqSubNode->size()==0;
	bool bOneLett = iNewSufxLen == 0;

	if (bNoSub) {
		if (bOneLett) {
			//not error to come here, but nothing to be done here
			//cerr << "Internal error: Type '1 node should not be created!" << endl;
			return;
		}

		//flag
		abData[iAddr] = (bEntireWord ? TypeLeafACEw : TypeLeafAC);
		iAddr += FlagLen;
		
		//address of rule
		memcpy(&abData[iAddr], &rrRule->iAddress, AddrLen);
		iAddr += AddrLen;
		
		//length of addtional characters
		abData[iAddr] = (byte) iNewSufxLen;
		iAddr += LenSpecLen;
		
		//additional characters to check
		memcpy(&abData[iAddr], acEnding, iNewSufxLen);
		iAddr += iNewSufxLen;
		
		return;
	}
	else {
		//flag
		abData[iAddr] = (byte) (bOneLett ? TypeIntr : TypeIntrAC);
		iAddr += FlagLen;

		//address of rule
		memcpy(&abData[iAddr], &rrRule->iAddress, AddrLen);
		iAddr += AddrLen;

		//additional characters if appliable
		if (!bOneLett) {
			//length of addtional characters
			abData[iAddr] = iNewSufxLen;
			iAddr += LenSpecLen;
			
			//additional characters to check
			memcpy(&abData[iAddr], acEnding, iNewSufxLen);
			iAddr += iNewSufxLen;
		}

		//hashtable of exception
		int iMinHash = GetMinHash();
		abData[iAddr] = (byte) iMinHash;

		iAddr += ModLen;
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it) {
			RdrTree *rtSub = (*it);
			byte bChar = rtSub->acEnding[rtSub->iNewSufxLen];
			if (rtSub->iEndingLen == iEndingLen && rtSub->bEntireWord ) bChar = 0;
			dword iOffset = iAddr + (bChar % iMinHash) * (AddrLen + CharLen);

			if (abData[iOffset] != NULL) {
				char *acNodeStr = NodeToString(NICE_MIN);
				char *acSubNodeStr = rtSub->NodeToString(NICE_MIN);

				cerr << "Internal error: Hash table entry already filled when trying to add new!" << endl;
				cerr << "          node  " << acNodeStr << " " << iNewSufxLen <<  endl;
				cerr << " added subnode  " << "char = " << bChar << "[" << (int)bChar << "] "
					 << acSubNodeStr << " " << rtSub->iNewSufxLen << endl;
				cerr << " exist.subnode  char = " << abData[iOffset] << "[" << (int)abData[iOffset] << "]"
					 << " hash table len = " << iMinHash << endl;

				delete[] acNodeStr;
				delete[] acSubNodeStr;

				return;
			}

			dword iSumAddr = rtSub->iAddress;
			//if it's leaf and dont have additional characters then just add it's rule
			if((rtSub->deqSubNode==NULL || rtSub->deqSubNode->size()==0) && rtSub->iNewSufxLen == 0)
				iSumAddr = rtSub->rrRule->iAddress;

			//set char and follow address;
			abData[iOffset]=bChar;
			memcpy(&abData[iOffset+CharLen], &iSumAddr, AddrLen);
		}

		//increase address just to check it
		iAddr += iMinHash * (AddrLen + CharLen);
	}

	//check if size is ok
	if (iAddr != iAddress + GetByteLength()) {
		char *acNodeStr = NodeToString(NICE_MIN);

		cerr << "Internal error: Declaration size and definition size of a tree node diferate!" << endl;
		cerr << "          node: " << acNodeStr << endl;

		delete[] acNodeStr;
	}
}


//-------------------------------------------------------------------------------------------
//sets code of this node and all the subnodes
void RdrTree::SetByteCode(byte *abData) {
	//write self code & code of current rule (some rules may be overwriten many times but this is OK)
	SetByteCodeThis(abData);
	rrRule->SetByteCodeThis(abData, bEntireWord);

	//go to subnodes
	if (deqSubNode!=NULL && deqSubNode->size()!=0)
		for(deqrt::iterator it = deqSubNode->begin(); it != deqSubNode->end(); ++it)
			(*it)->SetByteCode(abData);
}

//-------------------------------------------------------------------------------------------
//return RdrTree that is generated from given lemamtizer
RdrTree *RdrTree::GenFromLemmatizer(const RdrLemmatizer &lem, dword iNodeAddr, char *acParSufx, char cNewChar) {
	//data byte stream
	byte *abData = lem.abData;
	//current address pointer
	dword iAddr = iNodeAddr;
	
	//data that we must collect in order to create RdrTree object
	byte iType = 0;				//.. 1 ..
	dword iRuleAddress = NULL;	//.. 2 ..
	
	byte iSufxLen = 0;			//.. 3 ..
	char *acSufx = NULL;		//.. 3 ..

	byte iSubsNum = 0;			//.. 4 ..
	dword *iSubAddr = NULL;		//.. 4 ..
	byte *bSubChar = NULL;		//.. 4 ..
	
	byte iLeftLen = 0;			//.. 5 ..
	byte iRightLen = 0;			//.. 5 ..
	char *acLeft = NULL;		//.. 5 ..
	char *acRight = NULL;		//.. 5 ..

	//...................... 1 ......................
	//read node type
	GETBYTEMOVE(, iType, FlagLen);
	
	//...................... 2 ......................
	//get rule address
	if((iType & ~BitEntireWr)==TypeRule)
		//rule address is actualy this address
		iRuleAddress = iAddr;
	else {
		GETDWORDMOVE(, iRuleAddress, AddrLen);
		iRuleAddress += FlagLen;
	}

	//...................... 3 ......................
	//eat up added characters if they exist in this node type
	byte iNewSufxLen = 0;
	byte iNewCharLen = cNewChar == NULL?0:1;
	char* acNewSuffix = NULL;
	if ((iType & BitAddChar) == BitAddChar) {
		GETBYTEMOVE(, iNewSufxLen, LenSpecLen);
		GETSTRINGMOVE(, acNewSuffix, iNewSufxLen);
	} 

	//construct suffix of this node
	if (iNodeAddr == DataStart) {
		iSufxLen = 0;
		acSufx = "";
	}
	else {
		iSufxLen = iNewCharLen + iNewSufxLen + strlen(acParSufx);
		acSufx = new char[iSufxLen + 1];

		//append added chars (if they exist)
		if (acNewSuffix != NULL) 
			strncpy(&acSufx[0],acNewSuffix,iNewSufxLen);
		//append new char (if it exists)
		if (cNewChar != NULL) 
			acSufx[0 + iNewSufxLen] = cNewChar;
		//append old suffix
		strcpy(&acSufx[iNewCharLen + iNewSufxLen],acParSufx);
	} 

	//...................... 4 ......................
	//construct hashtable if it exist in this node type
	if ((iType & BitInternal) == BitInternal) {
		GETBYTEMOVE(, iSubsNum, ModLen);
		iSubAddr = new dword[iSubsNum];
		bSubChar = new byte[iSubsNum];

		for (int i = 0; i< iSubsNum; i++) {
			GETBYTEMOVE(, bSubChar[i], CharLen);
			GETDWORDMOVE(, iSubAddr[i], AddrLen);
		}
	}	

	//...................... 5 ......................
	//Get data for rule - acLeft=From & acRight=TO
	iAddr = iRuleAddress;
	GETBYTEMOVE(, iLeftLen, LenSpecLen);
	GETBYTEMOVE(, iRightLen, LenSpecLen);
	acLeft = Str::copy(&acSufx[iSufxLen - iLeftLen]);
	GETSTRINGMOVE(, acRight, iRightLen);

	//....................... actual rdrtree creation ...........................
	//create current node we must provide acSufx, acLeft, acRight, iSubsNum, iSubAddr[i]-table of adresses, bSubChar[i]-table of chars
	RdrTree *rt = new RdrTree();
	
	//fill it with data
	rt->SetEnding((byte *) acSufx);
	rt->rrRule = new RdrRule(iLeftLen, (byte *) acRight);
	rt->bEntireWord = ((iType & BitEntireWr) == BitEntireWr);	

	//create subnodes
	if (iSubsNum>0) {
		rt->deqSubNode = new RdrTree::deqrt();
		for (int i = 0; i<iSubsNum; i++)
			if (iSubAddr[i]!=NULL) {
				RdrTree *rdrSubNode = RdrTree::GenFromLemmatizer(lem, iSubAddr[i], acSufx, bSubChar[i]);
				rt->deqSubNode->push_back(rdrSubNode);
			}
	}

	//correct the data on the whole tree structure
	if (iNodeAddr == DataStart) 
		rt->SetNewSufxLen(0);

	//sort subnodes before exiting
	if (rt->deqSubNode != NULL)
		sort(rt->deqSubNode->begin(), rt->deqSubNode->end(), LowerTree());

	//return tree
	return rt;

}

