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
