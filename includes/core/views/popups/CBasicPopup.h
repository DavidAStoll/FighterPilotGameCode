/*
 * CBasicPopup.h
 *
 *  Created on: Oct 14, 2012
 *      Author: dstoll
 */

#ifndef CBASICPOPUP_H_
#define CBASICPOPUP_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/transitions/CBasicTransition.h"
#include "includes/core/views/components/CButton.h"

struct SPopupBasicArgs
{
	CBasicView* OwningView;
	const char* LeftButtonStringId; //pass as NULL if not to be used
	const char* RightButtonStringId; //pass as NULL if not to be used
	const char* TitleStringId; //pass as NULL if not to be used
	void (*ReturnMethodCall) (TBool, void*);
	void* ReturnMethodArgs;
};

class CBasicPopup : public CBasicView
{

protected:
	//need to pass in the transition so we can set that the oldView does not get deleted otherwise we can't return to it
	CBasicPopup(const SPopupBasicArgs& aArgs);
	virtual ~CBasicPopup();
	//called with Constructor
	void Construct(const SPopupBasicArgs& aArgs);
	//this TouchEvent method should be overriden by the child view
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	//this draw Method should be overriden by the child view
	virtual void ChildViewDrawMethod();
	//should be called when the Popup finished(closes), either when one of the Buttons are pressed or due to some external event
	virtual void Finish(TBool aLeftButtonPressed);
	//has to be implemented by Popup child, its additional TouchEvent Code should be added here
	virtual void PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent) = 0;
	//has to be implemented by Popup child, its additional Draw Code should be added here
	virtual void PopupDrawViewExtraCode() = 0;

	CBasicView* iOwningView;
	void* iReturnMethod;
	void* iExtraArgs;

	CTextureObject* iPopUpBackground;

	CString* iStringLeftButtonLabel;
	CString* iStringRightButtonLabel;
	CString* iStringTitle;

	SFont iFontForButtons;
	SFont iFontForPrompt;

	CButton* iLeftButton;
	CButton* iRightButton;
};

#endif /* CBASICPOPUP_H_ */
