/*
 * CGameSettingsView.h
 *
 *  Created on: Sep 13, 2012
 *      Author: dstoll
 */

#ifndef CGAMESETTINGSVIEW_H_
#define CGAMESETTINGSVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CCheckBoxButton.h"
#include "includes/core/views/components/CScrollBar.h"

class CGameSettingsView : public CBasicView
{

public:
	virtual ~CGameSettingsView();
	static CGameSettingsView* New(TBool aReturnToGameView);

	//loads all the image through the imageStore that this view needs
	static void LoadImagesNeededForView();
	//unloads all the images from the imageStore that were needed for this view
	static void UnloadImagesNeededForView();

protected:
	CGameSettingsView(TBool aReturnToGameView);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;

	CString* iStringCancelButton;
	CString* iStringSaveButton;
	CString* iStringChangeSongButton;
	CString* iStringUseIconsForButtons;

	SFont iFontForButtons;
	SFont iFontForOptions;

	TFloat iMusicPercentage;
	TFloat iSoundEffectsPercentage;
	TBool iReturnToGameView;

	CButton* iCancelButton;
	CButton* iSaveButton;
	CButton* iChangeSongButton;
	CCheckBoxButton* iUseIconsButton;
	CScrollBar* iScrollBarMusic;
	CScrollBar* iScrollBarSoundEffects;
};

#endif /* CGAMESETTINGSVIEW_H_ */
