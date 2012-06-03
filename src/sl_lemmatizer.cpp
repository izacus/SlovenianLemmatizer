/*
 *  This exports C interface for the lemmatizer
 *
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sl_lemmatizer.h"
#include "RdrLemmatizer.h"

RdrLemmatizer *lemmatizer = NULL;

int lem_load_language_library(char *file_name) {

	struct stat buf;
	// Check if file exists first
	if (stat(file_name, &buf) != 0) {
		return STATUS_FILE_NOT_FOUND;
	}

	if (lemmatizer != NULL) {
		delete lemmatizer;
	}

	lemmatizer = new RdrLemmatizer(file_name);
    return STATUS_OK;
}

extern "C" void lem_lemmatize_word(char* input_word, char* output_word) {
	if (lemmatizer == NULL) {
		std::cerr << "[ERROR] Language file for lemmatizer has to be loaded first!" << std::endl;
		return;
	}

    lemmatizer->Lemmatize(input_word, output_word);
}

extern "C" char* lem_lemmatize_word_alloc(char* input_word) {
	if (lemmatizer == NULL) {
		std::cerr << "[ERROR] Language file for lemmatizer has to be loaded first!" << std::endl;
		return NULL;
	}

	char* output_word = lemmatizer->Lemmatize(input_word, NULL);
	// Output word is allocated with "new" so we have to reallocate to change to
	// C-like malloc allocation
	char* return_val = (char*)malloc(sizeof(char) * strlen(output_word) + 1);
	strcpy(return_val, output_word);
	delete[] output_word;

	return return_val;
}
