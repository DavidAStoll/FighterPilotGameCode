package fighterPilot.RisingSunAlpha;

import android.media.MediaPlayer;

public class AndroidMediaPlayerCompleted implements MediaPlayer.OnCompletionListener

{
	//just a hack, we know that we want to exit the game so we just hardcode it
	public void onCompletion(MediaPlayer mp) 
	{
		FighterPilotActivity.GFighterPilotActivity.iGameThread.ExitGame();
	}

}
