/*
 * CGamePauseView.h
 *
 *  Created on: Sep 11, 2012
 *      Author: dstoll
 */

#ifndef CGAMEPAUSEVIEW_H_
#define CGAMEPAUSEVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CGameView.h"

class CGamePauseView : public CBasicView
{

public:

	virtual ~CGamePauseView();
	static CGamePauseView* New(CGameData& aGameData);

	//loads all the image through the imageStore that this view needs
	static void LoadImagesNeededForView();
	//unloads all the images from the imageStore that were needed for this view
	static void UnloadImagesNeededForView();

protected:
	CGamePauseView(CGameData& aGameData);
	void Construct();

	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//audio should be paused
	virtual void ContextGained();
	//audio should be paused
	virtual void ContextLost();

  enum TPauseMenuLabels {EPauseMenuNoneSelected, EPauseMenuReturnToGameLabel, EPauseMenuLoadGameLabel, EPauseMenuSaveGameLabel, EPauseMenuShowBattlefieldLabel, EPauseMenuGameSettingsLabel, EPauseMenuRestartMissionLabel, EPauseMenuExitToMainMenuLabel};
  enum TPauseMenuLoadGameLabels{EPauseMenuLoadGameLabelNoneSelected,EPauseMenuLoadGameLabelYes,EPauseMenuLoadGameLabelNo};
  enum TPauseMenuSaveGameLabels{EPauseMenuSaveGameLabelNoneSelected,EPauseMenuSaveGameLabelYes,EPauseMenuSaveGameLabelNo};
  enum TPauseMenuRestartMissionLabels{EPauseMenuRestartMissionLabelNoneSelected,EPauseMenuRestartMissionLabelYes,EPauseMenuRestartMissionLabelNo};
  enum TPauseMenuEndMissionLabels{EPauseMenuEndMissionLabelNoneSelected,EPauseMenuEndMissionLabelYes,EPauseMenuEndMissionLabelNo};
  enum TPauseMenuDepthLevel{EPauseMenuDepthLevelTopSelection,EPauseMenuDepthLevelLoadGameConfirm,EPauseMenuDepthLevelSaveGameConfirm,EPauseMenuDepthLevelSaveGameSuccessMessage,
                            EPauseMenuDepthLevelRestartMissionConfirm,EPauseMenuDepthLevelEndMissionConfirm};

  CGameView* iGameView;
	CTextureObject* iBackgroundTexture;
	CGameData& iGameData;
	SFont iFont;
	TBool iHasSaveGame;

	CString* iStringReturnToGame;
	CString* iStringRestartMission;
	CString* iStringExitToMainMenu;
	CString* iStringLoadGame;
	CString* iStringSaveGame;
	CString* iStringShowBattlefieldYes;
	CString* iStringShowBattlefieldNo;
	CString* iStringGameSettings;
	CString* iStringYes;
	CString* iStringNo;
	CString* iStringConfirmContinue;
	CString* iStringSaveGameWarning;
	CString* iStringSaveGameSuccessMessage;
	CString* iStringLoadGameWarning;
	CString* iStringRestartGameWarning;
	CString* iStringEndMissionWarning;
	CString* iStringEndMissionStatus;
	CString* iStringEndMissionFailed;
	CString* iStringEndMissionPending;
	CString* iStringEndMissionSuccess;

	TRect iReturnToGameRect;
	TRect iLoadGameRect;
	TRect iLoadGameYesRect;
	TRect iLoadGameNoRect;
	TRect iSaveGameRect;
	TRect iSaveGameYesRect;
	TRect iShowBattlefieldRect;
	TRect iSaveGameNoRect;
	TRect iGameSettingsRect;
	TRect iRestartMissionRect;
	TRect iRestartMissionYesRect;
	TRect iRestartMissionNoRect;
	TRect iEndMissionRect;
	TRect iEndMissionYesRect;
	TRect iEndMissionNoRect;

	TPauseMenuLabels iSelectedPauseMenuLabel;
	TPauseMenuDepthLevel iDepthLevel;
	TPauseMenuLoadGameLabels iLoadGameLabel;
	TPauseMenuSaveGameLabels iSaveGameLabel;
	TPauseMenuRestartMissionLabels iRestartMissionLabel;
	TPauseMenuEndMissionLabels iEndMissionLabel;

};

#endif /* CGAMEPAUSEVIEW_H_ */
