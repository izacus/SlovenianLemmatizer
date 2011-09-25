#ifndef LEMMATIZER_H
#define LEMMATIZER_H

#define STATUS_OK 0
#define STATUS_FILE_NOT_FOUND -1

#ifdef __cplusplus
extern "C"
{
#endif
	int load_language_library(char *file_name);
	void lemmatize(char* input_word, char* output_word);
	char* lemmatize_r(char* input_word);

#ifdef __cplusplus
}
#endif
#endif
