package fighterPilot.RisingSunAlpha;

import java.io.File;
import java.util.Arrays;

import android.content.res.AssetManager;
import android.os.Environment;


public class AndroidDirectory {
	
	//need default constructor for JNI calls
	public AndroidDirectory()
	{
	}
	
	public static void CreateDirectory(String aString)
	{
		File lFileDirectory = new File(aString);
		lFileDirectory.mkdirs();
	}
	
	public static String GetStorageDirectory()
	{
		return ""; //return nothing, Android doesn't give us a filepath to its storage directory, thus we can't use this
	}
	
	public static String[] GetFilesInDirectory(String aString)
	{
		File lFileDirectory = new File(aString);
		String[] lFiles = lFileDirectory.list();
		
		//want it sorted alphabetically
		if(lFiles != null)
			Arrays.sort(lFiles);
		return lFiles;
	}
	
	public static String GetPublicDirectory()
	{
		String lPublicDirectory = null;
		lPublicDirectory = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;
		
		return lPublicDirectory;
	}
	
	public static AssetManager GetAssetManager()
	{
		return FighterPilotActivity.GFighterPilotActivity.getAssets();
	}

}
