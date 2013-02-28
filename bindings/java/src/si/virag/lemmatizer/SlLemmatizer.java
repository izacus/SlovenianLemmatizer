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

package si.virag.lemmatizer;

import java.io.IOException;
import java.nio.ByteBuffer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class SlLemmatizer 
{
	private static final Logger log = LoggerFactory.getLogger(SlLemmatizer.class);
	private native int loadLanguageLibrary(String fileName) throws IOException;
	public native int lemmatize(ByteBuffer inputWord, ByteBuffer output);
	
	static 
	{
		System.loadLibrary("Lemmatizer");
	}
	
	public SlLemmatizer(String languageFile)
	{
		log.info("Lemmatizer loading language file: " + languageFile);
		try {
			this.loadLanguageLibrary(languageFile);
		} catch (IOException e) {
			String message = "Lemmatizer language file not found: " + languageFile;
			log.error(message);
			throw new RuntimeException(message);
		}
		
	}
}
