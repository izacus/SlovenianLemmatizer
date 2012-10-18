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

#include <iostream>
#include <jni.h>
#include "sl_lemmatizer_java.h"
#include "RdrLemmatizer.h"

// Private lemmatizer instance
RdrLemmatizer *lem_instance = NULL;

JNIEXPORT jint JNICALL Java_si_virag_lemmatizer_SlLemmatizer_loadLanguageLibrary(JNIEnv *jniEnv, jobject jObject, jstring jFileName)
{
	const char* fileName = jniEnv->GetStringUTFChars(jFileName, 0);
	// Check if file exists first
	ifstream dataFile(fileName);
	if (!dataFile.good()) {
		dataFile.close();
		jclass exceptionClass = jniEnv->FindClass("java/io/IOException");
		jniEnv->ThrowNew(exceptionClass, "Could not find language dictionary file, check path!");
		return -1;
	};
	dataFile.close();

	if (lem_instance != NULL)
	{
		delete lem_instance;
	}

	lem_instance = new RdrLemmatizer(fileName);
	return 0;
};

JNIEXPORT jint JNICALL Java_si_virag_lemmatizer_SlLemmatizer_lemmatize(JNIEnv *jniEnv, jobject jObject, jobject inputWord, jobject output)
{
	if (lem_instance == NULL)
	{
		jclass exceptionClass = jniEnv->FindClass("java/lang/Exception");
		jniEnv->ThrowNew(exceptionClass, "Language data file not loaded, call loadLanguageLibrary first!");
		return 0;
	}

  char* cWord = (char*)jniEnv->GetDirectBufferAddress(inputWord);
  char* cOutput = (char*)jniEnv->GetDirectBufferAddress(output);
  lem_instance->Lemmatize(cWord, cOutput);
  return strlen(cOutput);
}


