/*
 * CMainMenuExitView.h
 *
 *  Created on: Jul 8, 2012
 *      Author: dstoll
 */

#ifndef CMAINMENUEXITVIEW_H_
#define CMAINMENUEXITVIEW_H_

#include "includes/core/views/CBasicView.h"

class CMainMenuExitView : public CBasicView
{
	enum TSelected
	{
		ESelectYes,
		ESelectNo,
		ESelectNone
	};

public:
	virtual ~CMainMenuExitView();
	static CMainMenuExitView* New();



protected:
	CMainMenuExitView();
	void Construct();
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();


	SFont iFont;
	CString* iYesString;
	CString* iNoString;
	CString* iConfirmationMessage;
	TRect iYesRect;
	TRect iNoRect;
	TSelected iSelected;
	CTextureObject* iBackgroundTexture;
	//Needed to play back trialer
	TBool iIsPlayingVideo;
	TInt iFrame;

};

#endif /* CMAINMENUEXITVIEW_H_ */
