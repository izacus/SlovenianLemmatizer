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

#include "MiscLib.h"

//encapsulates rule representation (hor to transform word to it's lemma)
class Rule {
public:
	//variables
	char *acLeft;
	char *acRight;

	short int iLeftLen;
	short int iRightLen;

	int iSupportCount;
	int *iObjectCounter;

	//temporal variables for tree building algorithm
	int iWordCount;
	bool bSeen;
	bool bAppliable;

public:
	//constructors/destructors
	Rule(void);
	Rule(char *acRule);
	~Rule(void);

	//copy constructors and assigment operator
	Rule(const Rule &r);
	Rule &operator=(const Rule &r);

	//initialization & delition
	void Initialize(char *acRule);
	void Clear();

	//copy function
	void Copy(const Rule &wl);

	//comparator
	bool operator==(const Rule &y) const;
	bool operator<(const Rule &y) const;

	//lemmatizer function
	char *Lematize(const char* acWord, char *acOutBuffer = NULL) const;

	//output functions
	char *ToStirng(char *acLabel = "->") const;
};