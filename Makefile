CXX := g++
CXXFLAGS := -O3 -fPIC -fomit-frame-pointer -ffast-math -march=native

SRCDIR := ./src
BINDIR := ./bin
OBJECTS := $(SRCDIR)/RdrLemmatizer.o $(SRCDIR)/sl_lemmatizer.o $(SRCDIR)/sl_java_headers.o
JAVAPATH := ./bindings/java/headers
LIBNAME := libLemmatizer.so
EXECNAME := lemmatizer

all: java library testExecutable

$(SRCDIR)/%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

library: $(OBJECTS)
	$(CXX) $(OBJECTS) -shared -fPIC -o $(BINDIR)/$(LIBNAME)

testExecutable: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.c -o $(SRCDIR)/main.o
	$(CXX) $(OBJECTS) $(SRCDIR)/main.o -o $(BINDIR)/$(EXECNAME)

java:
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(JAVAPATH) -I$(JAVAPATH)/linux -c $(JAVAPATH)/sl_lemmatizer_java.cpp -o $(SRCDIR)/sl_java_headers.o

clean:
	rm $(SRCDIR)/*.o
	rm $(BINDIR)/$(LIBNAME)
	rm $(BINDIR)/$(EXECNAME)
