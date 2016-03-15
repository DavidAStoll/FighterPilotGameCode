package fighterPilot.RisingSunAlpha;

import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.SocketChannel;

import android.hardware.Camera.Size;

public class AndroidTCPConnectionEngine {
	
	SocketChannel iSocketChannel;
	
	public AndroidTCPConnectionEngine()
	{
		iSocketChannel = null;
	}
	
	public static boolean Open(AndroidTCPConnectionEngine aTCPObject, String aIpAddress, int aPort, int aConnectionTimeoutInSecs)
	{
		try
		{
			aTCPObject.iSocketChannel = SocketChannel.open();
			aTCPObject.iSocketChannel.configureBlocking(true);
			aTCPObject.iSocketChannel.socket().connect(new InetSocketAddress(aIpAddress, aPort), aConnectionTimeoutInSecs * 1000);
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	public static void Close(AndroidTCPConnectionEngine aTCPObject)
	{
		try
		{
			aTCPObject.iSocketChannel.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public static int[] Read(AndroidTCPConnectionEngine aTCPObject, int aSize)
	{
		int lBytesRead = 0;
		int[] aMessageData = new int[aSize];
		ByteBuffer lMessageBuffer = ByteBuffer.allocate(aSize * 4); //since one int is 4 bytes in size
		
		try
		{
			lMessageBuffer.order(ByteOrder.BIG_ENDIAN);
			lMessageBuffer.clear();
			
			//get data from socket and store in buffer
			while(lBytesRead < aSize * 4) //since bytes
				lBytesRead += aTCPObject.iSocketChannel.read(lMessageBuffer);
			lMessageBuffer.flip();
			
			//copy values back into the array
			for(int lIndex = 0; lIndex < aSize; lIndex++)
				aMessageData[lIndex] = lMessageBuffer.getInt();
			
			return aMessageData;
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return null;
		}
	}
	
	public static void Send(AndroidTCPConnectionEngine aTCPObject, int[] aMessageData)
	{
		try
		{
			ByteBuffer lMessageBuffer = ByteBuffer.allocate(aMessageData.length * 4); //since one int is 4 bytes in size
			lMessageBuffer.order(ByteOrder.BIG_ENDIAN);
			lMessageBuffer.clear();
			for(int lIndex = 0; lIndex < aMessageData.length; lIndex++)
				lMessageBuffer.putInt(aMessageData[lIndex]);
			lMessageBuffer.flip();
			
			while(lMessageBuffer.hasRemaining())
				aTCPObject.iSocketChannel.write(lMessageBuffer);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

}
