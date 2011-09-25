/*
 * sl_lemmatizer_java.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: Jernej Virag
 */
#include <jni.h>
#include <iostream>
#include "sl_lemmatizer_java.h"
#include "RdrLemmatizer.h"

// Private lemmatizer instance
RdrLemmatizer *lem_instance = NULL;
char* lemmatizer_buffer;

JNIEXPORT jint JNICALL Java_si_virag_lemmatizer_SlLemmatizer_loadLanguageLibrary
  (JNIEnv *jniEnv, jobject jObject, jstring jFileName)
{
	const char* fileName = jniEnv->GetStringUTFChars(jFileName, 0);
	// Check if file exists first
	ifstream dataFile(fileName);
	if (!dataFile.good()) {
		dataFile.close();
		jclass exceptionClass = jniEnv->FindClass("java/io/IOException");
		jniEnv->ThrowNew(exceptionClass, "Could not open library file!");
		return -1;
	};
	dataFile.close();

	if (lem_instance != NULL)
	{
		delete lem_instance;
		delete lemmatizer_buffer;
	}

	lem_instance = new RdrLemmatizer(fileName);
	lemmatizer_buffer = new char[255];

	return 0;
};

JNIEXPORT jstring JNICALL Java_si_virag_lemmatizer_SlLemmatizer_lemmatize
  (JNIEnv *jniEnv, jobject jObject, jstring jWord)
{
	if (lem_instance == NULL)
	{
		jclass exceptionClass = jniEnv->FindClass("java/lang/Exception");
		jniEnv->ThrowNew(exceptionClass, "Language data file not loaded, call loadLanguageLibrary first!");
		return jniEnv->NewStringUTF("");
	}

	lem_instance->Lemmatize(jniEnv->GetStringUTFChars(jWord,0), lemmatizer_buffer);
	return jniEnv->NewStringUTF(lemmatizer_buffer);
}


