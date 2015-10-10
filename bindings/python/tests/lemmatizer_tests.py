#!/usr/bin/env python
# -*- coding: utf-8 -*-

from lemmagen.lemmatizer import Lemmatizer
import unittest


class TestLemmatizer(unittest.TestCase):

    def test_lemmatize(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize("hodimo")
        self.assertEqual("hoditi", lemmatized)

    def test_utf8lemmatize(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize(u"čistijo")
        self.assertEqual(u"čistiti", lemmatized)

    def test_emptystring(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize("")
        self.assertEqual("", lemmatized)

    def test_punctuation(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize(u"!\"=`.,/:")
        self.assertEqual(u"!\"=`.,/:", lemmatized)

    def test_null(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize(None)
        self.assertEqual(None, lemmatized)

if __name__ == "__main__":
    unittest.main()