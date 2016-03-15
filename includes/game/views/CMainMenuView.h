/*
 * CMainMenuView.h
 *
 *  Created on: Jul 6, 2012
 *      Author: dstoll
 */

#ifndef CMAINMENUVIEW_H_
#define CMAINMENUVIEW_H_

#include "includes/core/views/CBasicView.h"

class CMainMenuView : public CBasicView
{
	enum TMenuSelected
	{
		ESelectCampaign,
		ESelectContinueCampaign,
		ESelectGameSettings,
		ESelectCheckHighscore,
		ESelectLanguage,
		ESelectAbout,
		ESelectExit,
		ESelectNone
	};

public:
	~CMainMenuView();
	static CMainMenuView* New(TBool aShowTitleScreen);

protected:
	CMainMenuView(TBool aShowTitleScreen);
	void Construct();
	virtual void ChildViewDrawMethod();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);

	SFont iFontMenuSelection;
	SFont iFontForTouchScreenReminder;
	CString* iSelectCampaignString;
	CString* iContinueLastSaveString;
	CString* iGameSettingsString;
	CString* iCheckHighscoresString;
	CString* iSelectLanguageString;
	CString* iAboutString;
	CString* iExitString;
	CString* iTouchScreenString;
	TRect iSelectCampaignRect;
	TRect iContinueLastSaveRect;
	TRect iGameSettingsRect;
	TRect iCheckHighscoresRect;
	TRect iSelectLanguageRect;
	TRect iAboutRect;
	TRect iExitRect;
	TRect iFacebookRect;
	TRect iGooglePlusRect;
	TBool iFacebookSelected;
	TBool iGooglePlusSelected;
	TBool iShowTitleScreen;
	TBool iLoadLastSaveEnabled;
	TMenuSelected iActiveMenuSelection;
	CTextureObject* iBackgroundTexture;
};

#endif /* CMAINMENUVIEW_H_ */
