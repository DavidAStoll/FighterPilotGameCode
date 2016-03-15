/*
 * CLoadingScreen.h
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#ifndef CLOADINGSCREEN_H_
#define CLOADINGSCREEN_H_

#include "includes/core/views/CBasicView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"

enum TLoadingScreenMode
{
	ELoadingScreenModeStartMap,
	ELoadingScreenModeLoadMap,
	ELoadingScreenModeGoToVictoryScreen
};

class CLoadingScreen : public CBasicView
{
public:

	static CLoadingScreen* New(SCampaign* iCampaign, SMission* iMission, TLoadingScreenMode aMode);
	virtual ~CLoadingScreen();

protected:
	CLoadingScreen(SCampaign* iCampaign, SMission* iMission, TLoadingScreenMode aMode);
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();

	TImageId iRandomLoadImage;
	CTextureObject* iBackgroundTexture;
	TBool iReady;
	TBool iInitDone;
	TLoadingScreenMode iMode;
	SMission* iMission;
	SCampaign* iCampaign;
};

#endif /* CLOADINGSCREEN_H_ */
