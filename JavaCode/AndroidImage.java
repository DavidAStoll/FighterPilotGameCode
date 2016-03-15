package fighterPilot.RisingSunAlpha;

import fighterPilot.RisingSunAlpha.FighterPilotActivity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

public class AndroidImage {
	
	public AndroidImage() {
		//needed for JNI
	}
	
	public static Bitmap OpenBitmap(String aPath)
	{
		Bitmap lBitmap;
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
	
		if(FighterPilotActivity.IsGooglePlayBuild)
		{	
			try
			{
				lBitmap = BitmapFactory.decodeStream(FighterPilotActivity.GooglePlayExternalResourceFile.getInputStream(aPath));
			}
			catch(Exception aException)
			{
				lBitmap = null;
				Log.d("Image Not Found", "Image was not found!!!! " + aPath);
			}
		}
		else
		{
			AssetManager lAssetManager = lContext.getAssets();
			
			try
			{
				lBitmap = BitmapFactory.decodeStream(lAssetManager.open(aPath));
			}
			catch(Exception aException)
			{
				lBitmap = null;
				Log.d("Image Not Found", "Image was not found!!!! " + aPath);
			}
		}
		
		return lBitmap;
	}
	
	public static int GetWidth(Bitmap aBitmap)
	{
		return aBitmap.getWidth();
	}
	
	public static int GetHeight(Bitmap aBitmap)
	{
		return aBitmap.getHeight();
	}
	
	public static void GetPixelData(Bitmap aBitmap, int[] aPointer)
	{
		try
		{
			int lWidth = aBitmap.getWidth();
			int lHeight = aBitmap.getHeight();
			aBitmap.getPixels(aPointer, 0, lWidth, 0, 0, lWidth, lHeight);
		}
		catch(Exception aException)
		{
			aPointer = null;
			Log.d("Not Enough Memory!","Out Of Memory During GetPixelData!!!");
		}
	}
	
	public static void CloseBitmap(Bitmap aBitmap)
	{
		aBitmap.recycle();
	}
}
