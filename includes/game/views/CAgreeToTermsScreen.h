/*
 * CAgreeToTermsScreen.h
 *
 *  Created on: Oct 26, 2012
 *      Author: dstoll
 */

#ifndef CAGREETOTERMSSCREEN_H_
#define CAGREETOTERMSSCREEN_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CScrollBar.h"
#include "includes/core/views/components/CTouchSurface.h"

class CAgreeToTermsScreen : public CBasicView
{
public:

	static CAgreeToTermsScreen* New(TBool aReturnToHighscoreScreen);
	virtual ~CAgreeToTermsScreen();

protected:
	CAgreeToTermsScreen(TBool aReturnToHighscoreScreen);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iDescriptionBackgroundTexture;

	TFloat iAgreementOffset;
	TBool iReturnToHighscoreScreen;
	TLanguage iPreviousLanguage;

	CString* iStringYes;
	CString* iStringNo;
	CString* iStringAgreementTitle;
	CString* iStringAgreement;

	SFont iFontForButtons;
	SFont iFontForTitle;
	SFont iFontForAgreement;

	CButton* iButtonNo;
	CButton* iButtonYes;
	CScrollBar* iScrollBar;
	CTouchSurface* iTouchSurface;
};

#endif /* CAGREETOTERMSSCREEN_H_ */
