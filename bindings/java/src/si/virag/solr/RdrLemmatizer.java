package si.virag.solr;

import java.io.IOException;

import org.apache.lucene.analysis.TokenFilter;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;

import si.virag.lemmatizer.SlLemmatizer;

public class RdrLemmatizer extends TokenFilter 
{
	private static SlLemmatizer lemmatizer = null;
	
	private final CharTermAttribute termAtt = addAttribute(CharTermAttribute.class);
	
	protected RdrLemmatizer(TokenStream input, String dictionaryPath) 
	{
		super(input);
		if (lemmatizer == null)
		{
			lemmatizer = new SlLemmatizer(dictionaryPath);
		}
	}
	
	@Override
	public boolean incrementToken() throws IOException 
	{
		if (this.input.incrementToken())
		{
			termAtt.setLength(this.parseBuffer(termAtt.buffer(), termAtt.length()));
			return true;
		}
		
		return false;
	}
	
	protected int parseBuffer(char[] buffer, int bufferLength)
	{
		String str = new String(buffer, 0, bufferLength);
		char[] lemmatized = lemmatizer.lemmatize(str).toCharArray();
		System.arraycopy(lemmatized, 0, buffer, 0, lemmatized.length);
		return lemmatized.length;
	}
}
