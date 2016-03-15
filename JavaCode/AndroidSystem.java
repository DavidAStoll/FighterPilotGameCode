package fighterPilot.RisingSunAlpha;

import java.text.SimpleDateFormat;
import java.util.Calendar;

import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;

public class AndroidSystem {
	
	public static void Exit()
	{
		FighterPilotActivity.GFighterPilotActivity.iWakeLock.release(); //screen can dimm again
		System.exit(0); //closes the activity and deletes all its Threads
	}
	
	public static String GetDate()
	{
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy:MM:dd");
		String lDate = sdf.format(Calendar.getInstance().getTime());
		
		return lDate;
	}
	
	public static String GetTime()
	{
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
		String lTime = sdf.format(Calendar.getInstance().getTime());
		
		return lTime;
	}
	
	public static String GetPhoneSerialNumber()
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		TelephonyManager lTelephonyManager = (TelephonyManager) lContext.getSystemService(Context.TELEPHONY_SERVICE);
		String lPhoneSerialNumber = lTelephonyManager.getDeviceId();
		
		if(lPhoneSerialNumber == null || lPhoneSerialNumber.length() < 5)
		{
			lPhoneSerialNumber = "X" + Build.SERIAL;
		}
		
		if(lPhoneSerialNumber == null || lPhoneSerialNumber.length() < 5)
		{
			try
			{
				//means that this device is has no phone capabilities, create a custom string that is somewhat unique
				String lPseudoString = "XX" + //signal that this device is not a phone
										+ Build.BOARD.length() % 10 +  Build.BRAND.length() % 10
										+ Build.CPU_ABI.length() % 10 +  Build.DEVICE.length() % 10
										+ Build.DISPLAY.length() % 10 +  Build.HOST.length() % 10
										+ Build.ID.length() % 10 +  Build.MANUFACTURER.length() % 10
										+ Build.MODEL.length() % 10 +  Build.PRODUCT.length() % 10
										+ Build.TAGS.length() % 10 +  Build.TYPE.length() % 10
										+ Build.USER.length() % 10; //13 digits
				
				//also use Andorid Id, to make it even more likely to have a unique id
				String lAndroidId = Secure.getString(lContext.getContentResolver(), Secure.ANDROID_ID);
				if(lAndroidId == null)
				{
					lAndroidId = "AndroidIdFailed";
				}
				else
				{
					if(lAndroidId.length() > 10)
						lAndroidId = lAndroidId.substring(0, 10);
					else
						lAndroidId = lAndroidId.substring(0, lAndroidId.length());
				}
				
				lPseudoString += lAndroidId; //another 10 digits
				
				//replace PhoneSerialNumber
				lPhoneSerialNumber = lPseudoString;
			}
			catch(Exception e)
			{
				lPhoneSerialNumber = "PhoneSerialFailed!";
			}
		}
		 
		return lPhoneSerialNumber;
	}
	
	public static String GetPhoneModel()
	{
		return Build.MODEL;
	}

	public static String GetPhoneOSVersion()
	{
		String lPhoneOS = "Andriod_";
		lPhoneOS = lPhoneOS + Build.VERSION.SDK_INT;
		
		return lPhoneOS;
	}
	
	public static boolean GetIsConnectedToWLan()
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		ConnectivityManager lConnectManager = (ConnectivityManager) lContext.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo lActiveNetwork = lConnectManager.getActiveNetworkInfo();
		
		if(lActiveNetwork.isConnectedOrConnecting())
		{
			if(lActiveNetwork.getType() != ConnectivityManager.TYPE_MOBILE)
			{
				return true;
			}
		}
		
		return false;
	}
	
	public static boolean GetIsInSilentMode()
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		AudioManager lAudioManager = (AudioManager) lContext.getSystemService(Context.AUDIO_SERVICE);
		int lRingerMode = lAudioManager.getRingerMode();
		boolean lDisableSound = true;

		switch (lRingerMode) 
		{
			case AudioManager.RINGER_MODE_SILENT:
			{
				lDisableSound = true;
				break;
			}
			case AudioManager.RINGER_MODE_VIBRATE:
			{
				lDisableSound = true;
				break;
			}
			case AudioManager.RINGER_MODE_NORMAL:
			{
				lDisableSound = false;
				break;
			}
		}
		
		return lDisableSound;
	}
	
	public static void OpenWebsite(String aURL)
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		
		try
		{	
			//opens the default webbrowser
			Intent lBrowserIntent = new Intent(Intent.ACTION_VIEW);
			lBrowserIntent.setData(Uri.parse(aURL));
			lBrowserIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			lContext.startActivity(lBrowserIntent);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

}
