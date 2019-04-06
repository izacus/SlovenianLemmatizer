# cython: language_level=3

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

from libc.string cimport strcpy, strlen
from libc.stdlib cimport malloc

cdef extern from "lemmagen.h":
    int lem_load_language_library(const char *file_name)
    void lem_lemmatize_word(const char *input_word, char *output_word)

cdef class Lemmagen:
    cdef char* output_buffer;

    STATUS_OK = 0
    STATUS_FILE_NOT_FOUND = -1

    def __init__(self, language_library_path):
        self.output_buffer = <char*> malloc(256 * sizeof(char))
        cdef int result = lem_load_language_library(language_library_path)
        if result == self.STATUS_FILE_NOT_FOUND:
            raise IOError("Lemmatizer dictionary file not found!")
        if not result == self.STATUS_OK:
            raise IOError("Failed to initialize lemmatizer!")

    def lemmatize(self, input_buffer: bytes) -> bytes:
        lem_lemmatize_word(input_buffer, self.output_buffer)
        return self.output_buffer