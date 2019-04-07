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
#pragma once

#define AllInOneFile

//-------------------------------------------------------------------------------------------
//includes
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#if defined(_MSC_VER) && _MSC_VER < 1600
	#include "stdint-msvc90.h"
	#define nullptr NULL
#else 	
	#include <cstdint>
#endif

#include "string.h"

//-------------------------------------------------------------------------------------------
//const variables that algorithm depends on
#define AddrLen 4
#define FlagLen 1
#define ModLen 1
#define LenSpecLen 1
#define CharLen 1
#define DataStart 0

#define RoundDataLen 8

#define BitDefault	0x00
#define BitAddChar	0x01
#define BitInternal	0x02
#define BitEntireWr	0x04

#define TypeRule		(BitDefault)
#define TypeRuleEw		(BitDefault | BitEntireWr)
#define TypeLeafAC		(BitDefault | BitAddChar)
#define TypeLeafACEw	(BitDefault | BitAddChar | BitEntireWr)
#define TypeIntr		(BitDefault | BitInternal)
#define TypeIntrAC		(BitDefault | BitAddChar | BitInternal)

//-------------------------------------------------------------------------------------------
#define GETDWORD(type, wVar, wAddr) \
			type wVar = *((uint32_t *) &abData[wAddr])

#define GETBYTEMOVE(type, bByte, iSize) \
			type bByte = abData[iAddr]; \
			iAddr += iSize

#define GETDWORDMOVE(type, wVar, iSize) \
			GETDWORD(type, wVar, iAddr);  \
			iAddr += iSize

#define GETSTRINGMOVE(type, acString, iSize) \
			type acString = new char[iSize+1]; \
			strncpy(acString, (char*) &abData[iAddr], iSize); \
			acString[iSize] = NULL; \
			iAddr += iSize

//-------------------------------------------------------------------------------------------

class RdrLemmatizer{
public:
	uint8_t* abData;
	int32_t iDataLen;

public:	
	RdrLemmatizer(const char *acFileName);	
	~RdrLemmatizer();

	uint32_t SizeOfTree() const;

	char *Lemmatize(const char *acWord, char *acOutBuffer = nullptr) const;
};
