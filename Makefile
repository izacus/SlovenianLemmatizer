CXX := g++
CPP := cc
CXXFLAGS := -O3 -fPIC
CPPFLAGS := -O3

SRCDIR := ./src
OBJECTS := $(SRCDIR)/RdrLemmatizer.o $(SRCDIR)/lemmatizer.o
LIBNAME := libLemmatizer.so
EXECNAME := lemmatizer

all: library testExecutable

$(SRCDIR)/%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

library: $(OBJECTS)
	$(CXX) $(OBJECTS) -shared -fPIC -o $(LIBNAME)

testExecutable: $(OBJECTS)
	$(CXX) $(CPPFLAGS) -c $(SRCDIR)/main.c -o $(SRCDIR)/main.o
	$(CXX) $(OBJECTS) $(SRCDIR)/main.o -o $(EXECNAME)

clean:
	rm $(SRCDIR)/*.o
	rm $(LIBNAME)
	rm $(EXECNAME)
