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
import lemmagen

class Lemmatizer(object):
    STATUS_OK = 0
    STATUS_FILE_NOT_FOUND = -1

    def __init__(self, dictionary=lemmagen.DICTIONARY_SLOVENE, library_path=None):
        """
        @param dictionary: Path to lemmatizer dictionary file
        """

        this_dir = os.path.dirname(os.path.realpath(__file__))
        print "This dir:", this_dir

        if library_path is None:
            self._lib = cdll.LoadLibrary(os.path.join(this_dir, "libLemmagen.so"))
        else:
            self._lib = cdll.LoadLibrary(library_path)

        if not dictionary.startswith("/"):
            dictionary = os.path.join(this_dir, dictionary)
        result = self._lib.lem_load_language_library(dictionary)

        if result == self.STATUS_FILE_NOT_FOUND:
            raise IOError("Lemmatizer dictionary file could not be found.")
        assert(result == self.STATUS_OK)

        self._output_buffer_len = 64
        self._output_buffer = create_string_buffer(self._output_buffer_len)

    def lemmatize(self, word):
        if (self._output_buffer_len < 2 * len(word)):
            self._output_buffer_len = 2 * len(word)
            self._output_buffer = create_string_buffer(self._output_buffer_len)
        
        is_unicode = isinstance(word, unicode)
        if is_unicode:
            word = word.encode('utf-8')

        self._lib.lem_lemmatize_word(word, self._output_buffer)
        return self._output_buffer.value.decode('utf-8') if is_unicode else self._output_buffer.value

