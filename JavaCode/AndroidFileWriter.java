package fighterPilot.RisingSunAlpha;

import java.io.DataOutputStream;
import android.content.Context;

public class AndroidFileWriter {
	
	String iFileName;
	int iMode;
	DataOutputStream iDataOutputStream;
	
	//need default constructor for JNI calls
	public AndroidFileWriter()
	{
		iFileName = "";
		iDataOutputStream = null;
	}
	
	public AndroidFileWriter(String aFileName, int aMode) {
		iFileName = aFileName;
		iDataOutputStream = null;
		
		if(aMode == 0) //replace
		{
			iMode = Context.MODE_PRIVATE;
		}
		else
		{
			iMode = Context.MODE_APPEND;
		}
	}
	
	public static int OpenFile(AndroidFileWriter aFilewriter)
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		
		try
		{
			aFilewriter.iDataOutputStream = new DataOutputStream( lContext.openFileOutput(aFilewriter.iFileName, aFilewriter.iMode));
		}
		catch(Exception aException)
		{
			return 0;
		}
		
		return 1;
	}
	
	public static void CloseFile(AndroidFileWriter aFileWriter)
	{
		if(aFileWriter.iDataOutputStream != null)
		{
			try
			{
				aFileWriter.iDataOutputStream.close();
			}
			catch (Exception aException)
			{
				//do nothing
			}
		}
	}
	
	public static void WriteInt32(AndroidFileWriter aFileWriter, int aInt)
	{
		try
		{
			aFileWriter.iDataOutputStream.writeInt(aInt);
		}
		catch(Exception aException)
		{
			//do nothing
		}
	}
	
	public static void WriteInt64(AndroidFileWriter aFileWriter, long aInt64)
	{
		try
		{
			aFileWriter.iDataOutputStream.writeLong(aInt64);
		}
		catch(Exception aException)
		{
			//do nothing
		}
	}
	
	public static void WriteString(AndroidFileWriter aFileWriter, String aString)
	{
		try
		{
			aFileWriter.iDataOutputStream.writeUTF(aString);
		}
		catch(Exception aException)
		{
			//do nothing
		}
	}
	
	public static void DeleteFile(String aFileName)
	{
		try
		{
			Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
			lContext.deleteFile(aFileName);
		}
		catch(Exception aExeption)
		{
			//do nothing
		}
	}
}
