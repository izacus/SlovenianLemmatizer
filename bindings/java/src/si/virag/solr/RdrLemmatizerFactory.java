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

package si.virag.solr;

import java.util.Map;

import org.apache.lucene.analysis.TokenStream;
import org.apache.solr.analysis.BaseTokenFilterFactory;

public class RdrLemmatizerFactory extends BaseTokenFilterFactory  
{
	String dictionaryPath;
	Map<String, String> args;
	
	@Override
	public Map<String, String> getArgs() 
	{
		return args;
	}

	@Override
	public void init(Map<String, String> args) 
	{
		if (!args.containsKey("dictionary"))
		{
			String message = "Missing dictinary property for RdrLemmatizer!"; 
			log.error(message);
			throw new RuntimeException(message);
		}
		else
		{
			dictionaryPath = args.get("dictionary");
		}
		
		this.args = args;
	}

	@Override
	public TokenStream create(TokenStream input) 
	{
		return new RdrLemmatizer(input, dictionaryPath);
	}
}
