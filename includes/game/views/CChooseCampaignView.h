/*
 * CChooseCampaignView.h
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#ifndef CCHOOSECAMPAIGNVIEW_H_
#define CCHOOSECAMPAIGNVIEW_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/components/CTouchSurface.h"
#include "includes/core/views/components/CScrollBar.h"

class CChooseCampaignView : public CBasicView
{

public:
	~CChooseCampaignView();
	static CChooseCampaignView* New();

protected:
	CChooseCampaignView();
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	CTextureObject* iBackgroundTexture;
	CTextureObject* iSelectionBackgroundTexture;
	CTextureObject* iCampaignImage;
	CTextureObject* iCampaignBox;

	CButton** iCampaignButtons;
	CButton* iBackButton;

	CTouchSurface* iScrollSurface;
	CScrollBar* iScrollBar;

	CString* iStringChooseCampaign;
	CString* iStringBackButton;
	CString* iStringStartButtonLabel;
	CString* iStringLocked;
	CString** iStringCampaignNames;

	SFont iFontChooseCampaign;
	SFont iFontLocked;
	SFont iFontBackButton;
	SFont iFontCampaignNames;
	SFont iFontStartButton;

	TFloat iYOffsetValue;
	CPointerArray<SCampaign>* iCampaigns;

};

#endif /* CCHOOSECAMPAIGNVIEW_H_ */
