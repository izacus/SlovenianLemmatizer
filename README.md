LemmaGen Slovene Lemmatizer module
===========================================
[![Build Status](https://travis-ci.org/izacus/SlovenianLemmatizer.svg?branch=master)](https://travis-ci.org/izacus/SlovenianLemmatizer)

This is a lemmatizer for slovene language.

The file directory structure is as follows:

```
bindings/
..java/       - Java JNI bindings and Solr 4.x module
..python/     - Python bindings

data/         - Binary language dictionary files required by the lemmatizer
..about.txt   - Data file key

src/          - Lemmatizer C++ source

Makefile      - Makefile to build lemmatizer .so file on Linux systems
pom.xml       - Maven POM to build Solr module and JNI binding JAR
```

Compilation
----------------

Calling "make" will build
 * `libLemmatizer.so` - shared library for use with Java, Python and Solr.
 * `lemmatizer`         executable to lemmatize words from commandline

Solr/Java JAR is built by calling `mvn compile` in directory with `pom.xml`.


Contributors:
----------------

Original version form Joseph Stephan Institute (http://lemmatise.ijs.si/)

* Jernej Virag
* Domen Grabec
* Gašper Žejn
