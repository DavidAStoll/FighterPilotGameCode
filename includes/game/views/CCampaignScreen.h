/*
 * CCampaignScreen.h
 *
 *  Created on: Jul 30, 2012
 *      Author: dstoll
 */

#ifndef CCAMPAIGNSCREEN_H_
#define CCAMPAIGNSCREEN_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/core/views/components/CScrollBar.h"

class CCampaignScreen : public CBasicView
{
	struct SMissionBox
	{
		SMission* Mission;
		CString* MissionName;
		CString* MissionScore;
		CButton* LoadButton;
		CButton* StartButton;
	};

public:
	~CCampaignScreen();
	static CCampaignScreen* New(SCampaign* aCampaign);

protected:
	CCampaignScreen(SCampaign* aCampaign);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iSelectionBackgroundTexture;
	CTextureObject* iMissionImage;
	CTextureObject* iMissionDifficultyImage;
	CTextureObject* iBriefingImage;
	CTextureObject* iMissionBox;

	TFloat iYOffsetValue;

	CTouchSurface* iScrollSurface;
	CScrollBar* iScrollBar;

	CString* iStringSelectMission;
	CString* iStringLocked;
	CString* iStringBackButton;
	CString* iStringStartButton;
	CString* iStringInfoButton;
	CString* iStringLoadButton;
	CString* iStringHighscore;
	CString* iStringBriefingScreen;
	CString* iStringDebriefingScreen;

	SFont iFontSelectMission;
	SFont iFontBackButton;
	SFont iFontLocked;
	SFont iFontForHighscore;
	SFont iFontForMissionNames;
	SFont iFontForStartAndLoadButton;
	SFont iFontForBigStartButton;

	CButton* iBackButton;
	CButton* iBriefingButton;
	CButton* iDebriefingButton;

	CPointerArray<SMissionBox>* iMissionBoxes;

	//keeps track what campaign we are displaying
	SCampaign* iCampaign;
};

#endif /* CCAMPAIGNSCREEN_H_ */
