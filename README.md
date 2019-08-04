LemmaGen Slovene Lemmatizer module
===========================================
[![Build Status](https://travis-ci.org/izacus/SlovenianLemmatizer.svg?branch=master)](https://travis-ci.org/izacus/SlovenianLemmatizer)

This is a lemmatizer for slovene language.

The file directory structure is as follows:

```
data/         - Binary language dictionary files required by the lemmatizer
..about.txt   - Data file key

src/          - Lemmatizer C++ source

Makefile      - Makefile to build lemmatizer .so file on Linux systems
```

Compilation
----------------

Calling "make" will build
 * `libLemmatizer.so` - shared library for use with Java, Python and Solr.
 * `lemmatizer`         executable to lemmatize words from commandline


Contributors:
----------------

Original version form Joseph Stephan Institute (http://lemmatise.ijs.si/)

* Jernej Virag
* Domen Grabec
* Gašper Žejn
