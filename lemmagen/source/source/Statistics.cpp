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
#include "../header/Statistics.h"

//-------------------------------------------------------------------------------------------	
//main function for statistics
int Statistics::StatAll(vector<string> vsInfile, string sOutfile, string sTextfile, bool bVerbose) {
	//timing
	Timing timeAll;

	stringstream ssOut;
	CreateHeadLines(ssOut, sTextfile!="");
	
	for (int i=0; i<vsInfile.size(); ++i) {
		Timing time;
		//import files	
		WordList wlst(vsInfile[i].c_str(), NULL, bVerbose);

		//print file column
		ssOut << "#" << setw(45)  << setfill(' ') << right << vsInfile[i].c_str() << " ";
		if (bVerbose) cout << "    Calculating statistics for file " << i+1 << "..." << endl;

		CreateStat(wlst, ssOut, (bVerbose || sOutfile==""));

		//if calculating additional statistics
		if(sTextfile != "")
			CoveringStat(wlst, sTextfile, ssOut, (bVerbose || sOutfile==""));

		ssOut << "#" << endl;
		if (bVerbose) time.Print(cout, "    Time needed for this file ");
	}

	if(sOutfile != "") {
		ofstream ofs(sOutfile.c_str());
		ofs << ssOut.str();
		ofs.close();
	}

	if (bVerbose) timeAll.Print(cout, "  Time needed altogether ");

	return 0;

}

//-------------------------------------------------------------------------------------------	
int Statistics::CreateStat(WordList &wls, ostream &os, bool bVerbose){
     
	set<string> ssForm;
	set<string> ssLemma;
	set<string> ssWord;

	typedef set<string> ssmap;
	typedef map<string, ssmap> sssmap;
	sssmap mapWordPerLemma;
	sssmap mapLemmaPerWord;

	int iAll = 0;
	int iWord = 0;
	int iLemma = 0;
	int iForm = 0;
	double dWpl = 0;
	double dLpw = 0;

	WordList::iterator *itBegin, *itEnd;
	for(WordList::iterator it = *(itBegin = wls.begin()); it != *(itEnd = wls.end()); ++it) {
		iAll ++;

		ssWord.insert(it->acWord);
		ssLemma.insert(it->acLemm);
		ssForm.insert(it->acForm);

		mapWordPerLemma[it->acLemm].insert(it->acWord);
		mapLemmaPerWord[it->acWord].insert(it->acLemm);
	}
	delete itBegin;	delete itEnd;

	iWord = ssWord.size();
	iLemma = ssLemma.size();
	iForm = ssForm.size();
	
	for(sssmap::iterator it = mapWordPerLemma.begin(); it != mapWordPerLemma.end(); ++it) 
		dWpl += ((set<string>)(it->second)).size();
	dWpl /= iLemma;

	for(sssmap::iterator it = mapLemmaPerWord.begin(); it != mapLemmaPerWord.end(); ++it) 
		dLpw += ((set<string>)(it->second)).size();
	dLpw /= iWord;

	os << "|" << setw(7)  << setfill(' ') << right << iAll << " ";
	os << "#" << setw(7)  << setfill(' ') << right << iWord << " ";
	os << "|" << setw(7)  << setfill(' ') << right << iLemma << " ";
	os << "|" << setw(7)  << setfill(' ') << right << iForm << " ";
	os << "#" << setw(8)  << setfill(' ') << right << fixed << setprecision(4) << dWpl << " ";
	os << "|" << setw(8)  << setfill(' ') << right << fixed << setprecision(4) << dLpw << " ";

	if (bVerbose) {
		cout << "      --------------------------: ------" << endl;
		cout << "                     file entrys: " << iAll << endl;
		cout << "                           words: " << iWord << endl;
		cout << "                          lemmas: " << iLemma << endl;
		cout << "                           forms: " << iForm << endl;
		cout << "      --------------------------: ------" << endl;
		cout << "      avg. difr. words per lemma: " << fixed << setprecision(4) << dWpl << endl;
		cout << "      avg. difr. lemmas per word: " << fixed << setprecision(6) << dLpw << endl;
		cout << "      --------------------------: ------" << endl;
	}

	return 1;
}

//-------------------------------------------------------------------------------------------	
int Statistics::CoveringStat(WordList &wls, string sTextfile, ostream &os, bool bVerbose){
	typedef map<string, int> mapsi;
	mapsi mTextWords = GetWordsFromFile(sTextfile);
	mapsi mLexWords;

	int iLexWordNum = 0;

	WordList::iterator *itBegin, *itEnd;
	for(WordList::iterator it = *(itBegin = wls.begin()); it != *(itEnd = wls.end()); ++it) {
		iLexWordNum++;

		Str::lower(it->acWord);
		string sWord = it->acWord;

		mapsi::iterator itWord = mLexWords.find(sWord);
		if (itWord == mLexWords.end()) mLexWords.insert(mapsi::value_type(sWord,1));
		else itWord->second++;
	}
	delete itBegin;	delete itEnd;

	int iTextWordNum = 0;
	for(mapsi::iterator it = mTextWords.begin(); it != mTextWords.end(); ++it) 
		iTextWordNum += ((int)(it->second));


	//how many words from lexicon there is in text
	int iLexCovered = 0;
	int iTextCovered = 0;
	int iCoverage = 0;
	for(mapsi::iterator itText = mTextWords.begin(); itText != mTextWords.end(); ++itText) {
		mapsi::iterator itLex = mLexWords.find(itText->first);
		if (itLex != mLexWords.end()) {
			iTextCovered += itText->second;
			iLexCovered += itLex->second;
			iCoverage++;
		} 
	}

	/*
	ofstream ofile1("Lexicon.txt");
	ofstream ofile2("Text.txt");
	for(mapsi::iterator it = mLexWords.begin(); it != mLexWords.end(); ++it) 
		ofile1 << (it->first) << endl;
	for(mapsi::iterator it = mTextWords.begin(); it != mTextWords.end(); ++it) 
		ofile2 << (it->first) << endl;
	ofile1.close();
	ofile2.close();
	*/
	
	double bPercDiffText =  (float)mTextWords.size()/iTextWordNum;
	double bPercDiffLex =  (float)mLexWords.size()/iLexWordNum;
	double dLexCovered = (float)iLexCovered/iLexWordNum;
	double dTextCovered = (float)iTextCovered/iTextWordNum;

	if (bVerbose) {
		cout << "              words in text file: " << iTextWordNum << endl;
		cout << "       diffr. words in text file: " << mTextWords.size() << endl;
		cout << "      difference text percentage: " << fixed << setprecision(2) << bPercDiffText*100 << "%" << endl;
		cout << "      --------------------------: ------" << endl;
		cout << "           words in lexicon file: " << iLexWordNum << endl;
		cout << "      diffr. words in lexi. file: " << mLexWords.size() << endl;
		cout << "       diffr. lexicon percentage: " << fixed << setprecision(2) <<  bPercDiffLex*100 << "%" << endl;
		cout << "      --------------------------: ------" << endl;
		cout << "        same in lexicon and text: " << iCoverage << endl;
		cout << "       percent of lexicon coverd: " << fixed << setprecision(2) <<  dLexCovered*100 << "%" << endl;
		cout << "          percent of text coverd: " << fixed << setprecision(2) <<  dTextCovered*100 << "%" << endl;
		cout << "      --------------------------: ------" << endl;
	}

	os << "#" << setw(9)   << setfill(' ') << right << iTextWordNum << " ";
	os << "|" << setw(9)   << setfill(' ') << right << mTextWords.size() << " ";
	os << "|" << setw(10)  << setfill(' ') << right << fixed << setprecision(4) << bPercDiffText << " ";
	os << "#" << setw(9)   << setfill(' ') << right << iLexWordNum << " ";
	os << "|" << setw(9)   << setfill(' ') << right << mLexWords.size() << " ";
	os << "|" << setw(10)  << setfill(' ') << right << fixed << setprecision(4) <<  bPercDiffLex << " ";
	os << "#" << setw(10)   << setfill(' ') << right << iCoverage << " ";
	os << "|" << setw(10)  << setfill(' ') << right << fixed << setprecision(4) <<  dLexCovered << " ";
	os << "|" << setw(10)  << setfill(' ') << right << fixed << setprecision(4) <<  dTextCovered << " ";

	return 1;
}

//-------------------------------------------------------------------------------------------	
void Statistics::CreateHeadLines(ostream &os, bool bCoverStat) {
	os << "#" << setw(45)  << setfill(' ') << right << "FileName" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Entrys" << " ";
	os << "#" << setw(7)  << setfill(' ') << right << "Words" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Lemmas" << " ";
	os << "|" << setw(7)  << setfill(' ') << right << "Forms" << " ";
	os << "#" << setw(8)  << setfill(' ') << right << "W/L" << " ";
	os << "|" << setw(8)  << setfill(' ') << right << "L/W" << " ";

	if (bCoverStat) {
		os << "#" << setw(9)   << setfill(' ') << right << "TextWrd" << " ";
		os << "|" << setw(9)   << setfill(' ') << right << "DifTxW" << " ";
		os << "|" << setw(10)  << setfill(' ') << right << "TextDif%" << " ";
		os << "#" << setw(9)   << setfill(' ') << right << "LexcWrd" << " ";
		os << "|" << setw(9)   << setfill(' ') << right << "DifLxW" << " ";
		os << "|" << setw(10)  << setfill(' ') << right << "LexcDif%" << " ";
		os << "#" << setw(10)   << setfill(' ') << right << "MatchDif" << " ";
		os << "|" << setw(10)  << setfill(' ') << right << "LexcCovr" << " ";
		os << "|" << setw(10)  << setfill(' ') << right << "TextCovr" << " ";
	}

	os << "#" << endl;
}

//-------------------------------------------------------------------------------------------	
//returns words from file including their frequencies
map<string, int> Statistics::GetWordsFromFile(string sFileName) {
	typedef map<string, int> mapsi;
	mapsi mWords;

	int         iWordNum = 0;
	int         iLine = 0;
	const int	MAX_LEN = 10000;
	char *acBuf = new char[MAX_LEN*2];

	ifstream	ifs(sFileName.c_str());

	// read lines from file
	while (!ifs.eof()) {
		iLine++;
		ifs.getline(acBuf, MAX_LEN, '\n');

		//get words from one line
		char *acStart = acBuf;
		
		for(int i=0; i==0 || (acBuf[i-1]!='\n' && acBuf[i-1]!=NULL); ++i) {
			if (WORDCHARS)	continue;

			//we are at the delimiter
			if (acStart < &(acBuf[i])) {
				iWordNum++;
				
				//get word
				char ch = acBuf[i];
				acBuf[i] = 0;
				Str::lower(acStart);
				string sWord = string(acStart);
				acBuf[i] = ch;

				//save word by word
				mapsi::iterator itWord = mWords.find(sWord);
				if (itWord == mWords.end()) mWords.insert(mapsi::value_type(sWord,1));
				else itWord->second++;
			}

			acStart = &(acBuf[i+1]);
		}
	}
	ifs.close();

	return mWords;
}
