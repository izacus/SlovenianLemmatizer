package si.virag.lemmatizer;

public class SlLemmatizer 
{
	private native int loadLanguageLibrary(String fileName);
	public native String lemmatize(String word);
	
	static 
	{
		System.loadLibrary("Lemmatizer");
	}
	
	public SlLemmatizer(String languageFile)
	{
		this.loadLanguageLibrary(languageFile);
	}
}
