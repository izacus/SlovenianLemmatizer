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
#include "../header/Xval.h"

//-------------------------------------------------------------------------------------------	
//main function for cross validation
int Xval::PerformXval(string sInfile, string sOutfile, int iSplit, int iType, int iAlg, 
					int iRandSeed, bool bVerbose, bool bSilent, int iNumRuns) {
	
	bool bVerb = !bSilent && bVerbose;
	bool bSlnt = bSilent && bVerbose;
	bool bEmptyTest = iType>1 && iSplit==0;

	//timing
	Timing timeAll;

	//import files	
	WordList *wlstOrig  = new WordList(sInfile.c_str(), NULL, bVerb);

	//count runs
	int iSetNum = (iType<2 ? iSplit : 1);
	int iAllRuns = iSetNum * iNumRuns;

	//averarge accurancies
	double dAccTest;
	double dAccTrain;
	double dTime;
	double (*adAccTime)[3][2] = new double[iAllRuns][3][2];

	//output file if needed or dummy if not
	ostream *os = new stringstream();
	if (sOutfile != "") os = new ofstream(sOutfile.c_str(), ofstream::app | ofstream::out);

	//create head lines
	CreateHeadLines(*os, (iAlg==2?2:1));

	//timing
	Timing timeXvalAll;

	//repeat for each run
	for(int iRun=0; iRun<iNumRuns; iRun++){
		WordList *wlstOneRun  = new WordList(*wlstOrig);

		//timing
		Timing timeXval;

		//randomization
		Rand::seed(iRandSeed+iRun);

		//split on sets
		if (iType<2)
			wlstOneRun->SplitCrossValid(iSplit, iType, iRun==0 ? bVerb*2 : 0);
		else
			wlstOneRun->SplitSubset(iSplit, iType%2, iRun==0 ? bVerb*2 : 0);

		//sort if needed
		WordList *wlstSort = wlstOneRun;
		if (iAlg == 2) wlstSort = new WordList(*wlstOneRun);
		if (iAlg == 2 || iAlg == 0) wlstSort->Sort(iRun==0 ? bVerb : 0);

		//wlstOneRun->Export("notsorted_test.txt",0,0,true);
		//wlstSort->Export("sorted_test.txt",0,0,true);

		//doit
		if (bSlnt) cout << sInfile.substr( max(0, (int)(sInfile.length()-18)) ) << " (" << iRun+1 << ")" << ": ";
		if (bVerb && iRun==0) cout << "  Test started..." << endl;

		for (int iSet = 0; iSet < iSetNum; ++iSet) {
			int iStatElem = iRun*iSetNum+iSet;
			Timing time;
			//--------------------------------------------------------------------------
			//print out initial data
			*os << "#" << setw(45) << setfill(' ') << right << sInfile << " ";
			*os << "|" << setw(6)  << setfill(' ') << right << 1 << " ";
			*os << "|" << setw(6)  << setfill(' ') << right << iSet << " ";
			*os << "|" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec(false) << " ";
			*os << "#" << setw(7)  << setfill(' ') << right << wlstOneRun->GetWordNum(-1) << " ";
			*os << "|" << setw(7)  << setfill(' ') << right << wlstOneRun->GetWordNum(iSet, false) << " ";
			*os << "|" << setw(7)  << setfill(' ') << right << wlstOneRun->GetWordNum(iSet, true) << " ";
			//--------------------------------------------------------------------------

			if (bSlnt) cout << iSet+1 << "";
			if (bVerb) cout << setfill('0') << "    Run " << setw(2) << iRun+1 << ", ";
			if (bVerb) cout << "Set " << setw(2) << iSet+1 << " - " << setfill(' ');

			//covering algorithm
			if (iAlg!=1) {
				*os << "##";
				if (bVerb) {cout << "cover:["; cout.flush();};

				Timing timeThis;
				CrossValidCvr(*os, *wlstSort, iSet, bVerbose, dAccTest, dAccTrain);
				dTime = timeThis.ElapSec(true);

				adAccTime[iStatElem][0][0] = dAccTrain;
				adAccTime[iStatElem][1][0] = dAccTest;
				adAccTime[iStatElem][2][0] = dTime;

				if (bVerb) {
					cout << "] "; 
					if (!bEmptyTest) cout << setprecision(2) << fixed << dAccTest << "% ";
					cout << setprecision(2) << fixed << dAccTrain << "% ";
					cout << dTime << "s ";
				}
			}


			if (bSlnt && iAlg==2) cout << "|";
			cout.flush();

			//original algorithm
			if (iAlg!=0) {
				*os << "##";
				if (bVerb) {cout << "rdr:["; cout.flush();};

				Timing timeThis;
				CrossValidRdr(*os, *wlstOneRun, iSet, bVerbose, dAccTest, dAccTrain);
				dTime = timeThis.ElapSec(true);

				adAccTime[iStatElem][0][1] = dAccTrain;
				adAccTime[iStatElem][1][1] = dAccTest;
				adAccTime[iStatElem][2][1] = dTime;

				if (bVerb) {
					cout << "] "; 
					if (!bEmptyTest) cout << setprecision(2) << fixed << dAccTest << "% ";
					cout << setprecision(2) << fixed << dAccTrain << "% ";
					cout << dTime << "s ";

				}
			}

			if (bSlnt) cout << "";
			if (bVerb) cout << endl;


			//--------------------------------------------------------------------------
			//close this line
			*os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec(true) << " ";
			*os << "#" << endl;
			//--------------------------------------------------------------------------
		}

		if (bSlnt) cout << " " << setprecision(1) << fixed << timeXval.ElapSec() << "s" << endl;

		
		//cleanup
		if (wlstOneRun != wlstSort) {
			delete wlstOneRun;
			delete wlstSort;
		} else
			delete wlstOneRun;
	}

	//cleanup
	delete wlstOrig;

	//write statistics
	if (!bSilent) {
		//algorithms and properties names
		string asAlgName[2] = {"Cover algorithm:", "Rdr algorithm:"};
		string asPropName[3] = {"Train set", "Test set ", "Time     "};
		string asPropUnit[3] = {"%", "%", "s"};

		cout << "  Statistics (avg +-stdev):" << endl;
		cout << "    Runs       : " << iNumRuns << endl;
		cout << "    Splits     : " << (iType/2==0?"cross validation (":"random split (") << iSplit << (iType/2==0?"x)":"% in test set)") << endl;
		cout << "    Experiments: " << iAllRuns << endl;

		for (int iAlgId = iAlg%2; iAlgId<2 && iAlgId<iAlg+1; iAlgId++) {
			cout << "    " << asAlgName[iAlgId] << endl;

			//foreach property
			for(int iProp=0; iProp<3; iProp++) {
				//calculate average
				double adAccTimeAvg = 0;
				for (int iStatElem = 0; iStatElem < iAllRuns; iStatElem++) 
					adAccTimeAvg += adAccTime[iStatElem][iProp][iAlgId];
				adAccTimeAvg = adAccTimeAvg/iAllRuns;

				//calculate variance
				double adAccTimeVar = 0;
				for (int iStatElem = 0; iStatElem < iAllRuns; iStatElem++) 
					adAccTimeVar += pow(adAccTimeAvg - adAccTime[iStatElem][iProp][iAlgId],2);
				adAccTimeVar = sqrt(adAccTimeVar/(iAllRuns-1));

				//output statistics
				if (adAccTimeAvg != -1) {
					cout << "      " + asPropName[iProp] +": ";
					cout << setprecision(4) << fixed << right << setw(7) << adAccTimeAvg << asPropUnit[iProp];
					if (iAllRuns > 1) cout << " +-" << right << setw(7) << adAccTimeVar << asPropUnit[iProp];
					cout << endl;
				}
			}
		}
	}
	delete[] adAccTime;


	//close the file
	if (sOutfile != "")
		((ofstream*)os)->close();
	delete os;

	//write final message
	if (bVerb) {
		if (sOutfile != "")
			 cout << "  Results writen to file '" << sOutfile << "'." << endl;
		timeXvalAll.Print(cout, "  Time needed for all tests ");
		timeAll.Print(cout, "  Time needed altogether ");
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
void Xval::CrossValidCvr(ostream &os, WordList &wlst, int iTestSet,
								bool bVerbose, double &dAccTest, double &dAccTrain) {
	
	WordList::iterator *itBegin, *itEnd;
									
	//learn with my algorithm	
	CoverNodeAlg *nStart = CoverNodeAlg::BuildTree(wlst,iTestSet,false,2,os);
	nStart->PrintStat(os, 2);
	if(bVerbose) {cout << "."; cout.flush();} //1

	//--------------------------------------------------------------------------
	//test accurancy with my algorithm	
	dAccTrain = nStart->TestAccurancy(*(itBegin = wlst.begin(iTestSet,false)),*(itEnd = wlst.end(iTestSet,false)),2,os);
	delete itBegin;	delete itEnd;
	dAccTest = nStart->TestAccurancy(*(itBegin = wlst.begin(iTestSet,true)),*(itEnd = wlst.end(iTestSet,true)),2,os);
	delete itBegin;	delete itEnd;
	if(bVerbose) {cout << "."; cout.flush();} //2

	//--------------------------------------------------------------------------
	//print tree representation to memory
	stringstream ssCvr;
	RdrTree *rt = nStart->GenRdrTree();
	rt->Print(NICE_IF, ssCvr);
	delete rt;
	if(bVerbose) {cout << "."; cout.flush();} //3

	//--------------------------------------------------------------------------
	//test almost universaly from here on for cover and rdr algorithm
	if (!TestRdrTree(ssCvr, wlst, os, iTestSet)) 
		{ssCvr.seekg(ios_base::beg); os << ssCvr.str(); os.flush(); return;}
	if(bVerbose) {cout << "."; cout.flush();} //4

	//--------------------------------------------------------------------------
	//clean after cover part of lematization
	delete nStart;
	ssCvr.flush();
}

//-------------------------------------------------------------------------------------------	
void Xval::CrossValidRdr(ostream &os, WordList &wlst, int iTestSet,
								bool bVerbose, double &dAccTest, double &dAccTrain) {
	
	WordList::iterator *itBegin, *itEnd;
	
	//learn with rdr algorithm	
	BaseAlg *rdrRdr = new BaseAlg();
	rdrRdr->train(*(itBegin = wlst.begin(iTestSet,false)),*(itEnd = wlst.end(iTestSet,false)),2,os);
	delete itBegin;	delete itEnd;
	rdrRdr->PrintStat(os, 2);
	if(bVerbose) {cout << "."; cout.flush();} //1

	//--------------------------------------------------------------------------
	//test accurancy with rdr algorithm	
	dAccTrain = rdrRdr->lemmatize(*(itBegin = wlst.begin(iTestSet,false)),*(itEnd = wlst.end(iTestSet,false)),2,os);
	delete itBegin;	delete itEnd;
	dAccTest = rdrRdr->lemmatize(*(itBegin = wlst.begin(iTestSet,true)),*(itEnd = wlst.end(iTestSet,true)),2,os);
	delete itBegin;	delete itEnd;
	if(bVerbose) {cout << "."; cout.flush();} //2

	//--------------------------------------------------------------------------
	//print tree representation to memory
	stringstream ssRdr;
	RdrTree *rt = rdrRdr->GenRdrTree();
	rt->Print(NICE_IF, ssRdr);
	delete rt;
	if(bVerbose) {cout << "."; cout.flush();} //3

	//--------------------------------------------------------------------------
	//test almost universaly from here on for cover and rdr algorithm
	if (!TestRdrTree(ssRdr, wlst, os, iTestSet))
		{ssRdr.seekg(ios_base::beg); os << ssRdr.str(); os.flush(); return;}
	if(bVerbose) {cout << "."; cout.flush();} //4

	//--------------------------------------------------------------------------
	//clean after rdr part of lematization
	delete rdrRdr;
	ssRdr.flush();
}

//-----------------------------------------------------------------------------
bool Xval::TestRdrLemm(WordList::iterator &itStart, WordList::iterator &itEnd, RdrLemmatizer &rdrLemm, ostream &os) {

	int iTrue = 0;
	char *acOutBuff = new char[255];

	Timing time;

	for (WordList::iterator it = itStart; it != itEnd; ++it) {
		rdrLemm.Lemmatize(it->acWord, acOutBuff);
		if (strcmp(acOutBuff, it->acLemm) == 0)	
			iTrue++;		
	}

	double dAccurancy = (double) iTrue*100 / itStart.GetWordNum();
	double dTimeNs = time * 1000000000 / itStart.GetWordNum();

	os << "#" << setw(10) << setfill(' ') << right << fixed << setprecision(6) << dAccurancy << "% ";
	os << "|" << setw(10) << setfill(' ') << right << iTrue << " ";
	os << "|" << setw(8) << setfill(' ') << right << fixed << setprecision(0) << dTimeNs << " ";

	delete[] acOutBuff;

	return true;
}

//-----------------------------------------------------------------------------
bool Xval::TestRdrTree(stringstream &ss, WordList &wlFile, ostream &os, int kSplit) {
	//--------------------------------------------------------------------------
	//create standard tree representation
	ss.seekg(ios_base::beg);
	RdrScanner *scanner = new RdrScanner();
	
	Timing time;
	RdrTree *rdrStart = scanner->scan(&ss, &cout, 0);
	os << "#" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";

	if (!scanner->bSuccess) return false;

	//--------------------------------------------------------------------------
	//create optimized lemmatizer data structure from tree in memory

	RdrLemmatizer *rdrLemm = rdrStart->CreateLemmatizer();
	os << "|" << setw(7) << setfill(' ') << right << fixed << setprecision(2) << time.ElapSec() << " ";
	rdrStart->PrintStat(os,2);

	//--------------------------------------------------------------------------
	//test accurancy with algorithm on new structure
	WordList::iterator *itBegin, *itEnd;
	if (!TestRdrLemm(*(itBegin = wlFile.begin(kSplit,false)), *(itEnd = wlFile.end(kSplit,false)),*rdrLemm,os)) return false;
	delete itBegin;	delete itEnd;
	if (!TestRdrLemm(*(itBegin = wlFile.begin(kSplit,true)), *(itEnd = wlFile.end(kSplit, true)),*rdrLemm,os)) return false;
	delete itBegin;	delete itEnd;

	delete scanner;
	delete rdrStart;
	delete rdrLemm;

	return true;
}

//-----------------------------------------------------------------------------
void Xval::CreateHeadLines(ostream &os, int iTests) {
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
