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
#include "../header/Word.h"

//-------------------------------------------------------------------------------------------
//constructors / destructors
Word::Word(){
	iObjectCounter = new int(1);
	iSet = new int(0);
	acWord = NULL;
	acLemm = NULL;
	acForm = NULL;
}
Word::Word(char *acLine, const char cAddPrefix, const char cDelimit){
	iObjectCounter = new int(1);
	iSet = new int(0);
	Initialize(acLine, cAddPrefix, cDelimit);
}

Word::~Word(void){
	Clear();
}
//-------------------------------------------------------------------------------------------
//copy constructors and assigment operator
Word::Word(const Word &w) {
	Copy(w);
}
Word &Word::operator=(const Word &w) {
	if(&w!=this) {
		Clear();
		Copy(w);
	}
	return *this;
}
//-------------------------------------------------------------------------------------------
//initialization functions
void Word::Initialize(char *acLine, const char cAddPrefix, const char cDelimit){
	int iChar = 0;
	bool bEnd = false;
	for (int iWord = 0; iWord < 2; iWord++) {

		while(acLine[iChar] != cDelimit && acLine[iChar] != NULL) 
			iChar++;
		bEnd = (acLine[iChar] == NULL);
		acLine[iChar] = NULL;

		switch (iWord) {			
			case 0:
				acWord = Str::copyAppend(cAddPrefix, acLine);
				iWordLen = strlen(acWord);
				acWordEnd = &acWord[iWordLen];
				break;
			case 1:
				if (acLine[0]=='='/* && acLine[1]=='\t'*/)
					acLemm = Str::copyAppend(cAddPrefix, acWord);
				else
					acLemm = Str::copyAppend(cAddPrefix, acLine);
				break;
		} 
		
		acLine = &acLine[iChar + (bEnd?0:1)];
		iChar = 0;
	}

	acForm = Str::copy(acLine);
	iStemLen = Str::eqPrefixLen(acWord, acLemm);
	iRule = 0;
}
//-------------------------------------------------------------------------------------------
//deleting functions
void Word::Clear() {
	(*iObjectCounter)--;

	if((*iObjectCounter)==0) {
		delete[] acWord;
		delete[] acLemm;
		delete[] acForm;
		delete iObjectCounter;
		delete iSet;
	}
}

//-------------------------------------------------------------------------------------------
//copy function
void Word::Copy(const Word &w) {
	acWord = w.acWord;
	acLemm = w.acLemm;
	acForm = w.acForm;

	acWordEnd = w.acWordEnd;

	iStemLen = w.iStemLen;
	iRule = w.iRule;
	iWordLen = w.iWordLen;
	
	iSet = w.iSet;
	iObjectCounter = w.iObjectCounter;
	(*iObjectCounter)++;
}
//-------------------------------------------------------------------------------------------
//equality / less than operators
bool Word::operator==(const Word &y) const {
	return (strcmp(acWord, y.acWord) == 0 && strcmp(acLemm, y.acLemm) == 0 );
}
bool Word::operator<(const Word &y) const {
	return (strcmp(acWord, y.acWord) < 0 ||
		strcmp(acWord, y.acWord) == 0 && strcmp(acLemm, y.acLemm) < 0);
}

//-------------------------------------------------------------------------------------------
//aux output functions
char *Word::GetStem(){
	char *acStem = new char[iStemLen+ 1];
	acStem[iStemLen] = NULL;
	strncpy(acStem, acWord, iStemLen);

	return acStem;
}

char *Word::GetRule(char *acArrow){
	return Str::cropJoin(&acWord[iStemLen], acArrow, &acLemm[iStemLen], 0);
}

char *Word::ToString(bool bExtend, const char cRemPrefix){
	char *delimit = (char*)(bExtend ? ", ": "\t");

	int iRemWord = (cRemPrefix==*acWord && cRemPrefix!=NULL)?1:0;
	int iRemLemm = (cRemPrefix==*acLemm && cRemPrefix!=NULL)?1:0;

	size_t len = strlen(acWord) - iRemWord
		+ strlen(acLemm) - iRemLemm
		+ strlen(acForm) + strlen(delimit)*2;

	char *acReturn = new char[len+1];
	acReturn[0] = NULL;

	strcat(acReturn, &acWord[iRemWord]);
	strcat(acReturn, delimit);
	strcat(acReturn, &acLemm[iRemLemm]);
	strcat(acReturn, delimit);
	strcat(acReturn, acForm);

	if (!bExtend)
		return acReturn;
	else {
		char *acLabelX = "set(";
		char *acLabel0 = ") word(";
		char *acLabel1 = ") stem(";
		char *acLabel2 = ") rule(";
		char *acLabel3 = ")";

		char acSet[2];
		acSet[0] = '0' + *iSet;
		acSet[1] = NULL;

		char *acStem = GetStem();
		char *acRule = GetRule("-->");

		len = strlen(acLabel0) + strlen(acLabel1) + 
			strlen(acLabel2) + strlen(acLabel3) +
			strlen(acReturn) + strlen(acStem) + 
			strlen(acRule) +  strlen(acLabelX) + 
			strlen(acSet);

		char *acExt = new char[len + 1];
		acExt[0] = NULL;

		strcat(acExt, acLabelX);
		strcat(acExt, acSet);
		strcat(acExt, acLabel0);
		strcat(acExt, acReturn);
		strcat(acExt, acLabel1);
		strcat(acExt, acStem);
		strcat(acExt, acLabel2);
		strcat(acExt, acRule);
		strcat(acExt, acLabel3);

		delete[] acStem;
		delete[] acRule;
		delete[] acReturn;

		return acExt;
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//constructors	
Word::LessThan::LessThan(){
	this->type = forw;
}
Word::LessThan::LessThan(Type type){
	this->type = type;
}

//-------------------------------------------------------------------------------------------
//function object opeartor ()
bool Word::LessThan::operator()(const Word &w1, const Word &w2) const { 
	switch(type) {
		case forw: return forward(w1,w2);
		case back: return backward(w1,w2);
		default: return true;
	}
};
//function object opeartor ()
bool Word::LessThan::operator()(const Word *&w1, const Word *&w2) const { 
	switch(type) {
		case forw: return forward(*w1,*w2);
		case back: return backward(*w1,*w2);
		default: return true;
	}
};

//-------------------------------------------------------------------------------------------
//comparator functions
bool Word::LessThan::forward(const Word &w1, const Word &w2) const {
	byte *acw1 = (byte *) w1.acWord;
	byte *acw2 = (byte *) w2.acWord;

	int i=0;
	while (acw1[i]==acw2[i] && acw2[i]!=NULL) i++;

	return acw1[i]<acw2[i];

}

bool Word::LessThan::backward(const Word &w1, const Word &w2) const {
	byte *acw1 = (byte *) w1.acWordEnd-1;
	byte *acw2 = (byte *) w2.acWordEnd-1;
	int iMaxLen = w1.iWordLen > w2.iWordLen ? -w2.iWordLen : -w1.iWordLen;

	int i=0;
	while (i!=iMaxLen && acw1[i]==acw2[i]) i--;

	int iComp = 0;
	if (i==iMaxLen)
		iComp = w1.iWordLen < w2.iWordLen;
	else
		iComp = acw1[i] < acw2[i];



	return iComp;
}

