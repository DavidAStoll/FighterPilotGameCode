/*
 * CMissionBriefingView.h
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#ifndef CMISSIONBRIEFINGVIEW_H_
#define CMISSIONBRIEFINGVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/core/graphic/COverSizedImage.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CRadioButtonGroup.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/core/views/components/CScrollBar.h"


class CMissionBriefingView : public CBasicView
{
public:

	static CMissionBriefingView* New(SCampaign* aCampaignData, SMission* aMissionData, TBool aLoadingSaveGame);
	virtual ~CMissionBriefingView();

protected:
	CMissionBriefingView(SCampaign* aCampaignData, SMission* aMissionData, TBool aLoadingSaveGame);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iDescriptionBackgroundTexture;
	COverSizedImage* iOverSizedImage;

	CString* iStringMissionTitle;
	CString* iStringBackButton;
	CString* iStringStartButton;
	CString* iStringDescription;
	CString* iStringBaby;
	CString* iStringEasy;
	CString* iStringMedium;
	CString* iStringHard;

	SFont iFontMissionTitle;
	SFont iFontForButtons;
	SFont iFontDescription;
	SFont iFontDifficulty;

	//Controls
	CButton* iBackButton;
	CButton* iStartButton;
	CRadioButtonGroup* iRadioButtonsDifficulty;
	CTouchSurface* iScrollSurfaceDescription;
	CScrollBar* iScrollBarDescription;
	CTouchSurface* iScrollSurfaceMap;
	CScrollBar* iScrollBarMap;

	SCampaign* iCampaignData;
	SMission* iMissionData;
	TBool iLoadingSaveGame;
	TFloat iDescriptionOffsetValue;
	TFloat iMapOffsetValue;
};

#endif /* CMISSIONBRIEFINGVIEW_H_ */
