#include "lemmatizer.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("usage: %s <language file.bin> <word>\r\n", argv[0]);
		return 1;
	}

    if (load_language_library(argv[1]) != STATUS_OK)
    {
    	fprintf(stderr, "[ERROR] Failed to open language library file!\r\n");
    	return -1;
    }

    char* test = lemmatize_r(argv[2]);
    printf("%s", test);
    return 0;
}
