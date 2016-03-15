/*
 * CPopupProgress.h
 *
 *  Created on: Oct 15, 2012
 *      Author: dstoll
 */

#ifndef CPOPUPPROGRESS_H_
#define CPOPUPPROGRESS_H_

#include "includes/core/views/popups/CBasicPopup.h"

struct SPopupProgressBarArgs
{
	CBasicView* OwningView;
	const char* RightButtonStringId; //pass as NULL if not to be used
	const char* TitleStringId; //pass as NULL if not to be used
	void (*ReturnMethodCall) (TBool, void*);
	void* ReturnMethodArgs;
};

class CPopupProgressBar : public CBasicPopup
{
public:
	static CPopupProgressBar* New(const SPopupProgressBarArgs& aArgs, void* (*aMethodCall) (void*), void* aArgForMethod);

protected:
	CPopupProgressBar(const SPopupBasicArgs& aArgs);
	virtual ~CPopupProgressBar();
	//create Objects here
	void Construct(void* (*aMethodCall) (void*), void* aArgForMethod);

	//has to be implemented by Popup child, its additional TouchEvent Code should be added here
	virtual void PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent);
	//has to be implemented by Popup child, its additional Draw Code should be added here
	virtual void PopupDrawViewExtraCode();

	CTextureObject* iProgressBar;
	void* iWaitForMethodCall;
	TInt iNumberOfTimesCalled;
	TBool* iMethodCallComplete; //don't clean up in destructor, need to keep it around since User might close popup before Thread completes
	TBool* iMethodCallCompletedWithoutErrors;
};

#endif /* CPOPUPPROGRESS_H_ */
