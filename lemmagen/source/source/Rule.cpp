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
#include "../header/Rule.h"

//-------------------------------------------------------------------------------------------
//constructors / destructors
Rule::Rule() {
	iObjectCounter = new int(1);
	acLeft = NULL;
	acRight = NULL;
}
Rule::Rule(char *acRule) {
	iObjectCounter = new int(1);
	Initialize(acRule);
}
Rule::~Rule(void) {
	Clear();
}
//-------------------------------------------------------------------------------------------
//copy constructors and assigment operator
Rule::Rule(const Rule &r) {
	Copy(r);
}
Rule &Rule::operator=(const Rule &r) {
	if(&r!=this) {
		Clear();
		Copy(r);
	}
	return *this;
}
//-------------------------------------------------------------------------------------------
//initialization functions
void Rule::Initialize(char *acRule) {
	acLeft = acRule;
	acRight = &(strchr(acLeft, '\t')[1]);
	acRight[-1] = NULL;

	iLeftLen = strlen(acLeft);
	iRightLen = strlen(acRight);

	iSupportCount = 0;

	iWordCount = 0;
	bSeen = false;
	bAppliable = false;
}
//-------------------------------------------------------------------------------------------
//deleting functions
void Rule::Clear() {
	(*iObjectCounter)--;

	if((*iObjectCounter)==0) {
		delete[] acLeft;
		delete iObjectCounter;
	}
}

//-------------------------------------------------------------------------------------------
//copy function
void Rule::Copy(const Rule &r) {
	acLeft = r.acLeft;
	acRight = r.acRight;

	iLeftLen = r.iLeftLen;
	iRightLen = r.iRightLen;

	iSupportCount = r.iSupportCount;

	iWordCount = r.iWordCount;
	bSeen = r.bSeen;
	bAppliable = r.bAppliable;

	iObjectCounter = r.iObjectCounter;
	(*iObjectCounter)++;
}
//-------------------------------------------------------------------------------------------
//equality / less than operators
bool Rule::operator==(const Rule &y) const {
	return (strcmp(acLeft, y.acLeft) == 0) && (strcmp(acRight, y.acRight) == 0);
}
bool Rule::operator<(const Rule &y) const {
	return (strcmp(acLeft, y.acLeft) < 0) || 
		((strcmp(acLeft, y.acLeft) == 0) && (strcmp(acRight, y.acRight) < 0));
}

//-------------------------------------------------------------------------------------------
//lematizer function (transforms word to its lemma)
char *Rule::Lematize(const char* acWord, char *acOutBuffer) const{
	size_t iStemLen = strlen(acWord) - iLeftLen;

	char *acReturn = acOutBuffer == NULL ? new char[iStemLen + iRightLen + 1] : acOutBuffer;
	
	memcpy(acReturn, acWord, iStemLen);
	memcpy(&acReturn[iStemLen], acRight, iRightLen);
	acReturn[iStemLen + iRightLen] = NULL;
	
	return acReturn;
}

//-------------------------------------------------------------------------------------------
//aux output functions
char *Rule::ToStirng(char *acLabel) const {
	return Str::cropJoin(acLeft, acLabel, acRight, 0);
}

