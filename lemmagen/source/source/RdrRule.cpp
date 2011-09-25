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
#include "../header/RdrRule.h"

//-------------------------------------------------------------------------------------------
//empty constructor
RdrRule::RdrRule() {
	iObjectCounter = 1;
	iFromLen = -1;
	iToLen = -1;
	acTo = NULL;
	iAddress = -1;
}
//-------------------------------------------------------------------------------------------
//constructor
RdrRule::RdrRule(int iFromLen, byte *acTo) {
	iObjectCounter = 1;
	this->acTo = acTo;
	this->iFromLen = iFromLen;
	iToLen = strlen((char*) acTo);
	iAddress = -1;
}
//-------------------------------------------------------------------------------------------
//destructor
RdrRule::~RdrRule() {
	delete[] acTo;
}
//-------------------------------------------------------------------------------------------
//creates text representation of this rule
char *RdrRule::ToString(const char *acSuffix, const char *acArrow) const {
	char *acFrom = Str::copy((char *) &acSuffix[strlen(acSuffix) - iFromLen]);
	char *acRet = Str::cropJoin((char*) acFrom, acArrow, (char*) acTo, 0);
	delete[] acFrom;
	return acRet;
}
//-------------------------------------------------------------------------------------------
//creates text representation of this rule
char *RdrRule::UniqueStringId(char *acAppend) const {
	char *acId = Str::copyAppend(iFromLen+1, (char*)acTo);
	if (acAppend != NULL) {
		char *acId2 = Str::copyConcat((const char*) acAppend, (const char*) acId);
		delete[] acId;
		acId = acId2;
	}
	return acId;
}
//-------------------------------------------------------------------------------------------
//lematizes word according to this rule
char *RdrRule::Lemmatize(const char *acWord, int iWordLen) const{
	if (iWordLen==-1) iWordLen = strlen(acWord);

	size_t iStemLen = iWordLen - iFromLen;

	char *acReturn = new char[iStemLen + iToLen + 1];
	strncpy(acReturn, acWord, iStemLen);
	strcpy(&acReturn[iStemLen], (char*) acTo);

	return acReturn;
}
//-------------------------------------------------------------------------------------------
//returns length of this rule in bytes
int RdrRule::GetByteLength() {
	return 3+iToLen;
}
//-------------------------------------------------------------------------------------------
//sets address of this subnode in byte array
void RdrRule::SetAddress(int &iAddr, bool bReset) {
	bool bNotSet = (iAddress == -1);
	if (bReset || bNotSet) iAddress = iAddr;
    if (!bReset && bNotSet) iAddr += GetByteLength();
}
//-------------------------------------------------------------------------------------------
//sets just the code of this node on the addres that was predefined
void RdrRule::SetByteCodeThis(byte *abData, bool bEntireWord) {
	int iAddr = iAddress;

	//flag
	abData[iAddr] = (bEntireWord ? TypeRuleEw : TypeRule);
	iAddr += FlagLen;

	//from length
	abData[iAddr] = iFromLen;
	iAddr += LenSpecLen;

	//to length
	abData[iAddr] = iToLen;
	iAddr += LenSpecLen;

	//to string
	strncpy((char*) &abData[iAddr], (char*) acTo, iToLen);
	iAddr += iToLen;

	//check if size is ok
	if (iAddr != iAddress + GetByteLength()) {
		cerr << "Internal error: Declaration size and definition size of a rule diferate!" << endl;
		cerr << "          rule: " << iFromLen << ToString("") << endl;
	}
}