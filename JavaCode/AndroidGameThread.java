package fighterPilot.RisingSunAlpha;

import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class AndroidGameThread implements Runnable {
	
	Thread iThread;
	boolean iInitPhase;
	public static volatile boolean iReadyToUseSurface = false;
	public static SurfaceRender iSurfaceRenderer;
	
	public AndroidGameThread() 
	{
		//needed for the JNI
	}
	
	public AndroidGameThread(Context aContext) 
	{
		iInitPhase = true;
	}
	
	public void run() 
	{
		while (true)
		{
			if(iReadyToUseSurface)
				break;
		}
		
		if(iInitPhase) // only initiliaze the game the first time
		{
			iInitPhase = false;
			InitializeGame();
			ResumeGame(); //should never complete, until game is suspended or Exists
		}
		else
		{
    		InitializeOpenGl(); //need to create openGl context
    		ResumeGame(); //should never complete, until game is suspended or Exists
		}
	}
	
	public void Pause()
	{
		//Pause the GameThread
		while(true)
		{
			//signal the GameThread that Needs to Pause and suspend
			//wait for a signal back from the game that it is suspended
			if(SuspendGame()) // also exists the Game Thread
			{
				break;
			}
		}
		
		//game is suspended, thus the surface is no longer in use and we can pause
	}
	
	public void Resume(Context aContext)
	{
		iSurfaceRenderer = new SurfaceRender(aContext, iInitPhase);
		iThread = new Thread(this);
		iThread.start();
	}
	
	//needed for the JNI
	public static Surface GetGameSurface()
	{
		return iSurfaceRenderer.iSurface.getSurface();
	}
	
	//needed to play video
	public static SurfaceHolder GetSurfaceHolder()
	{
		return iSurfaceRenderer.iSurface;
	}
	
    //rendering class
    class SurfaceRender extends SurfaceView  implements SurfaceHolder.Callback
    {
		public SurfaceHolder iSurface;
		boolean iInitPhase;
		
		public SurfaceRender(Context aContext, boolean aInitPhase)
		{
			super(aContext);
			iReadyToUseSurface = false;
			iSurface = getHolder();
			iSurface.addCallback(this);
		}
		
		   @Override
		    public void surfaceChanged(SurfaceHolder holder, int format, 
		        int width, int height) 
		   	{  
			   //nothing
		    }

		    @Override
		    public void surfaceCreated(SurfaceHolder holder) 
		    {
		    	iReadyToUseSurface = true;
		    }

		    @Override
		    public void surfaceDestroyed(SurfaceHolder holder) 
		    {  
		    	iReadyToUseSurface = false;
		    	DestroyOpenGl();
		    	System.gc();
		    }
		
	}
    
    public static native void InitializeOpenGl();
    public static native void InitializeGame();
    public static native void DestroyOpenGl();
    public static native void ResumeGame();
    public static native boolean SuspendGame(); //returns true once the game is suspended
    public static native void ExitGame();

}
