/*
 *  This is an exports C interface file for the lemmatizer
 *
 *
 */
#include <iostream>
#include <sys/stat.h>
#include "lemmatizer.h"
#include "RdrLemmatizer.h"

RdrLemmatizer lemmatizer;

int load_language_library(char *file_name) {

	struct stat buf;
	// Check if file exists first
	if (stat(file_name, &buf) != 0)
	{
		return STATUS_FILE_NOT_FOUND;
	}

    lemmatizer.LoadBinary(file_name);

    return STATUS_OK;
}

extern "C" void lemmatize(char* input_word, char* output_word) {
    lemmatizer.Lemmatize(input_word, output_word);
}

extern "C" char* lemmatize_r(char* input_word) {
    return lemmatizer.Lemmatize(input_word, NULL);
}
