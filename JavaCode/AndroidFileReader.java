package fighterPilot.RisingSunAlpha;

import java.io.DataInputStream;

import android.content.Context;
import android.graphics.Bitmap;
import android.webkit.WebView;

public class AndroidFileReader {
	
	String iFileName;
	DataInputStream iDataInputStream;
	
	//need default constructor for JNI calls
	public AndroidFileReader()
	{
		iFileName = "";
		iDataInputStream = null;
	}
	
	public AndroidFileReader(String aFileName)
	{
		iFileName = aFileName;
		iDataInputStream = null;
	}
	
	public static int OpenFile(AndroidFileReader aFileReader)
	{
		Context lContext =  FighterPilotActivity.GFighterPilotActivity.getApplication();
		
		try
		{
			aFileReader.iDataInputStream = new DataInputStream(lContext.openFileInput(aFileReader.iFileName));
		}
		catch(Exception aException)
		{
			aException.printStackTrace();
			return 0;
		}
		return 1;
	}
	
	public static void CloseFile(AndroidFileReader aFileReader)
	{
		if(aFileReader.iDataInputStream != null)
		{
			try
			{
				aFileReader.iDataInputStream.close();
			}
			catch (Exception aException)
			{
				//do nothing
				aException.printStackTrace();
			}
		}
	}
	
	public static int ReadInt32(AndroidFileReader aFileReader)
	{
		try
		{
			 int lValue = aFileReader.iDataInputStream.readInt();
			 return lValue;
		}
		catch (Exception aException)
		{
			aException.printStackTrace();
			return 0;
		}
	}
	
	public static long ReadInt64(AndroidFileReader aFileReader)
	{
		try
		{
			 long lValue = aFileReader.iDataInputStream.readLong();
			 return lValue;
		}
		catch (Exception aException)
		{
			aException.printStackTrace();
			return 0;
		}
	}
	
	public static String ReadString(AndroidFileReader aFileReader)
	{
		try
		{
			 String lValue = aFileReader.iDataInputStream.readUTF();
			 return lValue;
		}
		catch (Exception aException)
		{
			aException.printStackTrace();
			return null;
		}
	}
}
