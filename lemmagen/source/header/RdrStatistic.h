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

#include "../header/WordList.h"

class RdrStatistic {
public:
	static int StatAll(vector<string> vsInfile, string sOutfile, string sTextfile, bool bVerbose);
	static int Statistics(WordList &wls, ostream &os, bool bVerbose);
	static int CoveringStat(WordList &wls, string sTextfile, ostream &os, bool bVerbose);
	static void CreateHeadLines(ostream &os, bool bCoverStat);
	static map<string, int> GetWordsFromFile(string sFileName);
};


