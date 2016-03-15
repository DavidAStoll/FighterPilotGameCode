/*
 * CHighscoresView.h
 *
 *  Created on: Oct 12, 2012
 *      Author: dstoll
 */

#ifndef CHighscoresView_H_
#define CHighscoresView_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CScrollBar.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/core/utility/CTCPConnectionEngine.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"


class CHighscoresView : public CBasicView
{
	struct SHighscoreBox
	{
		SMission* Mission;
		CString* MissionName;
		CString* Highscore;
		CButton* ViewScoreButton;
	};

public:

	static CHighscoresView* New(TInt aInitialYOffset);
	virtual ~CHighscoresView();

	static TInt InitialYOffset; //gonna keep it around, so that the user doesn't need to keep scroll from the top everytime

protected:
	CHighscoresView(TInt aInitialYOffset);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//updates the Highscores Boxes, should be called after new highscores have been downloaded
	void UpdateHighscoreBoxes();
	//method contains code to upload and download Highscores
	friend void* UploadScores(void* aExtraArgs);
	//needed for EnterText popup
	friend void EnterPlayerNameCallback(TBool aPlayerEnteredName, void* aExtraArgs);
	//needed for Popup progress
	friend void ProgressPopupCallBack(TBool aPlayerEnteredName, void* aExtraArgs);

private:
	//method used when uploading scores, creates a Highscore Message with the given MissionData and player name
	static SRemoteMessageInternalFormat CreateMissionHighscoreMessage(SMission& aMission, CString* aPlayerName);
	//method to update Highscore Table data, when new scores get downloaded
	static void UpdateHighscoreTable(CString* aMissionName, CString* aRank, CString* aPlayerName, CString* aScore, CString* aDate, CString* aTime);
	//method updates the best Overall Score
	static void UpdateOverallScoreEntry(const SMission& aMission, TInt& aTotalScore, TInt& aTotalSeconds);

protected:
	CTextureObject* iBackgroundTexture;
	CTextureObject* iSelectionBackgroundTexture;
	CTextureObject* iHighscoreBoxTexture;
	CTextureObject* iMissionImage;

	TFloat iYOffsetValue;

	CString* iStringBack;
	CString* iStringUpdate;
	CString* iStringViewScores;
	CString* iStringHighscore;
	CString* iStringGlobalHighscores;
	CString* iStringTempPlayerName;

	SFont iFontButtonBackAndUpdate;
	SFont iFontButtonViewScores;
	SFont iFontHighscores;
	SFont iFontMissionNames;
	SFont iFontGlobalHighscores;

	CTouchSurface* iScrollSurface;
	CScrollBar* iScrollBar;

	CButton* iButtonBack;
	CButton* iButtonUpdate;

	CPointerArray<SHighscoreBox>* iHighscoreBoxes;
	static TBool ServerNotRespondedError; //gonna use this to signal if something failed during upload
	static TBool GameOutdated;

};

#endif /* CHighscoresView_H_ */
