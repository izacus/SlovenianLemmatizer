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

#ifndef SL_LEMMATIZER_H
#define SL_LEMMATIZER_H

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  /**
 * @file lemmagen.h
 * This is lemmatizer interface file for use of lemmatizer library
 * in other projects.
 */

#define STATUS_OK 0
#define STATUS_FILE_NOT_FOUND -1
#define STATUS_FAILED -2

  /**
   * @brief Loads lemmatizer language binary file
   *
   * Loads binary RDR tree file for language into the lemmatizer.
   * This HAS TO be executed before lemmatization starts.
   *
   * @param file_name Path to the RDR tree binary file
   * @return returns STATUS_OK if the file was loaded, STATUS_FILE_NOT_FOUND otherwise
   */
  EXPORT_API int lem_load_language_library(const char *file_name);

  /**
   * @brief Lemmatizes a single word
   *
   * @param input_word Word to lemmatize, null-terminated string
   * @param output_word Buffer in which the output word will be passed
   */
  EXPORT_API void lem_lemmatize_word(const char *input_word, char *output_word);

  /**
   * @brief Lemmatizes a single word
   *
   * Lemmatizes a single word and returns result as a newly allocated null-terminated string
   *
   * @param input_word Word to lemmatize, null-terminated string
   * @return returns word in newly-allocated string which has to be manually freed
   */
  EXPORT_API char *lem_lemmatize_word_alloc(const char *input_word);

 /**
  * @brief Unloads the library.
  */
  EXPORT_API void lem_unload_language_library(void);

#ifdef __cplusplus
}
#endif

#endif
