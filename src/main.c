#include "sl_lemmatizer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("usage: %s <language file.bin> <word>\r\n", argv[0]);
		return 1;
	}

    if (lem_load_language_library(argv[1]) != STATUS_OK)
    {
    	fprintf(stderr, "[ERROR] Failed to open language library file!\r\n");
    	return -1;
    }

    char* word = lem_lemmatize_word_alloc(argv[2]);
    printf("%s", word);
    free(word);

    return 0;
}
