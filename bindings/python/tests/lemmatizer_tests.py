#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from lemmagen.lemmatizer import Lemmatizer
import unittest

class TestLemmatizer(unittest.TestCase):

    def test_lemmatize(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize(str("hodimo"))
        self.assertEqual(str("hoditi"), lemmatized)

    def test_utf8lemmatize(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize("čistijo")
        self.assertEqual("čistiti", lemmatized)

    def test_emptystring(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize("")
        self.assertEqual("", lemmatized)

    def test_punctuation(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize("!\"=`.,/:")
        self.assertEqual("!\"=`.,/:", lemmatized)

    def test_null(self):
        lemmatizer = Lemmatizer()
        lemmatized = lemmatizer.lemmatize(None)
        self.assertEqual(None, lemmatized)

if __name__ == "__main__":
    unittest.main()