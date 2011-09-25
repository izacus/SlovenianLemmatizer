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

//learning alg
#include "../header/Node.h"
#include "../header/JoelRdr.h"
//lemmatizer generator
#include "../header/RdrScanner.h"


class RdrXval {
public:
	static int Xval(string sInfile, string sOutfile, int iSplit, int iType, int iAlg, 
					bool bExtended, bool bVerbose, bool bSilent);
	static void CrossValidMat(ostream &os, WordList &wlst, int iTestSet, bool bExtended,
								bool bVerbose, double &dAccTest, double &dAccTrain);
	static void CrossValidJoel(ostream &os, WordList &wlst, int iTestSet, bool bExtended,
								bool bVerbose, double &dAccTest, double &dAccTrain);
	static bool TestRdrLemm(WordList::iterator &itStart, WordList::iterator &itEnd, RdrLemmatizer &rdrLemm, ostream &os, bool bMatjaz);
	static bool TestRdrTree(stringstream &ss, WordList &wlFile, ostream &os, bool bMatjaz, int kSplit);
	static void CreateHeadLines(ostream &os, int iTests);
};