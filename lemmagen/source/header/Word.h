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

//encapsulte word object - word, lemma and form
class Word  {
public:
	//variables
	char *acWord;
	char *acLemm;
	char *acForm;

	char *acWordEnd;

	int iWordLen;
	int iStemLen;
	int iRule;
	int *iSet;

	int *iObjectCounter;

public:
	//subclasses
	struct LessThan;

public:
	//constructors/destructors
	Word();
	Word(char *acLine, const char cAddPrefix=NULL, const char cDelimit = '\t');
	~Word(void);

	//copy constructors and assigment operator
	Word(const Word &w);
	Word &operator=(const Word &w);

	//initialization & delition
	void Initialize(char *acLine, const char cAddPrefix=NULL, const char cDelimit = '\t');
	void Clear();

	//copy function
	void Copy(const Word &wl);

	//comparator
	bool operator==(const Word &y) const;
	bool operator<(const Word &y) const;

	//output functions
	char *GetStem();
	char *GetRule(char *acArrow = "\t");
	char *ToString(bool bExtend = false, const char cRemPrefix = NULL);

};

//-------------------------------------------------------------------------------------------
//comparator function objects
struct Word::LessThan {
	//type
	enum Type { forw, back };
	Type type;
	//constructor
	LessThan();
	LessThan(Type type);
	//operator
	bool operator()(const Word &w1, const Word &w2 ) const;
	bool operator()(const Word *&w1, const Word *&w2 ) const;
	//comparator
	virtual bool forward(const Word &w1, const Word &w2) const;
	virtual bool backward(const Word &w1, const Word &w2) const;
};

