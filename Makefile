CXX := g++
CXXFLAGS := -O3 -fPIC -fomit-frame-pointer -ffast-math -Wno-null-conversion -Wno-null-arithmetic

SRCDIR := ./src
BINDIR := ./bin
OBJECTS := $(SRCDIR)/RdrLemmatizer.o $(SRCDIR)/sl_lemmatizer.o $(SRCDIR)/sl_java_headers.o
JAVAPATH := ./bindings/java/headers
LIBNAME := libLemmatizer
EXECNAME := lemmatizer

all: java library testExecutable

$(SRCDIR)/%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

library: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -shared -fPIC -o $(BINDIR)/$(LIBNAME).so

testExecutable: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.c -o $(SRCDIR)/main.o
	$(CXX) $(OBJECTS) $(SRCDIR)/main.o -o $(BINDIR)/$(EXECNAME)

java:
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(JAVAPATH) -I$(JAVAPATH)/linux -c $(JAVAPATH)/sl_lemmatizer_java.cpp -o $(SRCDIR)/sl_java_headers.o

osx: java testExecutable
	$(CXX) $(OBJECTS) -dynamiclib -fPIC -o $(BINDIR)/$(LIBNAME).dylib

clean:
	rm $(SRCDIR)/*.o
	rm $(BINDIR)/$(LIBNAME).*
	rm $(BINDIR)/$(EXECNAME)
