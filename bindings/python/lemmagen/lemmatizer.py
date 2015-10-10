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

from ctypes import cdll, create_string_buffer
import os

import sys

import lemmagen
import sysconfig

IS_PYTHON3 = sys.version_info > (3, )


class Lemmatizer(object):
    STATUS_OK = 0
    STATUS_FILE_NOT_FOUND = -1

    def __init__(self, dictionary=lemmagen.DICTIONARY_SLOVENE, library_path=None):
        """
        @param dictionary: Path to lemmatizer dictionary file
        """

        this_dir = os.path.dirname(os.path.realpath(__file__))
        if library_path is None:
            self._lib = cdll.LoadLibrary(os.path.join(this_dir, "libLemmagen%s" % self._get_library_extension()))
        else:
            self._lib = cdll.LoadLibrary(library_path)

        if not dictionary.startswith("/"):
            dictionary = os.path.join(this_dir, dictionary)

        # We need to convert parameter to bytes in python 3
        result = self._lib.lem_load_language_library(bytes(dictionary, "UTF-8") if IS_PYTHON3 else dictionary)

        if result == self.STATUS_FILE_NOT_FOUND:
            raise IOError("Lemmatizer dictionary file could not be found.")
        assert(result == self.STATUS_OK)

        self._output_buffer_len = 64
        self._output_buffer = create_string_buffer(self._output_buffer_len)

    def lemmatize(self, word):
        if word is None:
            return None

        if self._output_buffer_len < 2 * len(word):
            self._output_buffer_len = 2 * len(word)
            self._output_buffer = create_string_buffer(self._output_buffer_len)

        if IS_PYTHON3:
            return self._lemmatize_py3(word)
        else:
            return self._lemmatize_py2(word)

    def _lemmatize_py3(self, word):
        word_b = bytes(word + "\0", 'UTF-8')
        self._lib.lem_lemmatize_word(word_b, self._output_buffer)
        return str(self._output_buffer.value, 'UTF-8')

    def _lemmatize_py2(self, word):
        is_unicode = isinstance(word, unicode)
        if is_unicode:
            word = word.encode('utf-8')

        self._lib.lem_lemmatize_word(word, self._output_buffer)
        return self._output_buffer.value.decode('utf-8') if is_unicode else self._output_buffer.value

    @staticmethod
    def _get_library_extension():
        extension = sysconfig.get_config_var("EXT_SUFFIX")
        if extension is None:   # EXT_SUFFIX is only available in new Pythons
            extension = sysconfig.get_config_var("SO")
        return extension
