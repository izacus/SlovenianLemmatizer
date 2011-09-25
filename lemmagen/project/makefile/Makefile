SRCPATH=../../source/source
HDRPATH=../../source/header
MAINPATH=../../source/main
DEFPATH=../../source/definition
INTPATH=../../source/interface
OBJPATH=../../binary/linux/object
BINPATH=../../binary/linux

#.....................................

OBJSCANNER=\
	$(OBJPATH)/RdrScanner.o \
	$(OBJPATH)/RdrLexer.o \
	$(OBJPATH)/RdrParser.o
OBJRDRTREE=\
	$(OBJPATH)/RdrTree.o \
	$(OBJPATH)/RdrRule.o
OBJWORDRULE=\
	$(OBJPATH)/WordList.o \
	$(OBJPATH)/Word.o \
	$(OBJPATH)/Rule.o
OBJLEARNALG=\
	$(OBJPATH)/BaseAlg.o \
	$(OBJPATH)/BaseNode.o \
	$(OBJPATH)/CoverNodeAlg.o
OBJINTER=\
	$(OBJPATH)/InterLearn.o \
	$(OBJPATH)/InterBuild.o \
	$(OBJPATH)/InterLemtz.o \
	$(OBJPATH)/InterXval.o \
	$(OBJPATH)/InterSplit.o \
	$(OBJPATH)/InterTest.o \
	$(OBJPATH)/InterStat.o
OBJEXECUTABLE=\
	$(OBJPATH)/lemmagen.o \
	$(OBJPATH)/lemLearn.o \
	$(OBJPATH)/lemBuild.o \
	$(OBJPATH)/lemmatize.o \
	$(OBJPATH)/lemXval.o \
	$(OBJPATH)/lemSplit.o \
	$(OBJPATH)/lemTest.o \
	$(OBJPATH)/lemStat.o \
#.....................................

OBJALL=\
	$(OBJPATH)/MiscLib.o \
	$(OBJWORDRULE) \
	$(OBJLEARNALG) \
	$(OBJPATH)/RdrLemmatizer.o \
	$(OBJRDRTREE) \
	$(OBJSCANNER) \
	$(OBJPATH)/CmdLineParser.o \
	$(OBJPATH)/Statistics.o \
	$(OBJPATH)/Xval.o \
	$(OBJINTER) \
	$(OBJPATH)/InterAll.o \
	$(OBJEXECUTABLE)

#.....................................

BINALL=\
	$(BINPATH)/lemmagen \
	$(BINPATH)/lemLearn \
	$(BINPATH)/lemBuild \
	$(BINPATH)/lemmatize \
	$(BINPATH)/lemXval \
	$(BINPATH)/lemSplit \
	$(BINPATH)/lemTest \
	$(BINPATH)/lemStat \

#.....................................

# add -g, -ggdb: debug mode
# add -w: inhibit all warning messages
# add -O3: optimization
DEBUG=
OPT=-O3 -fPIC
GPP=g++ $(DEBUG) -w $(OPT) 

#####################################################################

all : lemmagen \
	lemLearn \
	lemBuild \
	lemmatize \
	lemXval \
	lemSplit \
	lemTest \
	lemStat

lemmagen : $(BINPATH)/lemmagen	
lemLearn : $(BINPATH)/lemLearn
lemBuild : $(BINPATH)/lemBuild
lemmatize : $(BINPATH)/lemmatize
lemXval : $(BINPATH)/lemXval
lemSplit : $(BINPATH)/lemSplit
lemTest : $(BINPATH)/lemTest
lemStat : $(BINPATH)/lemStat

clean : cleanobjects \
	cleanbin
cleanbin:
	rm -f $(BINALL)
cleanobjects:
	rm -f $(OBJALL)
	
#####################################################################
	
$(BINPATH)/lemmagen : $(OBJPATH)/lemmagen.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJLEARNALG) \
	             $(OBJPATH)/RdrLemmatizer.o \
	             $(OBJRDRTREE) \
	             $(OBJSCANNER) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/Statistics.o \
	             $(OBJPATH)/Xval.o \
	             $(OBJINTER) \
	             $(OBJPATH)/InterAll.o

$(OBJPATH)/lemmagen.o : $(MAINPATH)/lemmagen.cpp \
	                $(OBJPATH)/InterAll.o
	$(GPP) -c -o $@ $<

$(OBJPATH)/InterAll.o : $(INTPATH)/InterAll.cpp \
	                $(INTPATH)/Interface.h\
	                $(OBJINTER)
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------

$(BINPATH)/lemLearn : $(OBJPATH)/lemLearn.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJLEARNALG) \
	             $(OBJPATH)/RdrLemmatizer.o \
	             $(OBJRDRTREE) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/InterLearn.o

$(OBJPATH)/lemLearn.o : $(MAINPATH)/lemLearn.cpp \
	                $(OBJPATH)/InterLearn.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/InterLearn.o : $(INTPATH)/InterLearn.cpp \
	                  $(INTPATH)/Interface.h\
	                  $(OBJPATH)/BaseAlg.o \
	                  $(OBJPATH)/CoverNodeAlg.o \
	                  $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------

$(BINPATH)/lemBuild : $(OBJPATH)/lemBuild.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJPATH)/RdrLemmatizer.o \
	             $(OBJRDRTREE) \
	             $(OBJSCANNER) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/InterBuild.o

$(OBJPATH)/lemBuild.o : $(MAINPATH)/lemBuild.cpp \
	                $(OBJPATH)/InterBuild.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/InterBuild.o : $(INTPATH)/InterBuild.cpp \
	                  $(INTPATH)/Interface.h \
	                  $(OBJPATH)/RdrScanner.o \
	                  $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------

$(BINPATH)/lemmatize : $(OBJPATH)/lemmatize.o
	$(GPP) -o $@ $< \
	             $(OBJRDRTREE) \
	             $(OBJPATH)/RdrLemmatizer.o \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJSCANNER) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/InterLemtz.o

$(OBJPATH)/lemmatize.o : $(MAINPATH)/lemmatize.cpp \
	                 $(OBJPATH)/InterLemtz.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/InterLemtz.o : $(INTPATH)/InterLemtz.cpp \
	                  $(INTPATH)/Interface.h \
	                  $(OBJPATH)/RdrLemmatizer.o \
	                  $(OBJPATH)/RdrScanner.o \
	                  $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------

$(BINPATH)/lemXval : $(OBJPATH)/lemXval.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJLEARNALG) \
	             $(OBJPATH)/RdrLemmatizer.o \
	             $(OBJRDRTREE) \
	             $(OBJSCANNER) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/Xval.o \
	             $(OBJPATH)/InterXval.o

$(OBJPATH)/lemXval.o : $(MAINPATH)/lemXval.cpp \
	               $(OBJPATH)/InterXval.o
	$(GPP) -c -o $@ $<

$(OBJPATH)/InterXval.o : $(INTPATH)/InterXVal.cpp \
	                 $(INTPATH)/Interface.h \
	                 $(OBJPATH)/Xval.o \
	                 $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/Xval.o : $(SRCPATH)/Xval.cpp \
	            $(HDRPATH)/Xval.h \
	            $(OBJPATH)/RdrScanner.o \
	            $(OBJPATH)/BaseAlg.o \
	            $(OBJPATH)/CoverNodeAlg.o
	$(GPP) -c -o $@ $<	
	
#--------------------------------------------------------------------

$(BINPATH)/lemSplit : $(OBJPATH)/lemSplit.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/InterSplit.o

$(OBJPATH)/lemSplit.o : $(MAINPATH)/lemSplit.cpp \
	                $(OBJPATH)/InterSplit.o
	$(GPP) -c -o $@ $<

$(OBJPATH)/InterSplit.o : $(INTPATH)/InterSplit.cpp \
	                  $(INTPATH)/Interface.h \
	                  $(OBJPATH)/WordList.o \
	                  $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------

$(BINPATH)/lemTest : $(OBJPATH)/lemTest.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/InterTest.o

$(OBJPATH)/lemTest.o : $(MAINPATH)/lemTest.cpp \
	               $(OBJPATH)/InterTest.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/InterTest.o : $(INTPATH)/InterTest.cpp \
	                 $(INTPATH)/Interface.h \
	                 $(OBJPATH)/WordList.o \
	                 $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<
#--------------------------------------------------------------------

$(BINPATH)/lemStat : $(OBJPATH)/lemStat.o
	$(GPP) -o $@ $< \
	             $(OBJPATH)/MiscLib.o \
	             $(OBJWORDRULE) \
	             $(OBJPATH)/CmdLineParser.o \
	             $(OBJPATH)/Statistics.o \
	             $(OBJPATH)/InterStat.o

$(OBJPATH)/lemStat.o : $(MAINPATH)/lemStat.cpp \
	               $(OBJPATH)/InterStat.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/InterStat.o : $(INTPATH)/InterStat.cpp \
	                 $(INTPATH)/Interface.h \
	                 $(OBJPATH)/Statistics.o \
	                 $(OBJPATH)/CmdLineParser.o
	$(GPP) -c -o $@ $<
	
$(OBJPATH)/Statistics.o : $(SRCPATH)/Statistics.cpp \
	                  $(HDRPATH)/Statistics.h \
	                  $(OBJPATH)/WordList.o
	$(GPP) -c -o $@ $<
	
	
#####################################################################

# parsing tree files and building tree, optimizing, generating lemmatizator

$(OBJPATH)/RdrScanner.o : $(SRCPATH)/RdrScanner.cpp \
	                  $(HDRPATH)/RdrScanner.h \
	                  $(OBJPATH)/RdrLexer.o
	$(GPP) -c -o $@ $<	

$(OBJPATH)/RdrLexer.o : $(SRCPATH)/RdrLexer.cpp \
	                $(HDRPATH)/RdrLexer.h \
	                $(OBJPATH)/RdrParser.o
	$(GPP) -c -o $@ $<	

$(OBJPATH)/RdrParser.o : $(SRCPATH)/RdrParser.cpp \
	                 $(HDRPATH)/RdrParser.h \
	                 $(OBJPATH)/RdrTree.o
	$(GPP) -c -o $@ $<	

$(OBJPATH)/RdrTree.o : $(SRCPATH)/RdrTree.cpp \
	               $(HDRPATH)/RdrTree.h \
	               $(OBJPATH)/RdrRule.o
	$(GPP) -c -o $@ $<	

$(OBJPATH)/RdrRule.o : $(SRCPATH)/RdrRule.cpp \
	               $(HDRPATH)/RdrRule.h \
	               $(OBJPATH)/RdrLemmatizer.o \
	               $(OBJPATH)/MiscLib.o
	$(GPP) -c -o $@ $<	

#--------------------------------------------------------------------
# generating lexer and parser from definition files

#TODO generate RDRLexer and RDRParser	
$(HDRPATH)/RdrLexer.h : $(DEFPATH)/RdrLexer.def
$(SRCPATH)/RdrLexer.cpp : $(DEFPATH)/RdrLexer.def
$(HDRPATH)/RdrParser.h : $(DEFPATH)/RdrParser.def
$(SRCPATH)/RdrParser.cpp : $(DEFPATH)/RdrParser.def

#--------------------------------------------------------------------	
# old, joel's learning algorithm
	
$(OBJPATH)/BaseAlg.o : $(SRCPATH)/BaseAlg.cpp \
	               $(HDRPATH)/BaseAlg.h \
	               $(OBJPATH)/BaseNode.o
	$(GPP) -c -o $@ $<	

$(OBJPATH)/BaseNode.o : $(SRCPATH)/BaseNode.cpp \
	                $(HDRPATH)/BaseNode.h \
	                $(OBJPATH)/WordList.o \
	                $(OBJPATH)/RdrTree.o 
	$(GPP) -c -o $@ $<	
	
#--------------------------------------------------------------------	
# new learning algorithm
$(OBJPATH)/CoverNodeAlg.o : $(SRCPATH)/CoverNodeAlg.cpp \
	                    $(HDRPATH)/CoverNodeAlg.h \
	                    $(OBJPATH)/WordList.o \
	                    $(OBJPATH)/RdrTree.o 
	$(GPP) -c -o $@ $<	

#--------------------------------------------------------------------	
# general support for multext file type - word, rule, wordlist

$(OBJPATH)/WordList.o : $(SRCPATH)/WordList.cpp \
	                $(HDRPATH)/WordList.h \
	                $(OBJPATH)/Word.o \
	                $(OBJPATH)/Rule.o
	$(GPP) -c -o $@ $<	
	
$(OBJPATH)/Word.o :  $(SRCPATH)/Word.cpp \
	             $(HDRPATH)/Word.h \
	             $(OBJPATH)/MiscLib.o
	$(GPP) -c -o $@ $<

$(OBJPATH)/Rule.o : $(SRCPATH)/Rule.cpp \
	            $(HDRPATH)/Rule.h \
	            $(OBJPATH)/MiscLib.o
	$(GPP) -c -o $@ $<	
	
#####################################################################		
# commandline argument parser

$(OBJPATH)/CmdLineParser.o : $(SRCPATH)/CmdLineParser.cpp \
	                     $(HDRPATH)/CmdLineParser.h \
	                     $(OBJPATH)/MiscLib.o
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------	
# lemamtizer completley independent from other packages

$(OBJPATH)/RdrLemmatizer.o : $(SRCPATH)/RdrLemmatizer.cpp \
	                     $(HDRPATH)/RdrLemmatizer.h 
	$(GPP) -c -o $@ $<	

#--------------------------------------------------------------------	
# some standard functions used everywhere

$(OBJPATH)/MiscLib.o : $(SRCPATH)/MiscLib.cpp \
	            $(HDRPATH)/MiscLib.h
	$(GPP) -c -o $@ $<

#--------------------------------------------------------------------		

