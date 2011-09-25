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
#include "../header/RdrXval.h"

//-------------------------------------------------------------------------------------------	
//main function for cross validation
int RdrXval::Xval(string sInfile, string sOutfile, int iSplit, int iType, int iAlg, 
					bool bExtended, bool bVerbose, bool bSilent) {
	bool bVerb = !bSilent && bVerbose;
	bool bSlnt = bSilent && bVerbose;
	
	//timing
	Timing timeAll;
	if (bVerb) cout << "  Test started...";

	//import files	
	WordList *wlstOrig  = new WordList(sInfile.c_str(), NULL, bVerb);
	
	//split on sets
	if (iType<2)
		wlstOrig->SplitCrossValid(iSplit, iType, bVerb*2);
	else
		wlstOrig->SplitSubset(/*(100-*/iSplit/*)*(wlstOrig->iWordNum/100)*/, iType%2, bVerb*2);

	//sort if needed
	WordList *wlstSort = wlstOrig;
	if (iAlg == 2) wlstSort = new WordList(*wlstOrig);
	if (iAlg == 2 || iAlg == 0) wlstSort->Sort(bVerb);

	//wlstOrig->Export("notsorted_test.txt",0,0,true);
	//wlstSort->Export("sorted_test.txt",0,0,true);

	//output file if needed or dummy if not
	ostream *os = new stringstream();
	if (sOutfile != "") os = new ofstream(sOutfile.c_str(), ofstream::app | ofstream::out);

	//create head lines
	CreateHeadLines(*os, (iAlg==2?2:1));
	
	//timing
	Timing timeXval;
	if (bVerb) cout << "  Test started..." << endl;
	if (bSlnt) cout << sInfile.substr( max(0, (int)(sInfile.length()-18)) ) << ": ";

	//averarge accurancies
	double dAccTest;
	double dAccTrain;
	double dAccMatTest = 0; 
	double dAccMatTrain = 0; 
	double dAccJoelTest = 0;
	double dAccJoelTrain = 0;

	//doit
	int iSetNum = (iType<2 ? iSplit : 1);
	for (int iSet = 0; iSet < iSetNum; ++iSet) {
		Timing time;
		//--------------------------------------------------------------------------
		//print out initial data
		*os << "#" << setw(45) << setfill(' ') << right << sInfile << " ";
		*os << "|" << setw(6)  << setfill(' ') << right << 1 << " ";
		*os << "|" << setw(6)  << setfill(' ') << right << iSet << " ";
		*os << "|" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec(false) << " ";
		*os << "#" << setw(7)  << setfill(' ') << right << wlstOrig->GetWordNum(-1) << " ";
		*os << "|" << setw(7)  << setfill(' ') << right << wlstOrig->GetWordNum(iSet, false) << " ";
		*os << "|" << setw(7)  << setfill(' ') << right << wlstOrig->GetWordNum(iSet, true) << " ";
		//--------------------------------------------------------------------------

		if (bSlnt) cout << iSet+1 << "";
		if (bVerb) cout << "    Set " << iSet+1 << " - ";

		Timing timeThis;
		if (iAlg!=1) {
			*os << "##";
			if (bVerb) cout << "cover:[";
			
			CrossValidMat(*os, *wlstSort, iSet, bExtended, bVerbose, dAccTest, dAccTrain);
			dAccMatTest += dAccTest;
			dAccMatTrain += dAccTrain;

			if (bVerb) 
				cout << "] " 
				     << setprecision(2) << fixed << dAccTest << "% " 
					 << setprecision(2) << fixed << dAccTrain << "% " 
					 << timeThis.ElapSec(true) << "s ";
		}
		
		if (bSlnt && iAlg==2) cout << "|";

		if (iAlg!=0) {
			*os << "##";
			if (bVerb) cout << "rdr:[";
			
			CrossValidJoel(*os, *wlstOrig, iSet, bExtended, bVerbose, dAccTest, dAccTrain);
			dAccJoelTest += dAccTest;
			dAccJoelTrain += dAccTrain;

			if (bVerb) 
				cout << "] " 
				     << setprecision(2) << fixed << dAccTest << "% " 
					 << setprecision(2) << fixed << dAccTrain << "% " 
					 << timeThis.ElapSec(true) << "s ";
		}

		if (bSlnt) cout << "";
		if (bVerb) cout << endl;
		

		//--------------------------------------------------------------------------
		//close this line
		*os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec(true) << " ";
		*os << "#" << endl;
		//--------------------------------------------------------------------------
	}

	if (bVerb) {
		if (dAccMatTest != 0)
			cout << "    Average for cover  (on test set): " << setprecision(4) << fixed << dAccMatTest/iSetNum << "%" << endl;
		if (dAccJoelTest != 0)
			cout << "    Average for rdr    (on test set): " << setprecision(4) << fixed << dAccJoelTest/iSetNum << "%" << endl;
		if (dAccMatTrain != 0)
			cout << "    Average for cover (on train set): " << setprecision(4) << fixed << dAccMatTrain/iSetNum << "%" << endl;
		if (dAccJoelTrain != 0)
			cout << "    Average for rdr   (on train set): " << setprecision(4) << fixed << dAccJoelTrain/iSetNum << "%" << endl;
	}

	//close the file
	if (sOutfile != "")
		((ofstream*)os)->close();

	//cleanup
	if (wlstOrig != wlstSort) {
		delete wlstOrig;
		delete wlstSort;
	} else
		delete wlstOrig;
	delete os;

	if (bVerb) {
		if (sOutfile != "")
			 cout << "  Results writen to file '" << sOutfile << "'." << endl;
		timeXval.Print(cout, "  Time needed for all tests ");
		timeAll.Print(cout, "  Time needed altogether ");
	}
	if (bSlnt) cout << " " << setprecision(1) << fixed << timeAll.ElapSec() << "s" << endl;

	return 0;
}

//-----------------------------------------------------------------------------
void RdrXval::CrossValidMat(ostream &os, WordList &wlst, int iTestSet, bool bExtended,
								bool bVerbose, double &dAccTest, double &dAccTrain) {
	//learn with my algorithm	
	Node *nStart = Node::BuildTree(wlst,iTestSet,false,2,os);
	nStart->PrintStat(os, 2);
	if(bVerbose) cout << "."; //1

	//--------------------------------------------------------------------------
	//test accurancy with my algorithm	
	dAccTrain = nStart->TestAccurancy(wlst.begin(iTestSet,false),wlst.end(iTestSet,false),2,os);
	dAccTest = nStart->TestAccurancy(wlst.begin(iTestSet,true),wlst.end(iTestSet,true),2,os);
	if(bVerbose) cout << "."; //2

	//--------------------------------------------------------------------------
	//print tree representation to memory
	stringstream ssMat;
	nStart->PrintTree(ssMat, wlst, 0);
	if(bVerbose) cout << "."; //3

	//--------------------------------------------------------------------------
	//test almost universaly from here on for my and joel's algorithm
	if (!TestRdrTree(ssMat, wlst, os, true, iTestSet)) 
		{ssMat.seekg(ios_base::beg); os << ssMat.str(); os.flush(); return;}
	if(bVerbose) cout << "."; //4

	//--------------------------------------------------------------------------
	//clean after matjaz's part of lematization
	delete nStart;
}

//-------------------------------------------------------------------------------------------	
void RdrXval::CrossValidJoel(ostream &os, WordList &wlst, int iTestSet, bool bExtended,
								bool bVerbose, double &dAccTest, double &dAccTrain) {
	//learn with joel's algorithm	
	JoelRdr *rdrJoel = new JoelRdr();
	rdrJoel->train(wlst.begin(iTestSet,false),wlst.end(iTestSet,false),2,os);		
	rdrJoel->PrintStat(os, 2);
	if(bVerbose) cout << "."; //1

	//--------------------------------------------------------------------------
	//test accurancy with joels algorithm	
	dAccTrain = rdrJoel->lemmatize(wlst.begin(iTestSet,false),wlst.end(iTestSet,false),2,os);
	dAccTest = rdrJoel->lemmatize(wlst.begin(iTestSet,true),wlst.end(iTestSet,true),2,os);
	if(bVerbose) cout << "."; //2

	//--------------------------------------------------------------------------
	//print tree representation to memory
	stringstream ssJoel;
	rdrJoel->PrintTree(&ssJoel,"");
	if(bVerbose) cout << "."; //3

	//--------------------------------------------------------------------------
	//test almost universaly from here on for my and joel's algorithm
	if (!TestRdrTree(ssJoel, wlst, os, false, iTestSet))
		{ssJoel.seekg(ios_base::beg); os << ssJoel.str(); os.flush(); return;}
	if(bVerbose) cout << "."; //4

	//--------------------------------------------------------------------------
	//clean after joels's part of lematization
	delete rdrJoel;
}

//-----------------------------------------------------------------------------
bool RdrXval::TestRdrLemm(WordList::iterator &itStart, WordList::iterator &itEnd, RdrLemmatizer &rdrLemm, ostream &os, bool bMatjaz) {

	int iTrue = 0;
	char *acInBuff = new char[255];
	char *acOutBuff = new char[255];

	if (bMatjaz) acInBuff[0]='#';
	if (bMatjaz) acOutBuff[0]='#';

	Timing time;

	for (WordList::iterator it = itStart; it != itEnd; ++it) {
		strcpy(&acInBuff[bMatjaz],it->acWord);
		rdrLemm.Lemmatize(acInBuff, acOutBuff);
		if (strcmp(&acOutBuff[bMatjaz], it->acLemm) == 0)	
			iTrue++;		
	}

	double dAccurancy = (double) iTrue*100 / itStart.GetWordNum();
	double dTimeNs = time * 1000000000 / itStart.GetWordNum();

	os << "#" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dAccurancy << "% ";
	os << "|" << setw(10) << setfill(' ') << right << iTrue << " ";
	os << "|" << setw(8) << setfill(' ') << right << fixed << setprecision(0) << dTimeNs << " ";



	delete[] acInBuff;
	delete[] acOutBuff;

	return true;
}

//-----------------------------------------------------------------------------
bool RdrXval::TestRdrTree(stringstream &ss, WordList &wlFile, ostream &os, bool bMatjaz, int kSplit) {
	//--------------------------------------------------------------------------
	//create standard tree representation
	RdrScanner::mapcr *mapRules = new RdrScanner::mapcr();
	RdrScanner *scanner = new RdrScanner();
	ss.seekg(ios_base::beg);
	
	Timing time;
	RdrTree *rdrStart = scanner->scan(*mapRules, &ss, &cout, 0);
	os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";

	if (!scanner->bSuccess) return false;

	//--------------------------------------------------------------------------
	//create optimized lemmatizer data structure from tree in memory

	RdrLemmatizer *rdrLemm = rdrStart->CreateLemmatizer(*mapRules);
	os << "|" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";
	rdrStart->PrintStat(os,2);

	//--------------------------------------------------------------------------
	//test accurancy with algorithm on new structure
	if (!TestRdrLemm(wlFile.begin(kSplit,false), wlFile.end(kSplit,false),*rdrLemm,os,bMatjaz)) return false;
	if (!TestRdrLemm(wlFile.begin(kSplit,true), wlFile.end(kSplit, true),*rdrLemm,os,bMatjaz)) return false;

	delete scanner;
	delete rdrStart;
	delete rdrLemm;

	for(RdrScanner::mapcr::iterator it = mapRules->begin(); it != mapRules->end(); ++it) {
		delete[] (*it).first;
		delete (*it).second;
	}	
	delete mapRules;

	return true;
}

//-----------------------------------------------------------------------------
void RdrXval::CreateHeadLines(ostream &os, int iTests) {
	os << "#" << setw(45)  << setfill(' ') << right << "FileName" << " ";
	os << "|" << setw(6)  << setfill(' ') << right << "Iter." << " ";
	os << "|" << setw(6)  << setfill(' ') << right << "Split" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Time s" << " ";
	os << "#" << setw(7)  << setfill(' ') << right << "Words" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Train" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Test" << " ";
	
	for(int i=0; i<iTests; i++) {
		os << "##";

		os << "#" << setw(7)  << setfill(' ') << right << "Buil s" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Nodes" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Leaves" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Inter" << " ";
		os << "|" << setw(6)  << setfill(' ') << right << "Depth" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Branch" << " ";
		os << "#" << setw(7)  << setfill(' ') << right << "Words" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Pos" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Neg" << " ";
		os << "|" << setw(11) << setfill(' ') << right << "Acc" << " ";
		os << "|" << setw(11) << setfill(' ') << right << "Err" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "AvgDep" << " ";

		os << "#" << setw(11) << setfill(' ') << right << "TrainAcc" << " ";
		os << "|" << setw(10) << setfill(' ') << right << "TrainTrue" << " ";
		os << "|" << setw(8) << setfill(' ') << right << "ns/word" << " ";
		os << "#" << setw(11) << setfill(' ') << right << "TestAcc" << " ";
		os << "|" << setw(10) << setfill(' ') << right << "TestTrue" << " ";
		os << "|" << setw(8) << setfill(' ') << right << "ns/word" << " ";

		os << "#" << setw(7)  << setfill(' ') << right << "Pars s" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Opti s" << " ";
		os << "#" << setw(7)  << setfill(' ') << right << "Nodes" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Leaves" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Inter" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "AvgDep" << " ";
		os << "|" << setw(7)  << setfill(' ') << right << "Branch" << " ";

		os << "#" << setw(11) << setfill(' ') << right << "TrainAccR" << " ";
		os << "|" << setw(10) << setfill(' ') << right << "TrainTruR" << " ";
		os << "|" << setw(8) << setfill(' ') << right << "ns/word" << " ";
		os << "#" << setw(11) << setfill(' ') << right << "TestAccR" << " ";
		os << "|" << setw(10) << setfill(' ') << right << "TestTruR" << " ";
		os << "|" << setw(8) << setfill(' ') << right << "ns/word" << " ";
	}

	os << "#" << setw(7)  << setfill(' ') << right << "All s" << " ";
	
	os << "#" << endl;
}
