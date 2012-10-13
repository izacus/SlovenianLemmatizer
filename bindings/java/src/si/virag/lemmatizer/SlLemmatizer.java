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

import java.nio.ByteBuffer;

public class SlLemmatizer 
{
	private native int loadLanguageLibrary(String fileName);
	private native int lemmatize(ByteBuffer inputWord, ByteBuffer output);

	private ByteBuffer inputBuffer;
	private ByteBuffer outputBuffer;
	
	static 
	{
		System.loadLibrary("Lemmatizer");
	}
	
	public SlLemmatizer(String languageFile)
	{
		this.loadLanguageLibrary(languageFile);
		inputBuffer = ByteBuffer.allocateDirect(128);
		outputBuffer = ByteBuffer.allocateDirect(128);
	}
	
	private void ensureBufferSpace(int length)
	{
		// Make sure enough space is available in the buffer, consider worst-case scenario of 4-byte
		// characters.
		if (inputBuffer.capacity() < (4 * length) + 1)
		{
			inputBuffer = ByteBuffer.allocateDirect(4 * length + 1);
		}
		
		if (outputBuffer.capacity() < (4 * length) + 1)
		{
			outputBuffer = ByteBuffer.allocateDirect(4 * length + 1);
		}
	}
	
	public int lemmatizeWord(char[] buffer, int bufferLength)
	{
		ensureBufferSpace(bufferLength);
		inputBuffer.rewind();
		UnicodeUtil.UTF16toUTF8(buffer, bufferLength, inputBuffer);
		// NUL terminate the string
		inputBuffer.put((byte)0x0);
		int byteLength = this.lemmatize(inputBuffer, outputBuffer);
		outputBuffer.rewind();
		// TODO TODO TODO TODO TODO
		// This truncates word if it's longer than buffer, implement buffer resizing
		int length = UnicodeUtil.UTF8toUTF16(outputBuffer, byteLength, buffer);
		return length;
	}
}
