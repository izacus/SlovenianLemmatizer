/*
 * sl_lemmatizer_java.cpp
 *
 *  Created on: Sep 25, 2011
 *      Author: Jernej Virag
 */
#include <iostream>
#include <jni.h>
#include "sl_lemmatizer_java.h"
#include "RdrLemmatizer.h"

// Private lemmatizer instance
RdrLemmatizer *lem_instance = NULL;
char* lemmatizer_buffer;
int lemmatizer_buffer_length = 512;

JNIEXPORT jint JNICALL Java_si_virag_lemmatizer_SlLemmatizer_loadLanguageLibrary
  (JNIEnv *jniEnv, jobject jObject, jstring jFileName)
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
		delete lemmatizer_buffer;
	}

	lem_instance = new RdrLemmatizer(fileName);
	lemmatizer_buffer = new char[lemmatizer_buffer_length];

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

    if (jniEnv->GetStringUTFLength(jWord) > (lemmatizer_buffer_length - 1)) 
    {
        delete lemmatizer_buffer;
        lemmatizer_buffer_length = lemmatizer_buffer_length * 2;
        lemmatizer_buffer = new char[lemmatizer_buffer_length];
    }

	lem_instance->Lemmatize(jniEnv->GetStringUTFChars(jWord,0), lemmatizer_buffer);
	return jniEnv->NewStringUTF(lemmatizer_buffer);
}


