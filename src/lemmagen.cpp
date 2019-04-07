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
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/lemmagen.h"
#include "RdrLemmatizer.h"

#define HAS_MUTEX __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)

// MSVC before 14 doesn't really handle C++11
#if HAS_MUTEX
	#include <mutex>
	static std::mutex mutex_lemmatizer;
#endif

static RdrLemmatizer *lemmatizer = nullptr;


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_API int lem_load_language_library(const char *file_name)
	{
		#if HAS_MUTEX
		std::lock_guard<std::mutex> lock(mutex_lemmatizer);
		#endif

		struct stat buf;
		// Check if file exists first
		if (stat(file_name, &buf) != 0)
		{
			std::cerr << "[ERROR] Language file " << std::string(file_name) << " could not be found!" << std::endl;
			return STATUS_FILE_NOT_FOUND;
		}

		if (!(buf.st_mode & S_IFREG))
		{
			std::cerr << "[ERROR] Language file " << std::string(file_name) << " is not a file!" << std::endl;
			return STATUS_FILE_NOT_FOUND;
		}

		if (lemmatizer != nullptr)
		{
			delete lemmatizer;
		}

		try {
			lemmatizer = new RdrLemmatizer(file_name);
		} catch (std::ifstream::failure e) {
			return STATUS_FILE_NOT_FOUND;
		}

		return STATUS_OK;
	}

	EXPORT_API void lem_lemmatize_word(const char *input_word, char *output_word)
	{
		// TODO figure out fast locking for this to ensure thread safety
		// Lemmatization should be reentrant, but initialization isn't.
		if (lemmatizer == nullptr)
		{
			std::cerr << "[ERROR] Language file for lemmatizer has to be loaded first!" << std::endl;
			return;
		}

		if (output_word == nullptr) {
			std::cerr << "[ERROR] Output buffer is NULL!" << std::endl;
			return;
		}

		if (input_word == nullptr) {
			output_word[0] = '\0';
			return;
		}

		lemmatizer->Lemmatize(input_word, output_word);
	}

	EXPORT_API char *lem_lemmatize_word_alloc(const char *input_word)
	{
		if (lemmatizer == nullptr)
		{
			std::cerr << "[ERROR] Language file for lemmatizer has to be loaded first!" << std::endl;
			return nullptr;
		}

		if (input_word == nullptr) {
			char *return_val = (char *)malloc(sizeof(char));
			return_val[0] = '\0';
			return return_val;
		}

		char *output_word = lemmatizer->Lemmatize(input_word, nullptr);
		// Output word is allocated with "new" so we have to reallocate to change to
		// C-like malloc allocation
		char *return_val = (char *)malloc(sizeof(char) * strlen(output_word) + 1);
		strcpy(return_val, output_word);
		delete[] output_word;

		return return_val;
	}

	EXPORT_API void lem_unload_language_library(void) {
		#if HAS_MUTEX
		std::lock_guard<std::mutex> lock(mutex_lemmatizer);
		#endif

		if (lemmatizer != nullptr) {
			delete lemmatizer;
			lemmatizer = nullptr;
		}
	}

#ifdef __cplusplus
}
#endif
