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
