package fighterPilot.RisingSunAlpha;

import java.io.FileInputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.MediaPlayer;
import android.widget.MediaController.MediaPlayerControl;

public class AndroidAssetDirectory {
	
	public static int GetAssetFileSizeInBytes(String aFileName) {
		int lFileSizeInBytes = -1;
		Context lContext = FighterPilotActivity.GFighterPilotActivity
				.getApplication();
		AssetManager lAssetManager = lContext.getAssets();

		try {
			AssetFileDescriptor lFileStream = lAssetManager.openFd(aFileName);
			lFileSizeInBytes = (int) lFileStream.getLength();
		} catch (Exception aException) {
			//do nothing
		}

		return lFileSizeInBytes;
	}
	
	public static void PlayVideoFromAssetDirectory(String aVideoFileName, int aVolume)
	{
		Context lContext = FighterPilotActivity.GFighterPilotActivity.getApplication();
		
		try {
			MediaPlayer lVideoPlayer = new MediaPlayer();
			AndroidMediaPlayerCompleted lVideoCompletionCallback = new AndroidMediaPlayerCompleted();
		    AssetFileDescriptor lAssetDescriptor = lContext.getAssets().openFd(aVideoFileName);
		    
		    lVideoPlayer.setDataSource(lAssetDescriptor.getFileDescriptor(), lAssetDescriptor.getStartOffset(), lAssetDescriptor.getLength());
		    lAssetDescriptor.close();
		    
		    //prepare player
		    lVideoPlayer.setDisplay(AndroidGameThread.GetSurfaceHolder());
		    lVideoPlayer.setVolume((float) aVolume / 100, (float) aVolume / 100);
		    lVideoPlayer.setOnCompletionListener(lVideoCompletionCallback);
		    lVideoPlayer.prepare();
		    lVideoPlayer.start();
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}

	public static byte[] ReadAssetFile(String aFileName) {
		ByteBuffer lByteBuffer = null;
		byte[] lFileContent = null;
		Context lContext = FighterPilotActivity.GFighterPilotActivity
				.getApplication();
		AssetManager lAssetManager = lContext.getAssets();

		try {
			AssetFileDescriptor lFileStream = lAssetManager.openFd(aFileName);
			int lFileSizeInBytes = (int) lFileStream.getLength();
			lFileContent = new byte[lFileSizeInBytes];
			lByteBuffer = ByteBuffer.allocate(lFileSizeInBytes);
			lByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
			FileInputStream lInputStream = lFileStream.createInputStream();
			lInputStream.read(lFileContent);
			lInputStream.close();
			lByteBuffer.put(lFileContent);
			lByteBuffer.rewind();
			lByteBuffer.get(lFileContent);
	
		} catch (Exception aException) {
			lFileContent = null;
		}

		return lFileContent;
	}
	
	public static byte[] ReadBytesFromAssetFile(String aFileName, int aLength, int aOffset) {
		ByteBuffer lByteBuffer = null;
		byte[] lFileContent = null;
		Context lContext = FighterPilotActivity.GFighterPilotActivity
				.getApplication();
		AssetManager lAssetManager = lContext.getAssets();

		try {
			AssetFileDescriptor lFileStream = lAssetManager.openFd(aFileName);
			lFileContent = new byte[aLength];
			lByteBuffer = ByteBuffer.allocate(aLength);
			lByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
			FileInputStream lInputStream = lFileStream.createInputStream();
			lInputStream.skip(aOffset); //skip to the correct position in the File
			lInputStream.read(lFileContent, 0, aLength);
			lInputStream.close();
			lByteBuffer.put(lFileContent);
			lByteBuffer.rewind();
			lByteBuffer.get(lFileContent);
	
		} catch (Exception aException) {
			lFileContent = null;
		}

		return lFileContent;
	}

}
