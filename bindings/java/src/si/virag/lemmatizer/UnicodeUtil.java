package si.virag.lemmatizer;

import java.nio.ByteBuffer;

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*
 * Some of this code came from the excellent Unicode
 * conversion examples from:
 *
 *   http://www.unicode.org/Public/PROGRAMS/CVTUTF
 *
 * Full Copyright for that code follows:
*/

/*
 * Copyright 2001-2004 Unicode, Inc.
 * 
 * Disclaimer
 * 
 * This source code is provided as is by Unicode, Inc. No claims are
 * made as to fitness for any particular purpose. No warranties of any
 * kind are expressed or implied. The recipient agrees to determine
 * applicability of information provided. If this file has been
 * purchased on magnetic or optical media from Unicode, Inc., the
 * sole remedy for any claim will be exchange of defective media
 * within 90 days of receipt.
 * 
 * Limitations on Rights to Redistribute This Code
 * 
 * Unicode, Inc. hereby grants the right to freely use the information
 * supplied in this file in the creation of products supporting the
 * Unicode Standard, and to make copies of this file in any form
 * for internal or external distribution as long as this notice
 * remains attached.
 */

/*
 * Additional code came from the IBM ICU library.
 *
 *  http://www.icu-project.org
 *
 * Full Copyright for that code follows.
 */

/*
 * Copyright (C) 1999-2010, International Business Machines
 * Corporation and others.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * provided that the above copyright notice(s) and this permission notice appear
 * in all copies of the Software and that both the above copyright notice(s) and
 * this permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE BE
 * LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder shall not
 * be used in advertising or otherwise to promote the sale, use or other
 * dealings in this Software without prior written authorization of the
 * copyright holder.
 */

public class UnicodeUtil 
{
	
	public static final int UNI_SUR_HIGH_START = 0xD800;
	public static final int UNI_SUR_HIGH_END = 0xDBFF;
	public static final int UNI_SUR_LOW_START = 0xDC00;
	public static final int UNI_SUR_LOW_END = 0xDFFF;
	public static final int UNI_REPLACEMENT_CHAR = 0xFFFD;

	private static final long UNI_MAX_BMP = 0x0000FFFF;

	private static final long HALF_SHIFT = 10;
	private static final long HALF_MASK = 0x3FFL;

	private static final int SURROGATE_OFFSET = Character.MIN_SUPPLEMENTARY_CODE_POINT
			- (UNI_SUR_HIGH_START << HALF_SHIFT) - UNI_SUR_LOW_START;
	
	 public static void UTF16toUTF8(final char[] source, final int length, ByteBuffer outputBuffer) 
	 {
		int i = 0;
		final int end = length;
		// Pre-allocate for worst case 4-for-1
		final int maxLen = length * 4;
		if (outputBuffer.capacity() < maxLen)
			throw new RuntimeException("Preallocated output buffer is too small, allocate 4*length!");
		
		while (i < end) 
		{

			final int code = (int) source[i++];

			if (code < 0x80)
				outputBuffer.put((byte) code);
			else if (code < 0x800) {
				outputBuffer.put((byte) (0xC0 | (code >> 6)));
				outputBuffer.put((byte) (0x80 | (code & 0x3F)));
			} 
			else if (code < 0xD800 || code > 0xDFFF) 
			{
				outputBuffer.put((byte) (0xE0 | (code >> 12)));
				outputBuffer.put((byte) (0x80 | ((code >> 6) & 0x3F)));
				outputBuffer.put((byte) (0x80 | (code & 0x3F)));
			} 
			else 
			{
				// surrogate pair
				// confirm valid high surrogate
				if (code < 0xDC00 && i < end) 
				{
					int utf32 = (int) source[i];
					// confirm valid low surrogate and write pair
					if (utf32 >= 0xDC00 && utf32 <= 0xDFFF) 
					{
						utf32 = (code << 10) + utf32 + SURROGATE_OFFSET;
						i++;
						outputBuffer.put((byte) (0xF0 | (utf32 >> 18)));
						outputBuffer.put((byte) (0x80 | ((utf32 >> 12) & 0x3F)));
						outputBuffer.put((byte) (0x80 | ((utf32 >> 6) & 0x3F)));
						outputBuffer.put((byte) (0x80 | (utf32 & 0x3F)));
						continue;
					}
				}
				// replace unpaired surrogate or out-of-order low surrogate
				// with substitution character
				outputBuffer.put((byte) 0xEF);
				outputBuffer.put((byte) 0xBF);
				outputBuffer.put((byte) 0xBD);
			}
		}
	}
	 
	 
	public static int UTF8toUTF16(ByteBuffer utf8, int length, char[] output) 
	{
		int offset = 0;
		int out_offset = 0;
		final int limit = length;
		while (offset < limit) 
		{
			int b = utf8.get() & 0xff;
			
			// Check for NUL terminator
			if (b == 0x0 || out_offset == output.length)
				break;
			
			if (b < 0xc0) 
			{
				assert b < 0x80;
				output[out_offset++] = (char) b;
			} 
			else if (b < 0xe0) 
			{
				output[out_offset++] = (char) (((b & 0x1f) << 6) + (utf8.get() & 0x3f));
			} 
			else if (b < 0xf0) 
			{
				output[out_offset++] = (char) (((b & 0xf) << 12)
						+ ((utf8.get() & 0x3f) << 6) + (utf8.get() & 0x3f));
				offset += 2;
			} 
			else 
			{
				assert b < 0xf8 : "b = 0x" + Integer.toHexString(b);
				int ch = ((b & 0x7) << 18) + ((utf8.get() & 0x3f) << 12)
						+ ((utf8.get() & 0x3f) << 6)
						+ (utf8.get() & 0x3f);
				
				if (ch < UNI_MAX_BMP) 
				{
					output[out_offset++] = (char) ch;
				} 
				else 
				{
					int chHalf = ch - 0x0010000;
					output[out_offset++] = (char) ((chHalf >> 10) + 0xD800);
					output[out_offset++] = (char) ((chHalf & HALF_MASK) + 0xDC00);
				}
			}
		}
		
		return out_offset;
	}
}
