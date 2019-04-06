"""
******************************************************************************
This file is part of the lemmagen library. It gives support for lemmatization.
Copyright (C) 2011 Jernej Virag <jernej@virag.si>

The lemmagen library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************
"""

import os
import sys
import sysconfig

import lemmagen
from lemmagen import libLemmagen

IS_PYTHON3 = sys.version_info > (3, )

class Lemmatizer(object):

    def __init__(self, dictionary=lemmagen.DICTIONARY_SLOVENE, library_path=None):
        """
        @param dictionary: Path to lemmatizer dictionary file
        """

        this_dir = os.path.dirname(os.path.realpath(__file__))
        if not dictionary.startswith("/"):
            dictionary = os.path.join(this_dir, dictionary)

        self.lemmagen = libLemmagen.Lemmagen(bytes(dictionary, "UTF-8") if IS_PYTHON3 else dictionary)

    def lemmatize(self, word):
        if word is None:
            return None

        if IS_PYTHON3:
            return self._lemmatize_py3(word)
        else:
            return self._lemmatize_py2(word)

    def _lemmatize_py3(self, word):
        word_b = bytes(word, 'UTF-8')
        return str(self.lemmagen.lemmatize(word_b), 'UTF-8')

    def _lemmatize_py2(self, word):
        is_unicode = isinstance(word, unicode)
        if is_unicode:
            word = word.encode('utf-8')
        return self.lemmagen.lemmatize(word).decode('utf-8') if is_unicode else self.lemmagen.lemmatize(word)
