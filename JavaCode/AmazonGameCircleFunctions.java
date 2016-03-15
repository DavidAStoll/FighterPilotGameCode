package fighterPilot.RisingSunAlpha;

import android.util.Log;

public class AmazonGameCircleFunctions {
	
	
	public static void UploadScoreToLeaderboard(String aLeaderboardId, int aScore)
	{
		if(FighterPilotActivity.IsAmazonServiceAvailable)
		{
			try
			{
				FighterPilotActivity.agsGameClient.getLeaderboardsClient().submitScore(aLeaderboardId, aScore);
			}
			catch(Exception e)
			{
				Log.d("Amazon Gamecircle Error!","Uploading Score to Amazon GameCircle caused an Exception!");
			}
		}
	}

}
