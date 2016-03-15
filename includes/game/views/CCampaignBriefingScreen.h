/*
 * CCampaignBriefingScreen.h
 *
 *  Created on: Aug 5, 2012
 *      Author: dstoll
 */

#ifndef CCAMPAIGNBRIEFINGSCREEN_H_
#define CCAMPAIGNBRIEFINGSCREEN_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/core/views/components/CScrollBar.h"

class CCampaignBriefingScreen : public CBasicView
{
public:
	static CCampaignBriefingScreen* New(SBriefingScreen* aBriefingScreenData, SCampaign* aCampaignScreenToReturnTo);
	virtual ~CCampaignBriefingScreen();

protected:

	CCampaignBriefingScreen(SBriefingScreen* aBriefingScreenData, SCampaign* aCampaignScreenToReturnTo);
	void Construct();
	void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iImageTexture;
	CTextureObject* iDescriptionBackgroundTexture;

	TFloat iImageOffsetValue;
	TFloat iDescriptionOffsetValue;
	TSize iSizeOfImage;

	CString* iStringTitle;
	CString* iStringContinue;
	CString* iStringDescription;

	SFont iFontTitle;
	SFont iFontContinue;
	SFont iFontDescription;

	CTouchSurface* iScrollSurfaceImage;
	CScrollBar* iScrollBarImage;
	CTouchSurface* iScrollSurfaceDescription;
	CScrollBar* iScrollBarDescription;

	CButton* iContinueButton;

	SBriefingScreen* iBriefingScreenData;
	SCampaign* iCampaignScreenToReturnTo;
};

#endif /* CCAMPAIGNBRIEFINGSCREEN_H_ */
