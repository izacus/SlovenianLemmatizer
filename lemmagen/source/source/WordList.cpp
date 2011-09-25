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
#include "../header/WordList.h"

//-------------------------------------------------------------------------------------------
//constructors
WordList::WordList() {
	Initialize();
}
WordList::WordList(const char *acFileName, const char cAddPrefix, int iVerbose) {
	Initialize();
	Import(acFileName, cAddPrefix, iVerbose);
}
WordList::~WordList() {
	Clear();
}
//-------------------------------------------------------------------------------------------
//copy constructors and assigment operator
WordList::WordList(const WordList &wl) {
	Initialize();
	Copy(wl);
}
WordList::WordList(const WordList &wl, int iSet, bool bSetElem, bool bDummy) {
	Initialize();
	SubsetCopy(wl, iSet, bSetElem);
}
WordList &WordList::operator=(const WordList &wl) {
	if(&wl!=this) {
		Clear();
		Copy(wl);
	}
	return *this;
}
//-------------------------------------------------------------------------------------------
//initialization
void WordList::Initialize() {
	aoWord = NULL;
	iWordNum = 0;

	iSetNum = 0;
	aiSetNum = NULL;
	aaiSetMap = NULL;

	aoRule = NULL;
	iRuleNum = 0;
}

//-------------------------------------------------------------------------------------------
//deleting functions
void WordList::ClearSets() {
	if (aaiSetMap != NULL)
		for(int i = 0; i<iSetNum; i++)
			delete[] aaiSetMap[i];
	delete[] aaiSetMap;
	delete[] aiSetNum;

	aaiSetMap = NULL;
	aiSetNum = NULL;
}

void WordList::Clear() {
	ClearSets();
	
	delete[] aoWord;
	delete[] aoRule;

	aoWord = NULL;
	aoRule = NULL;

	iWordNum = 0;
	iRuleNum = 0;
	iSetNum= 0;
}
//-------------------------------------------------------------------------------------------
//copy function
void WordList::Copy(const WordList &wl) {
	iWordNum = wl.iWordNum;
	iSetNum = wl.iSetNum;
	iRuleNum = wl.iRuleNum;

	aoWord = new Word[iWordNum];
	for(int i=0; i<iWordNum; i++) aoWord[i] = wl.aoWord[i];

	if(iSetNum>0) {
		aiSetNum = new int[iSetNum];
		for(int i=0; i<iSetNum; i++) aiSetNum[i] = wl.aiSetNum[i];

		aaiSetMap = new int*[iSetNum];
		for(int iSet=0; iSet<iSetNum; iSet++) {
			
			aaiSetMap[iSet] = new int[aiSetNum[iSet]];

			for(int i=0; i<aiSetNum[iSet]; i++)
				aaiSetMap[iSet][i] = wl.aaiSetMap[iSet][i];
		}
	}

	aoRule = new Rule[iRuleNum];
	for(int i=0; i<iRuleNum; i++) aoRule[i] = wl.aoRule[i];
}
//copy function
void WordList::SubsetCopy(const WordList &wl, int iSet, bool bSetElem) {
	if (bSetElem) 
		iWordNum = wl.aiSetNum[iSet];
	else
		iWordNum = wl.iWordNum - wl.aiSetNum[iSet];
	
	iSetNum = 1;
	iRuleNum = wl.iRuleNum;

	aoWord = new Word[iWordNum];
	{	int iNew = 0;
		for(int i=0; i<wl.iWordNum; i++) 
			if ((*(wl.aoWord[i].iSet) == iSet) == bSetElem)
				aoWord[iNew++] = wl.aoWord[i];
		
	}

	aiSetNum = new int[1];
	aiSetNum[0] = iWordNum;

	aaiSetMap = new int*[1];
	aaiSetMap[0] = new int[iWordNum];
	for(int i=0; i<iWordNum; i++) aaiSetMap[0][i] = i;

	aoRule = new Rule[iRuleNum];
	for(int i=0; i<iRuleNum; i++) aoRule[i] = wl.aoRule[i];

}
//-------------------------------------------------------------------------------------------
//Import from file
void WordList::Import(const char *acFileName, const char cAddPrefix, int iVerbose) {
	int iWordNumRealOld = iWordNum;

	const int	MAX_LEN = 10000;
	int			iRealLen = 0;
	int			iByteCount = 0;
	char *acBuf = new char[MAX_LEN];
	ifstream	ifs;

	typedef vector<char *> vecline;
	vecline vecLine;

	Timing timePart, timeAll;
	
	ifs.open(acFileName);

	if (iVerbose) {
		cout << "  Proccessing file '" << acFileName << "'." << endl;
		timePart.Print(cout, "    Time needed to open ");
	}

	// read lines from file

	while (!ifs.eof()) {
		ifs.getline(acBuf, MAX_LEN);

		iRealLen = strlen(acBuf)+1;
		if (iRealLen > 2) {
			vecLine.push_back(Str::copy(acBuf));
			iWordNum++;
			iByteCount += iRealLen-1;
		}
	}
	ifs.close();
	delete[] acBuf;

	if (iVerbose) {
		cout << "    Imported " 
			<< iWordNum-iWordNumRealOld << " lines, " 
			<< iByteCount << " bytes = " 
			<< setprecision(2) << fixed << (float)iByteCount/1024/1024 << " MB. " 
			<< "Current word count " << iWordNum <<  endl;
		timePart.Print(cout, "    Time needed for read ");
	}

	//Word parsing & insertion in array of words
	Word *aoWordNew = new Word[iWordNum];
	int iPos = iWordNumRealOld;
	for( vecline::iterator itLine = vecLine.begin(); itLine != vecLine.end(); itLine++, iPos++) {
		aoWordNew[iPos].Initialize(*itLine, cAddPrefix);
		*(aoWordNew[iPos].iSet) = iSetNum;
		delete[] *itLine;
	}
	//copy old words that were here from previous import
	for(int iWord=0; iWord<iWordNumRealOld; iWord++) {
		aoWordNew[iWord]=aoWord[iWord];
	}

	delete[] aoWord;
	aoWord = aoWordNew;
	//iSetNum++; RecreateSets couple of lines down does that


	if (iVerbose) timePart.Print(cout, "    Insertion to array of words ");

	GenerateRules(max(iVerbose-1,0));

	RecreateSets(max(iVerbose-1,0));

	if (iVerbose) timeAll.Print(cout, "    Import together ");
}

//-------------------------------------------------------------------------------------------
//generate rules
void WordList::GenerateRules(int iVerbose) {
	delete[] aoRule;
	iRuleNum = 0;

	Timing time;

	if (iVerbose) cout << "  Starting rules generation..." << endl;

	typedef map<const char*, int, Str::LowerCharStr> mapci;
	typedef map<int, int> mapii;
	mapii mRulesCounts;
	mapci mRulesId;
	int iRuleId = 0;

	char *acRule;
	for (int iPos = 0; iPos < iWordNum; iPos++) {
		acRule = aoWord[iPos].GetRule();
		mapci::iterator iter = mRulesId.find(acRule);

		if (iter == mRulesId.end())	{
			mRulesId.insert(mapci::value_type(acRule, iRuleId));
			aoWord[iPos].iRule = iRuleId;
			mRulesCounts.insert(mapii::value_type(iRuleId,1));
			iRuleId++;
		} else {
			aoWord[iPos].iRule = iter->second;
			++mRulesCounts[iter->second];
			delete[] acRule;
		}
	}

	iRuleNum = iRuleId;
	aoRule = new Rule[iRuleNum];
	for(mapci::iterator iter = mRulesId.begin(); iter!= mRulesId.end(); iter++){
		aoRule[iter->second].Initialize((char*)iter->first);
		aoRule[iter->second].iSupportCount = mRulesCounts[iter->second];
	}

	if (iVerbose) {
		cout << "  Generated " << iRuleNum << " rules." << endl;
		time.Print(cout, "  Time needed for rule generation ");
	}
}

//-------------------------------------------------------------------------------------------
//export to file
void WordList::Export(const char *acFileName, const char cRemPrefix, int iVerbose, bool bExtended) {
	int iLineCount = 0;
	int iByteCount = 0;
	Timing time;

	ofstream ofs;
	ofs.open(acFileName);

	if (iVerbose)
		time.Print(cout, "Time needed for open file ", ", export started...");

	Print(ofs, iLineCount, iByteCount, cRemPrefix, bExtended);
	ofs.close();

	if (iVerbose) {
		cout << "  Exported " 
			<< iLineCount << " lines, " 
			<< iByteCount << " bytes = " 
			<< setprecision(2) << fixed << (float)iByteCount/1024/1024 << " MB." << endl;
		time.Print(cout, "  Time needed for export ");
	}
}

//-------------------------------------------------------------------------------------------
//output functions
const char *WordList::ToString(const char cRemPrefix, bool bExtended) {
	ostringstream os;
	int iLine, iByte;

	Print(os, iLine, iByte, cRemPrefix, bExtended);

	string s = os.str();
	return strcpy(new char[s.length()+1],s.c_str());
}

void WordList::Print(ostream &os, int &iLineCount, int &iByteCount, const char cRemPrefix, bool bExtended) {
	//print words to out stream
	for (int iPos = 0; iPos < iWordNum; iPos++) {
		char *acLine = aoWord[iPos].ToString(bExtended, cRemPrefix);
		os << acLine << "\n";
		iLineCount ++;
		iByteCount += strlen(acLine) + 1;
	}
	os.flush();
}
//-------------------------------------------------------------------------------------------
//get word number of given set
int WordList::GetWordNum(int iSet, bool bSetElem) {
	if (iSet == -1) return iWordNum;
	
	if (bSetElem) return aiSetNum[iSet];
	else return iWordNum - aiSetNum[iSet];
}
//returns rule by it's id
Rule &WordList::GetRule(int iRuleId) const{
	return aoRule[iRuleId];
}

//-------------------------------------------------------------------------------------------
//iterator initialization
WordList::iterator *WordList::begin(int iSet, bool bSetElem) {
	return (new iterator(this,true,iSet,bSetElem));
}
WordList::iterator *WordList::rbegin(int iSet, bool bSetElem) {
	return (new iterator(this,false,iSet,bSetElem));
}

WordList::iterator *WordList::end(int iSet, bool bSetElem) {
	return &++*(new iterator(this,false,iSet,bSetElem));
}
WordList::iterator *WordList::rend(int iSet, bool bSetElem) {
	return &--*(new iterator(this,true,iSet,bSetElem));
}

//-------------------------------------------------------------------------------------------
//only if multiply = 1 we get realy equal sets of words than before
//sort words using given comparator
void WordList::Sort(int iVerbose, Word::LessThan::Type ltType) {
	Timing time;
	
	if (iVerbose) cout << "  Sorting " << iWordNum << " words." << endl;
	
	sort(aoWord, aoWord+iWordNum, Word::LessThan(ltType));

	if (iVerbose) time.Print(cout, "  Words sorted in ");

	if (iSetNum>0)
		RecreateSets(max(iVerbose-1,0));
}

//-------------------------------------------------------------------------------------------
//split words to sets for cross validation
void WordList::SplitCrossValid (int iKFold, bool bDeepSplit, int iVerbose) {
	if(iKFold <=0) return;
	ClearSets();

	Timing time;

	if (iVerbose)
		cout << "  Spliting set to K-fold. K = " << iKFold << ", deep split = " << 
		boolalpha << bDeepSplit << "." << endl;

	//deep split we must sort (no other option) remember sorting
	Word *aoWordNew = aoWord;
	if (bDeepSplit) {
		aoWordNew = new Word[iWordNum];
		for(int i=0; i<iWordNum; i++) aoWordNew[i] = aoWord[i];
		sort(aoWordNew, aoWordNew+iWordNum);
	} 

	iSetNum = iKFold;

	//split on train and test set
	aiSetNum = new int[iKFold];
	for(int i=0; i<iKFold; i++) aiSetNum[i] = 0;

	//assign to each word to what set it belongs to;
	int iRand=0, iLastRand;
	for (int i = 0; i < iWordNum; i++) {
		iLastRand = iRand;
		iRand = Rand::nextInt(0,iKFold);

		if (bDeepSplit && i>0 && aoWordNew[i-1]==aoWordNew[i])
			iRand = iLastRand;

		*(aoWordNew[i].iSet) = iRand;
	}

	//ii deep split clean after yourself
	if (bDeepSplit) delete[] aoWordNew;

	if (iVerbose) time.Print(cout, "  Randomized sets. Time ");

	RecreateSets(max(iVerbose-1,0));
}

//-------------------------------------------------------------------------------------------
//split words to two sets you can specify number of words for one
void WordList::SplitSubset(int iSubsetPerc, bool bDeepSplit, int iVerbose) {
	if(iSubsetPerc <0 || iSubsetPerc>=100) return;
	ClearSets();

	Timing time;


    if (iVerbose)
		cout << "  Spliting set to a 2 subsets: id 1 = " << (iWordNum*iSubsetPerc)/100 << 
		" examples id 2 = " << (iWordNum*(100-iSubsetPerc))/100 << " examples." << endl;

	//deep split we must sort (no other option) remember sorting
	Word *aoWordNew = aoWord;
	if (bDeepSplit) {
		aoWordNew = new Word[iWordNum];
		for(int i=0; i<iWordNum; i++) aoWordNew[i] = aoWord[i];
		sort(aoWordNew, aoWordNew+iWordNum);
	} 

	//split on train and test set
	iSetNum = 2;
	aiSetNum = new int[2];
	aiSetNum[0] = iWordNum - iSubsetPerc;
	aiSetNum[1] = iSubsetPerc;

	//reset all words to default dataset 0
	for (int i = 0; i < iWordNum; i++) *(aoWord[i].iSet) = 0;
	
	//assign to each word to what set it belongs to;
	int iRand=0, iLastRand;
	for (int i = 0; i < iWordNum; i++) {
		iLastRand = iRand;

		if (bDeepSplit && i>0 && aoWordNew[i-1]==aoWordNew[i])
			iRand = iLastRand;
		else
			iRand = Rand::nextInt(0,100);

		*(aoWordNew[i].iSet) = (iRand<iSubsetPerc?0:1);
	}

	//ii deep split clean after yourself
	if (bDeepSplit) delete[] aoWordNew;

	if (iVerbose) time.Print(cout, "  Randomized sets. Time ");

	RecreateSets(max(iVerbose-1,0));
}

//-------------------------------------------------------------------------------------------
//split words to sets for cross validation
//only if multiply = 1 we get realy equal sets of words than before
void WordList::RecreateSets(int iVerbose) {
	ClearSets();
	Timing time;

	//find real set number
	iSetNum = 0;
	for (int iWord=0; iWord<iWordNum; iWord++)
		if (*(aoWord[iWord].iSet)>=iSetNum) iSetNum = *(aoWord[iWord].iSet)+1;

	//split on train and test set
	aiSetNum = new int[iSetNum];
	for(int i=0; i<iSetNum; i++) aiSetNum[i] = 0;

	//counts number of words in each set
	for (int i = 0; i < iWordNum; i++) aiSetNum[*(aoWord[i].iSet)]++;

	if (iVerbose) {
		cout << "  Original set of " << iWordNum << " words splited in " << time << " into next subsets:";
		for(int i=0; i<iSetNum; i++) {
			if (i%10 == 0) cout << endl << "    ";
			cout << setw(6) << right << aiSetNum[i] << (i!=iSetNum-1 ? "," : "") ;
		}
		cout << endl;
	}

	//create mapping sets tables
	aaiSetMap = new int*[iSetNum]; 
	for(int i=0; i<iSetNum; i++) aaiSetMap[i] = new int[aiSetNum[i]];
	for(int i=0; i<iSetNum; i++) aiSetNum[i] = 0;

	//fill mapping sets tables
	for (int iWord=0; iWord<iWordNum; iWord++) {
		int iSet = *(aoWord[iWord].iSet);
		aaiSetMap[iSet][aiSetNum[iSet]++] = iWord;
	};

	if (iVerbose)
		time.Print(cout, "  Filled new tables. Time ");
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//iterators for word list - constructors
WordList::iterator::iterator(WordList *wl, bool bForw, int iSet, bool bSetElem)	{
	this->wl = wl;
	this->iSet = iSet;
	this->bSetElem = bSetElem;

	if(iSet < 0 || iSet >= wl->iSetNum) 
		this->iSet = -1;

	//set max value
	if (this->iSet == -1 || !bSetElem)
		iMax = wl->iWordNum;
	else 
		iMax = wl->aiSetNum[iSet];

	//set word position
	if (bForw) 
		iWordPos = 0;
	else 
		iWordPos = iMax-1;

	//if we are not on the right position, find the right
	if (this->iSet != -1 && !bSetElem) {
		Increment(!bForw);
		Increment(bForw);
	}

}
//-------------------------------------------------------------------------------------------
//Incrementing
void WordList::iterator::Increment(bool bInc) {
	iWordPos += bInc ? 1 : -1;
	if (iSet != -1 && !bSetElem)
		while(	iWordPos >= 0 && 
			iWordPos < iMax &&
			*(wl->aoWord[iWordPos].iSet) == iSet
			)
			iWordPos += bInc ? 1 : -1;
}

//-------------------------------------------------------------------------------------------
//aux functions for retrieving words and postitions
int WordList::iterator::GetWordPos() const {
	if (iWordPos<0 || iWordPos>=iMax) return -1;

	if (this->iSet != -1 && bSetElem)
			return wl->aaiSetMap[iSet][iWordPos];

	return iWordPos;	
}

//-------------------------------------------------------------------------------------------
//returns set of curent word if it exists
int WordList::iterator::GetSet() const{
	if (wl->iSetNum > 0)
		return *(wl->aoWord[GetWordPos()].iSet);
	else
		return -1;
}
//returns ruleid of curent word if it exists
int WordList::iterator::GetRuleId() const{
	return wl->aoWord[GetWordPos()].iRule;
}
//returns rule of curent word if it exists
Rule &WordList::iterator::GetRule() const{
	return wl->aoRule[ wl->aoWord[GetWordPos()].iRule ];
}
//returns number of words in this itterator
int WordList::iterator::GetWordNum() const {
	return wl->GetWordNum(iSet, bSetElem);
}

//-------------------------------------------------------------------------------------------
//increment/ decrement operators
WordList::iterator &WordList::iterator::operator++() {
	Increment();
	return *this;
}

WordList::iterator WordList::iterator::operator++(int dummy) {
	iterator copy(*this);
	Increment();
	return copy;
}
WordList::iterator &WordList::iterator::operator--() {
	Increment(false);
	return *this;
}
WordList::iterator WordList::iterator::operator--(int dummy) {
	iterator copy(*this);
	Increment(false);
	return copy;
}
WordList::iterator WordList::iterator::operator-(int iDec) {
	iterator copy(*this);
	while(iDec!=0) {--copy; --iDec;}
	return copy;
}


//-------------------------------------------------------------------------------------------
//equality operators
bool WordList::iterator::operator==(const iterator &y) const {
	return iWordPos==y.iWordPos;
}
bool WordList::iterator::operator!=(const iterator &y) const {
	return iWordPos!=y.iWordPos;
}
bool WordList::iterator::operator>=(const iterator &y) const {
	return iWordPos>=y.iWordPos;
}

//-------------------------------------------------------------------------------------------
//pointer to word functions
Word &WordList::iterator::operator*() const {
	return wl->aoWord[GetWordPos()];
}
Word *WordList::iterator::operator->() {
	return &(wl->aoWord[GetWordPos()]);
}


