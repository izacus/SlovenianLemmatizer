package si.virag.lemmatizer;

public class SlLemmatizer 
{
	private native int loadLanguageLibrary(String fileName);
	public native String lemmatize(String word);
	
	public SlLemmatizer(String languageFile)
	{
		System.loadLibrary("Lemmatizer");
		this.loadLanguageLibrary(languageFile);
	}
}
