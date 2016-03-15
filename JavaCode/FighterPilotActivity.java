package fighterPilot.RisingSunAlpha;

//For Amazon Game Circle
import java.util.EnumSet;

import com.amazon.ags.api.AmazonGames;
import com.amazon.ags.api.AmazonGamesCallback;
import com.amazon.ags.api.AmazonGamesClient;
import com.amazon.ags.api.AmazonGamesFeature;
import com.amazon.ags.api.AmazonGamesStatus;



import com.android.vending.expansion.zipfile.APKExpansionSupport;
import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.google.android.vending.expansion.downloader.Helpers;

import fighterPilot.RisingSunAlpha.R;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Canvas;
import android.media.ExifInterface;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;
import android.os.Messenger;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.provider.Settings;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

public class FighterPilotActivity extends Activity implements OnTouchListener, IDownloaderClient {
	
	WakeLock iWakeLock;
	AndroidGameThread iGameThread;
    
    static FighterPilotActivity GFighterPilotActivity = null;
    
   //needed for Google Play
   //tells us if this is used for Google Play
   static final boolean IsGooglePlayBuild = false; 
   static final boolean IsAmazoonPlayBuild = false; 
   static final int GooglePlayMajorVersion = 1; 
   static final int GooglePlayMinorVersion = 1; 
   static ZipResourceFile GooglePlayExternalResourceFile = null;
   IStub mDownloaderClientStub;
   private IDownloaderService mRemoteService;
   boolean DownloadComplete = true;
   //for UI
   private ProgressBar mPB;
   private TextView mStatusText;
   private TextView mProgressFraction;
   private TextView mProgressPercent;
   private TextView mAverageSpeed;
   private TextView mTimeRemaining;
   private View mDashboard;
   private View mCellMessage;
   private Button mPauseButton;
   private Button mWiFiSettingsButton;
   private boolean mStatePaused;
   private int mState;
   
   //for Amazon GameCircle 
   static AmazonGames agsGameClient = null;
   static boolean IsAmazonServiceAvailable = false;
   EnumSet<AmazonGamesFeature> agsGameFeatures = EnumSet.of(AmazonGamesFeature.Leaderboards);
    
    static
   {
       // Loads the c++ code
       System.loadLibrary("FighterPilotRisingSun");
    }
    
    @Override
    //Gets Called when the Activity is created
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
    	//go full screen
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    	//need global reference to activity
        GFighterPilotActivity = this;
                
        //get Max Java Memory
        Runtime lRuntime = Runtime.getRuntime();
        long lMaxMemory = lRuntime.maxMemory() / 1000000;
        Log.v("onCreate", "JavaMemory Max MegaBytes:" + Long.toString(lMaxMemory));
        
        //get a Wakelock
        iWakeLock = ((PowerManager)getSystemService(Context.POWER_SERVICE)).newWakeLock(PowerManager.FULL_WAKE_LOCK,"My WakeLock");
        iWakeLock.acquire(); //keeps screen from dimming
        
        //need to make specific call depending on what Store this game runs on
        MakeStoreSpecificSetupCall();
        
        if(IsAmazoonPlayBuild)
        {
            // Initialize Amazon GameCircle.
            agsGameClient = AmazonGamesClient.initialize(getApplication(), agsGameCallback, agsGameFeatures);
        }
          
        if(DownloadComplete)
        {
        	iGameThread = new AndroidGameThread(this);
        }
    }
    
    public void onBackPressed()
    {
    	//do nothing
    }
    
    //gets called when the Activity resumes from a pause State
    protected void onResume()
    {  	
    	super.onResume();
    	
    	if(DownloadComplete)
    	{
	        iWakeLock.acquire(); //keeps screen from dimming
	        iGameThread.Resume(this);
	        setContentView(iGameThread.iSurfaceRenderer);
	        iGameThread.iSurfaceRenderer.setOnTouchListener(this);
    	}
    }
    
    
    //gets called when the Activity is paused
    protected void onPause()
    {
    	super.onPause();
    	
    	if(DownloadComplete)
    	{
	        iWakeLock.release(); //screen can dimm again
	        iGameThread.Pause();
	        
	    	//gets called when the application is going to exit
	    	if(isFinishing())
	    	{
	    		//call code to exit the application
	    		iGameThread.ExitGame();
	            System.exit(0); //closes the activity and deletes all its Threads
	    	}
    	}
    }
    
    public boolean onTouch(View aView, MotionEvent aEvent)
    {
    	final int lActionValue = aEvent.getAction();
    	
    	switch(lActionValue &MotionEvent.ACTION_MASK)
    	{
    		case MotionEvent.ACTION_POINTER_DOWN: //called when more then one finger goes down
			case MotionEvent.ACTION_DOWN: //called when only one finger goes down
			{
				final int lPointerIndex = (lActionValue &MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				final int lPointerId = aEvent.getPointerId(lPointerIndex); //find out the Finger Id
				final int lXLocation = (int) aEvent.getX(lPointerIndex);
				final int lYLocation = (int) aEvent.getY(lPointerIndex);

				//got all the Data we need
				AddRawTouchEvent(lXLocation, lYLocation, 0, lPointerId); // 0 for Down
				break;
			}
			case MotionEvent.ACTION_MOVE: //called when any finger moves around
			{
				int lPointerCount = aEvent.getPointerCount(); //draw events are all combined into one call
				
				for(int lPointerIndex = 0; lPointerIndex < lPointerCount; lPointerIndex++)
				{
					//final int lPointerIndex = (lActionValue &MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
					final int lPointerId = aEvent.getPointerId(lPointerIndex); //find out the Finger Id
					final int lXLocation = (int) aEvent.getX(lPointerIndex);
					final int lYLocation = (int) aEvent.getY(lPointerIndex);
	
					//got all the Data we need
					AddRawTouchEvent(lXLocation, lYLocation, 1, lPointerId); //1 for Move
				}
				break;
			}
			case MotionEvent.ACTION_CANCEL:
			case MotionEvent.ACTION_UP: //called when the last finger is lifted
			case MotionEvent.ACTION_POINTER_UP: //should be called when any finers is lifted, but at least one remains
			{
				final int lPointerIndex = (lActionValue &MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				final int lPointerId = aEvent.getPointerId(lPointerIndex); //find out the Finger Id
				final int lXLocation = (int) aEvent.getX(lPointerIndex);
				final int lYLocation = (int) aEvent.getY(lPointerIndex);

				//got all the Data we need
				AddRawTouchEvent(lXLocation, lYLocation, 2, lPointerId); //2 for Up
				break;
			}
			default:
			{
				//do nothing
				break;
			}
    	}
    	
    	return true;
    }
    
    private void MakeStoreSpecificSetupCall()
    {
    	if(IsGooglePlayBuild)
    	{
    		//check if External Resources are already present
			int MajorVersion = FighterPilotActivity.GooglePlayMajorVersion;
			int MinorVersion = FighterPilotActivity.GooglePlayMinorVersion;
			ZipResourceFile lZipFile = null;
			
			try
			{
				lZipFile = APKExpansionSupport.getAPKExpansionZipFile(this, MajorVersion, MinorVersion);
			}
			catch(Exception e)
			{
				Log.d("External Resouce Error!", "External Resource file check threw Exception");
			}
			
			if(lZipFile != null)
			{
				//all good, resources are present on the harddrive and we don't need to download them again
				GooglePlayExternalResourceFile = lZipFile;
			}
			else
			{
				//file is not present, download it
		        // Build an Intent to start this activity from the Notification
		        Intent notifierIntent = new Intent(this, FighterPilotActivity.this.getClass());
		        notifierIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
		        PendingIntent pendingIntent = PendingIntent.getActivity( FighterPilotActivity.this, 0, notifierIntent, PendingIntent.FLAG_UPDATE_CURRENT);

		        // Start the download service (if required)
		        int lResult = 0;
		        try
		        {
		        	lResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(this, pendingIntent, GoogleDownloadService.class);
			        
		        	// If download has started, initialize this activity to show download progress
			        if (lResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) 
			        {
			           //Display download process
			           // Instantiate a member instance of IStub
			           mDownloaderClientStub = DownloaderClientMarshaller.CreateStub(this, GoogleDownloadService.class);
			            
			           //Inflate layout that shows download progress
			           initializeDownloadUI();
			           DownloadComplete = false;
			           return;
			        }
			        
			        //download was not required, get Zipfile
			        GooglePlayExternalResourceFile = APKExpansionSupport.getAPKExpansionZipFile(this, MajorVersion, MinorVersion);
		        }
		        catch(Exception e)
		        {
		        	Log.d("Google download Service Error!", "Google Download service code screwed up!");
		        }
			}	
    	}
    }
    
    public void GoogleFilesHaveBeenDownloaded()
    {
    	DownloadComplete = true;
    	
    	try
    	{
    		//get Zipfile
    		GooglePlayExternalResourceFile = APKExpansionSupport.getAPKExpansionZipFile(this, GooglePlayMajorVersion, GooglePlayMinorVersion);
    	}
    	catch(Exception e)
    	{
    		Log.d("Google download Service Error!", "Couldn't resume after downloading packages!");
    	}
    	
    	//create Game Thread
    	iGameThread = new AndroidGameThread(this);   
    	
        iWakeLock.acquire(); //keeps screen from dimming
        iGameThread.Resume(this);
        setContentView(iGameThread.iSurfaceRenderer);
        iGameThread.iSurfaceRenderer.setOnTouchListener(this);
    }
    
    @Override
    public void onServiceConnected(Messenger m) 
    {
        mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
        mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
    }
    
	@Override
	public void onDownloadProgress(DownloadProgressInfo lProgress) 
	{
        mAverageSpeed.setText("KiloBytes Per Second " + lProgress.mCurrentSpeed);
        mTimeRemaining.setText("Time remaining: " + lProgress.mTimeRemaining);

        lProgress.mOverallTotal = lProgress.mOverallTotal;
        mPB.setMax((int) (lProgress.mOverallTotal >> 8));
        mPB.setProgress((int) (lProgress.mOverallProgress >> 8));
        mProgressPercent.setText(Long.toString(lProgress.mOverallProgress * 100 / lProgress.mOverallTotal) + "%");
        mProgressFraction.setText(Helpers.getDownloadProgressString (lProgress.mOverallProgress, lProgress.mOverallTotal));	
	}

	@Override
	public void onDownloadStateChanged(int newState) 
	{
	       	setState(newState);
	        boolean showDashboard = true;
	        boolean showCellMessage = false;
	        boolean paused;
	        boolean indeterminate;
	        switch (newState) {
	            case IDownloaderClient.STATE_IDLE:
	                // STATE_IDLE means the service is listening, so it's
	                // safe to start making calls via mRemoteService.
	                paused = false;
	                indeterminate = true;
	                break;
	            case IDownloaderClient.STATE_CONNECTING:
	            case IDownloaderClient.STATE_FETCHING_URL:
	                showDashboard = true;
	                paused = false;
	                indeterminate = true;
	                break;
	            case IDownloaderClient.STATE_DOWNLOADING:
	                paused = false;
	                showDashboard = true;
	                indeterminate = false;
	                break;

	            case IDownloaderClient.STATE_FAILED_CANCELED:
	            case IDownloaderClient.STATE_FAILED:
	            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
	            case IDownloaderClient.STATE_FAILED_UNLICENSED:
	                paused = true;
	                showDashboard = false;
	                indeterminate = false;
	                break;
	            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
	            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
	                showDashboard = false;
	                paused = true;
	                indeterminate = false;
	                showCellMessage = true;
	                break;

	            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
	                paused = true;
	                indeterminate = false;
	                break;
	            case IDownloaderClient.STATE_PAUSED_ROAMING:
	            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
	                paused = true;
	                indeterminate = false;
	                break;
	            case IDownloaderClient.STATE_COMPLETED:
	                showDashboard = false;
	                paused = false;
	                indeterminate = false;
	                GoogleFilesHaveBeenDownloaded();
	                return;
	            default:
	                paused = true;
	                indeterminate = true;
	                showDashboard = true;
	        }
	        int newDashboardVisibility = showDashboard ? View.VISIBLE : View.GONE;
	        if (mDashboard.getVisibility() != newDashboardVisibility) {
	            mDashboard.setVisibility(newDashboardVisibility);
	        }
	        int cellMessageVisibility = showCellMessage ? View.VISIBLE : View.GONE;
	        if (mCellMessage.getVisibility() != cellMessageVisibility) {
	            mCellMessage.setVisibility(cellMessageVisibility);
	        }

	        mPB.setIndeterminate(indeterminate);
	        setButtonPausedState(paused);
		
	}
	
    private void initializeDownloadUI() 
    {
        mDownloaderClientStub = DownloaderClientMarshaller.CreateStub (this, GoogleDownloadService.class);
        setContentView(R.layout.main);

        mPB = (ProgressBar) findViewById(R.id.progressBar);
        mStatusText = (TextView) findViewById(R.id.statusText);
        mProgressFraction = (TextView) findViewById(R.id.progressAsFraction);
        mProgressPercent = (TextView) findViewById(R.id.progressAsPercentage);
        mAverageSpeed = (TextView) findViewById(R.id.progressAverageSpeed);
        mTimeRemaining = (TextView) findViewById(R.id.progressTimeRemaining);
        mDashboard = findViewById(R.id.downloaderDashboard);
        mCellMessage = findViewById(R.id.approveCellular);
        mPauseButton = (Button) findViewById(R.id.pauseButton);
        mWiFiSettingsButton = (Button) findViewById(R.id.wifiSettingsButton);

        mPauseButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mStatePaused) {
                    mRemoteService.requestContinueDownload();
                } else {
                    mRemoteService.requestPauseDownload();
                }
                setButtonPausedState(!mStatePaused);
            }
        });

        mWiFiSettingsButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
            }
        });

        Button resumeOnCell = (Button) findViewById(R.id.resumeOverCellular);
        resumeOnCell.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRemoteService.setDownloadFlags(IDownloaderService.FLAGS_DOWNLOAD_OVER_CELLULAR);
                mRemoteService.requestContinueDownload();
                mCellMessage.setVisibility(View.GONE);
            }
        });

    }
    
    private void setButtonPausedState(boolean paused) {
        mStatePaused = paused;
        int stringResourceID = paused ? R.string.text_button_resume :
                R.string.text_button_pause;
        mPauseButton.setText(stringResourceID);
    }
    
    private void setState(int newState) {
        if (mState != newState) {
            mState = newState;
            mStatusText.setText(Helpers.getDownloaderStringResourceIDFromState(newState));
        }
    }
    
    /**
     * Connect the stub to our service on start.
     */
    @Override
    protected void onStart() {
        if (null != mDownloaderClientStub) {
            mDownloaderClientStub.connect(this);
        }
        super.onStart();
    }

    /**
     * Disconnect the stub from our service on stop
     */
    @Override
    protected void onStop() {
        if (null != mDownloaderClientStub) {
            mDownloaderClientStub.disconnect(this);
        }
        super.onStop();
    }
    
    
    //--------------------------------------- AMAZOON GAME Circle Methods ----------------------------------//
    
    // Create a callback to handle initialization result codes.
    AmazonGamesCallback agsGameCallback = new AmazonGamesCallback() 
    {
    @Override
    public void onServiceReady() 
    {
    	IsAmazonServiceAvailable = true;
    }
    
    @Override
    public void onServiceNotReady(AmazonGamesStatus reason) {
        switch (reason) {
        case CANNOT_AUTHORIZE:
        {
            /**
             * The service could not authorize the client. This should only
             * occur if the network is not available the first time the game
             * attempts to connect.
             */
        	agsGameClient = null; //signal that amazon service is not available
        	break;
        }
        case CANNOT_BIND:
        {
            /**
            * The service could not authorize the client. This
            * should only occur if the network is not available the
            * first time the game attempts to connect.
            */
        	agsGameClient = null; //signal that amazon service is not available
        	break;
        }
        case NOT_AUTHENTICATED:
        {
            /**
             * The device is not registered with an account.
             */
        	agsGameClient = null; //signal that amazon service is not available
        	break;
        }
        case NOT_AUTHORIZED:
        {
            /**
             * The game in not authorized to use the service. Check your
             * package name and signature registered in the Developer's
             * Portal.
             */
        	agsGameClient = null; //signal that amazon service is not available
        	break;
        }
        case SERVICE_NOT_OPTED_IN:
        {
            /**
             * The device is not opted-in to use the service.
             */
        	agsGameClient = null; //signal that amazon service is not available
        	break;
        }
              
        default:
            break;
        }
    }
};
    
    
           
    //C++ Native methods
    public static native void AddRawTouchEvent(int aXLocation, int aYLocation, int aTypeEvent, int aFingerId);


}