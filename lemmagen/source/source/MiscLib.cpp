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
#include "../header/MiscLib.h"


//-------------------------------------------------------------------------------------------
//returns length of equal prefix of s1 & s2
int Str::eqPrefixLen(const char *s1, const char *s2) {
	size_t pos = 0;
	while(s1[pos] == s2[pos] && s2[pos] != NULL) pos++;
	return pos;
}

//-------------------------------------------------------------------------------------------
//returns length of equal prefix of s1end & s2end that is >= iConfirmedLen
int Str::eqEndLen(const char *s1, const char *s2, int iConfirmedLen)  {
	int iPos1 = strlen((char*)s1)-1-iConfirmedLen;
	int iPos2 = strlen((char*)s2)-1-iConfirmedLen;
	int iLen = iConfirmedLen;

	while (iPos1 >= 0 && iPos2 >= 0 && s1[iPos1]==s2[iPos2]) {
		iPos1--; 
		iPos2--; 
		iLen++;
	}

	return iLen;
}

//-------------------------------------------------------------------------------------------
//returns length of equal prefix of s1end & s2end that is >= iConfirmedLen
int Str::eqEndLenFast(const char *s1end, const char *s2end, const int iConfirmedLen, const int iMaxLen) {
	//!!! UNSAFE if length of s1 or s2 == 0; and if word is not terminated with NULL at start
	int iLen=-iConfirmedLen-1;
	int iMinusMaxLen = -iMaxLen;
	while (iLen>=iMinusMaxLen && s1end[iLen]==s2end[iLen]) iLen--;
	return -iLen-1;
}

//-------------------------------------------------------------------------------------------
//compares unsigned words
int Str::bytecmp(const byte *s1, const byte *s2)  {
	int i=0;
	while (s1[i]==s2[i] && s2[i]!=NULL) i++;
	return s1[i]-s2[i];
}

//-------------------------------------------------------------------------------------------
//compares unsigned words
//returns <0 if s1 is smaler than s2, 0 if equal, >0 otherwise
int Str::bytecmpBack(const byte *s1, const byte *s2)  {
	int iPos1 = strlen((char*)s1);
	int iPos2 = strlen((char*)s2);

	while (true) {
		if (s1[iPos1]!=s2[iPos2]) return s1[iPos1] - s2[iPos2];
		if (iPos1 == 0 || iPos2 == 0 ) return iPos1 - iPos2;
		iPos1--;
		iPos2--;
	}
}

//-------------------------------------------------------------------------------------------
//returns string acFrom + acArrow + acTo
//acFrom and acTo are croped of equal prefix
char *Str::cropJoin(const char *acFrom, const char *acArrow, const char *acTo, const int iEqLen) {
	int iEqLenR = iEqLen>=0 ? iEqLen : eqPrefixLen(acFrom, acTo);
	size_t len = strlen(acArrow) + strlen(&acFrom[iEqLenR]) + strlen(&acTo[iEqLenR]);

	char *acRule = new char[len + 1];
	acRule[0] = NULL;

	strcat(acRule, &acFrom[iEqLenR]);
	strcat(acRule, acArrow);
	strcat(acRule, &acTo[iEqLenR]);

	return acRule;
}

//-------------------------------------------------------------------------------------------
//returns lower representation of character (modified for slovene language)
byte Str::lower(byte ch){
	if (ch>0) return tolower(ch);
	switch (ch) {
		case 200: return 232;	//#200 = È, #232 = è
		case 138: return 154;	//#138 = Š, #154 = š
		case 142: return 158;	//#142 = Ž, #158 = ž
		case 198: return 230;	//#198 = Æ, #230 = æ
		case 208: return 240;	//#208 = Ð, #240 = ð
		default: return tolower(ch);
	}
}

//-------------------------------------------------------------------------------------------
//returns lower representation of character sequance (modified for slovene language)
void Str::lower(char *s){
	while(*s!=NULL)
		*s = lower(*s++);
}

//-------------------------------------------------------------------------------------------
//returns copy of string
byte *Str::copy(byte *s){
	return (byte*) strcpy(new char[strlen((char*)s)+1],(char*)s);
}

//-------------------------------------------------------------------------------------------
//returns copy of string
char *Str::copy(char *s){
	return strcpy(new char[strlen(s)+1],s);
}
//-------------------------------------------------------------------------------------------
//returns copy of string and appends char if != NULL
char *Str::copyAppend(const char cPrefix, char *s){
	if(cPrefix==NULL) {
		return strcpy(new char[strlen(s)+1],s);
	} else {
		char *acReturn = new char[strlen(s)+2];
		acReturn[0]=cPrefix;
		strcpy(&acReturn[1],s);
		return acReturn;
	}
}

//-------------------------------------------------------------------------------------------
//returns copy of string s1 concatenated with string s2
char *Str::copyConcat(const char *s1, const char *s2){
	int iS1Len = strlen(s1);

	char *acReturn = new char[iS1Len + strlen(s2)+1];

	strcpy(&acReturn[0], s1);
	strcpy(&acReturn[iS1Len], s2);

	return acReturn;
}

//-------------------------------------------------------------------------------------------
//converts string with escape character '\' to normal string without such characters
//repairs only 4 escapes (\\->\, \n->newline, \"->", \'->')
//if enclosed with apostroph or qoute then remove them
byte *Str::norm(byte *s){
	//cout << '#' << s << '#' << "==>#"; 
	int iLen = strlen((char*)s);
	if(iLen<2) return copy(s);


	int iStart=0;
	int iEnd=iLen;
	if ((s[iStart]==quo && s[iEnd-1]==quo)||
		(s[iStart]==apo && s[iEnd-1]==apo)) {
			iStart++;
			iEnd--;
	}

	int iEscapeNum = 0;
	for(int i=iStart; i<iEnd-1; i++) {
		word chdbl = *((word *) &s[i]);
		if(chdbl==_esc || chdbl==_nln || chdbl==_quo || chdbl==_apo || chdbl==_crt) {
			iEscapeNum++;
			i++;
		}
	}

	int iNewLen = iEnd-iStart-iEscapeNum;
	byte *sr = new byte[iNewLen+1];
	sr[iNewLen]=NULL;

	//delete just debug
	//for(int i=0; i<iNewLen; i++) sr[i] = '_';

	iEscapeNum = 0;
	int iStartCopy = iStart;
	for(int i=iStart; iEscapeNum>0 && i<iEnd-1 ; i++) {
		word chdbl = *((word *) &s[i]);
		if(chdbl==_esc || chdbl==_nln || chdbl==_quo || chdbl==_apo || chdbl==_crt) {
			switch (chdbl) {
					case _esc: sr[i-iStart-iEscapeNum]=esc; break;
					case _nln: sr[i-iStart-iEscapeNum]=nlnr; break;
					case _crt: sr[i-iStart-iEscapeNum]=crtr; break;
					case _quo: sr[i-iStart-iEscapeNum]=quo; break;
					case _apo: sr[i-iStart-iEscapeNum]=apo; break;
			}
			if(i-iStartCopy>0) {
				strncpy((char*) &sr[iStartCopy-iStart-iEscapeNum], (char*) &s[iStartCopy], i-iStartCopy);
				iEscapeNum++;
				iStartCopy=++i;
			}
		}
	}
	if(iEnd-iStartCopy>0)
		strncpy((char*) &sr[iStartCopy-iStart-iEscapeNum], (char*) &s[iStartCopy], iEnd-iStartCopy);


	return (byte*) sr;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//struct for usage in sort algorithms
//returns true if s1<s2		
bool Str::LowerCharStr::operator()( const char *s1, const char *s2 ) const {
	return strcmp( s1, s2 ) < 0;
}

//-------------------------------------------------------------------------------------------
//struct for usage in sort algorithms
//returns true if s1<s2
bool Str::LowerByteStr::operator()( const byte *s1, const byte *s2 ) const {
	return strcmp( (char *) s1, (char*) s2 ) < 0;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//processing true c++ strings
void TrueStr::Tokenize(const string& str, vector<string>& tokens, const string& delimiters){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//returns true if file can be read
bool CheckFile::CanRead(const char *sFileName){
	ifstream ifs;
	ifs.open(sFileName, ifstream::in);
	ifs.close();
	if(ifs.fail()) {
		ifs.clear(ios::failbit);
		return false;
	}
	return true;
}
//returns true if file can be written or created
bool CheckFile::CanWrite(const char *sFileName, bool bAskOverwrite){
	bool bExists = CanRead(sFileName);

	ofstream ofs;
	ofs.open(sFileName, ofstream::app);
	ofs.close();
	if(ofs.fail()) {
		ofs.clear(ios::failbit);
		return false;
	}
	if (!bExists) 
		remove(sFileName);
	else if(bAskOverwrite) {
		cout << "Are you shure to overwrite the file '" << sFileName << "'? [y,n]: ";
		string sAnwser = "";
		while (sAnwser.length() == 0 || (sAnwser.at(0)!='y' && sAnwser.at(0)!='n'))
			cin >> sAnwser;
		if (sAnwser.at(0)=='y')
			return true;
		else
			return false;
	}

	return true;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//returns more acurate random double [0..1)
double Rand::nextDouble() {
	//(GNU)RAND_MAX = 2147483647, RAND_MAX_REAL = -2147483648 so we don't need to complicate
	//(WIN)RAND_MAX = 32767, RAND_MAX_REAL = 32768 so we complicate

	double d;
	if (RAND_MAX_REAL <0)
		d = (float)rand()/RAND_MAX;
	else {
		d = rand()/RAND_MAX_REAL;
		d = (d + rand())/RAND_MAX_REAL;
		d = (d + rand())/RAND_MAX_REAL;
	}

	return d;
}

//-------------------------------------------------------------------------------------------
//more acurate random in given limits excluding max
int Rand::nextInt(int iMin, int iMax) {
	int iReturn = iMin + floor( (iMax-iMin)*nextDouble() );

	return iReturn;
}

//-------------------------------------------------------------------------------------------
//initializes random seed sequence
void Rand::seed(int iSeed) {
	if (iSeed==0) srand(time(NULL));
	//we want to get equal results over many runs so initialization should be te same every time
	else srand(iSeed);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//constructor
Timing::Timing(){
	bSuppresReset = false;
	Reset();
}
//-------------------------------------------------------------------------------------------
//operator member functions
Timing::operator double() {
	return ElapSec();
}
Timing &Timing::operator-() {
	bSuppresReset = true;
	return *this;
}


//-------------------------------------------------------------------------------------------
//reset timer
void Timing::Reset(){
	if (!bSuppresReset)
		clkStart = clock();
	else
		bSuppresReset = false;
}

//-------------------------------------------------------------------------------------------
//return elapsed seconds and reset
double Timing::ElapSec(bool bReset){
	double dTime = (double(clock())-double(clkStart))/CLOCKS_PER_SEC;
	if (bReset) Reset();
	return dTime;
}

//-------------------------------------------------------------------------------------------
//return elapsed seconds string and reset
const char *Timing::ElapString(bool bReset) {
	ostringstream os;
	os << setprecision(2) << fixed << ElapSec(bReset) << " s";
	os.flush();

	string s = os.str();
	return strcpy(new char[s.length()+1],s.c_str());
}
//-------------------------------------------------------------------------------------------
//prints the time
const void Timing::Print(ostream &os, const char* acPreText, const char* acPostText){
	os << acPreText << setprecision(2) << fixed << ElapSec() << " s" << acPostText << endl;
}
//-------------------------------------------------------------------------------------------
//operator prints the to the standard outstream
ostream &operator<<(ostream &os, Timing &time){
	const char *acTime = time.ElapString(true);
	os << acTime;
	delete[] acTime;
	return os;
}
