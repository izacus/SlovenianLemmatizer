LemmaGen Slovene Lemmatizer module
===========================================
[![Build Status](https://travis-ci.org/izacus/SlovenianLemmatizer.svg?branch=master)](https://travis-ci.org/izacus/SlovenianLemmatizer)

This is a lemmatizer for slovene language.

The file directory structure:

```
data/         - Binary language dictionary files required by the lemmatizer (about.txt describes each filename)
src/          - Lemmatizer C++ source

CMakeList.txt  - CMake build script to build the lemmatizer library
```

Compilation
----------------

Calling "make" will build
 * `libLemmatizer.so` - shared library with exported symbols for other bindings

Other language bindings
---------------

**Python**: [SlovenianLemmatizer-Python](https://github.com/izacus/SlovenianLemmatizer-Python)


Contributors:
----------------

Original version from Jožef Stefan Institute (http://lemmatise.ijs.si/)

* Jernej Virag
* Domen Grabec
* Gašper Žejn
