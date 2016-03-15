/*
 * CTutorialView.h
 *
 *  Created on: Oct 2, 2012
 *      Author: dstoll
 */

#ifndef CTUTORIALVIEW_H_
#define CTUTORIALVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CScrollBar.h"
#include "includes/core/views/components/CTouchSurface.h"

class CTutorialView : public CBasicView
{
public:
	static CTutorialView* New();
	virtual ~CTutorialView();

protected:
	CTutorialView();
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iTutorialTexture;
	CTextureObject* iDescriptionBackground;

	CString* iStringTutorialDescription;
	CString* iStringSkip;
	CString* iStringContinue;

	SFont iFontForButtons;
	SFont iFontForDescription;

	TFloat iDescriptionOffsetValue;

	TTutorialMethodCallIds iMethodCallWhenContinue;
	TTutorialMethodCallIds iMethodCallWhenSkip;

	CButton* iButtonSkip;
	CButton* iButtonContinue;
	CTouchSurface* iScrollSurfaceDescription;
	CScrollBar* iScrollBarDescription;
};

#endif /* CTUTORIALVIEW_H_ */
