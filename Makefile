CXX := g++
CXXFLAGS := -O3 -fPIC

SRCDIR := ./src
OBJECTS := $(SRCDIR)/RdrLemmatizer.o
LIBNAME := libLemmatizer.so

$(SRCDIR)/%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

lemmatizer: $(OBJECTS)
	$(CXX) $(OBJECTS) -shared -fPIC -o $(LIBNAME)

all: lemmatizer

clean:
	rm $(SRCDIR)/*.o
	rm $(LIBNAME)
