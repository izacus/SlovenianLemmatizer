from libc.string cimport strcpy, strlen
from libc.stdlib cimport malloc

cdef extern from "lemmagen.h":
    int lem_load_language_library(const char *file_name)
    void lem_lemmatize_word(const char *input_word, char *output_word)

cdef class Lemmagen:
    cdef char* output_buffer;

    DEF STATUS_OK = 0
    DEF STATUS_FILE_NOT_FOUND = -1

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