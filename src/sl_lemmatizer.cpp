/******************************************************************************
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
******************************************************************************/


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
	    std::cerr << "[ERROR] Language file " << std::string(file_name) << " could not be found!" << std::endl;
		return STATUS_FILE_NOT_FOUND;
	}

	if (!(buf.st_mode & S_IFREG)) {
	    std::cerr << "[ERROR] Language file " << std::string(file_name) << " is not a file!" << std::endl;
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
