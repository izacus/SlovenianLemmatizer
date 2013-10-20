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

        self._output_buffer = create_string_buffer(64)

    def lemmatize(self, word):
        if (self._output_buffer < 2 * len(word)):
            self._output_buffer = create_string_buffer(2 * len(word))
        
        is_unicode = isinstance(word, unicode)
        if is_unicode:
            word = word.encode('utf-8')

        self._lib.lem_lemmatize_word(word, self._output_buffer)
        return self._output_buffer.value.decode('utf-8') if is_unicode else self._output_buffer.value

