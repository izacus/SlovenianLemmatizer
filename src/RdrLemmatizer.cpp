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
#include <stdint.h>

#include "RdrLemmatizer.h"

#ifdef WIN32
#define strdup _strdup
#endif

//-------------------------------------------------------------------------------------------
//constructors
RdrLemmatizer::RdrLemmatizer(uint8_t* data, uint32_t dataLen)
{
	this->abData = data;
	this->iDataLen = dataLen;
}

RdrLemmatizer::RdrLemmatizer(const char *acFileName)
{
	LoadBinary(acFileName);
}

RdrLemmatizer::RdrLemmatizer()
{
	this->abData = (uint8_t *)abDataStatic;
	this->iDataLen = iDataLenStatic;
}

//-------------------------------------------------------------------------------------------
//destructor
RdrLemmatizer::~RdrLemmatizer()
{
	if (this->abData != (uint8_t *)abDataStatic)
		delete[] abData;
}

//-------------------------------------------------------------------------------------------
//returns size of a tree (number of bytes)
uint32_t RdrLemmatizer::SizeOfTree() const
{
	return iDataLen - DataStart;
}

//-------------------------------------------------------------------------------------------
//lematizes word according to this data
char *RdrLemmatizer::Lemmatize(const char *acWord, char *acOutBuffer) const
{
	// Original code only assumes lengths up to 256, truncate forcefully if buffers aren't correct.
	const auto len = strlen(acWord);
	uint8_t bWordLen = len > 250 ? 250 : (uint8_t)len;
	uint32_t iAddr = DataStart;
	uint32_t iParentAddr = DataStart;
	uint32_t iTmpAddr;
	int16_t bLookChar = bWordLen;
	uint8_t bType = abData[iAddr];

	while (true)
	{
		iTmpAddr = iAddr + FlagLen + AddrLen;

		//check if additional characters match
		if ((bType & BitAddChar) == BitAddChar)
		{
			auto bNewSufxLen = abData[iTmpAddr];
			iTmpAddr += LenSpecLen;

			bLookChar -= bNewSufxLen;

			//test additional chars if ok
			if (bLookChar >= 0)
				do
					bNewSufxLen--;
				while (bNewSufxLen != 255 && abData[iTmpAddr + bNewSufxLen] == (uint8_t)acWord[bLookChar + bNewSufxLen]);

			//wrong node, take parents rule
			if (bNewSufxLen != 255)
			{
				iAddr = iParentAddr;
				break;
			}

			//right node, but we are at the end (there will be no new loop) finish by returning this rule
			if ((bType & ~BitEntireWr) == TypeLeafAC)
				break;

			//right node and we need to go on with subnodes (it si probably type TypeIntrAC )
			//set iTmpAddr to start of hashtable
			iTmpAddr += abData[iTmpAddr - LenSpecLen];
		}

		//move lookup char back
		bLookChar--;
		//check if we are still inside the word (bLookChar==0 when at the begining of word)
		if (bLookChar < 0)
		{
			//this means that we are just one character in front of the word so we must look for entireword entries
			if ((bType & BitInternal) == BitInternal)
			{
				//go to the hashtable position 0(NULL) and look idf address is not NULL
				iTmpAddr += ModLen;
				const auto bChar = abData[iTmpAddr];
				GETDWORD(, iTmpAddr, iTmpAddr + CharLen);
				if (bChar == 0 && iTmpAddr != 0)
				{
					//we have a candidate for entireword, redirect addresses
					iParentAddr = iAddr;
					iAddr = iTmpAddr;
					bType = abData[iAddr];
					//increase lookchar (because we actualy eat one character)
					bLookChar++;
				}
			}
			break;
		}

		//find best node in hash table
		if ((bType & BitInternal) == BitInternal)
		{
			const uint8_t bMod = abData[iTmpAddr];
			const uint8_t bChar = (uint8_t)acWord[bLookChar];

			iTmpAddr += ModLen + (bChar % bMod) * (AddrLen + CharLen);

			iTmpAddr = abData[iTmpAddr] == bChar ? iTmpAddr + CharLen : iAddr + FlagLen;

			iParentAddr = iAddr;
			GETDWORD(, iAddr, iTmpAddr);
			bType = abData[iAddr];

			if ((bType & ~BitEntireWr) == TypeRule)
				break;
		}
	}
	//if this is entire-word node, and we are not at the begining of word it's wrong node - take parents
	if ((bType & BitEntireWr) == BitEntireWr && bLookChar != 0)
	{
		iAddr = iParentAddr;
		bType = abData[iAddr];
	}

	//search ended before we came to te node of type rule but current node is OK so find it's rule node
	if ((bType & ~BitEntireWr) != TypeRule)
		GETDWORD(, iAddr, iAddr + FlagLen);

	//we have (100%) node of type rule for lemmatization - now it's straight forward to lemmatize
	//read out rule
	iTmpAddr = iAddr + FlagLen;
	uint8_t iFromLen = abData[iTmpAddr];
	iTmpAddr += LenSpecLen;
	uint8_t iToLen = abData[iTmpAddr];
	iTmpAddr += LenSpecLen;

	//prepare output buffer
	uint8_t iStemLen = bWordLen - iFromLen;
	char *acReturn = acOutBuffer == nullptr ? new char[iStemLen + iToLen + 1] : acOutBuffer;

	//do actual lematirazion using given rule
	memcpy(acReturn, acWord, iStemLen);
	memcpy(&acReturn[iStemLen], &abData[iTmpAddr], iToLen);
	acReturn[iStemLen + iToLen] = 0;

	return acReturn;
}

//-------------------------------------------------------------------------------------------
//loads all data needed from binary file
void RdrLemmatizer::LoadBinary(std::istream &is)
{
	iDataLen = 0;
	is.read((char *)&iDataLen, 4);
	abData = new uint8_t[iDataLen];
	is.read((char *)abData, iDataLen);
}
//-------------------------------------------------------------------------------------------
//loads all data needed from binary file
void RdrLemmatizer::LoadBinary(const char *acFileName)
{
	std::ifstream ifs(acFileName, std::ios_base::in | std::ios_base::binary);
	LoadBinary(ifs);
	ifs.close();
}
