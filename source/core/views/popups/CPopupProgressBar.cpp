/*
 * CPopupProgress.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: dstoll
 */

#include "includes/core/views/popups/CPopupProgressBar.h"
#include "includes/core/utility/IPC.h"

void* MethodCallAndSettingFlag(void* lMethodAndArgs)
{
	//call actual Thread method
	void** lInt64Pointer = (void**) lMethodAndArgs;
	void* lMethod = (void*) lInt64Pointer[0]; //convert Int64 value to memory address
	void* lArg = (void*) lInt64Pointer[1];
	TBool* lFlag = (TBool*) lInt64Pointer[2];

	((void* (*)(void*))lMethod)(lArg);

	//set flag to true
	*lFlag = true;

	return NULL;
}


CPopupProgressBar::CPopupProgressBar(const SPopupBasicArgs& aArgs)
: CBasicPopup(aArgs)
{
	iMethodCallComplete = new TBool;
	*iMethodCallComplete = false;
	iNumberOfTimesCalled = 0;
}

CPopupProgressBar::~CPopupProgressBar()
{
	delete iProgressBar;
}

CPopupProgressBar* CPopupProgressBar::New(const SPopupProgressBarArgs& aArgs, void* (*aMethodCall) (void*), void* aArgForMethod)
{

	SPopupBasicArgs lArgsForBasicPopup = {aArgs.OwningView, NULL, aArgs.RightButtonStringId, aArgs.TitleStringId, aArgs.ReturnMethodCall, aArgs.ReturnMethodArgs};
	CPopupProgressBar* lSelf = new CPopupProgressBar(lArgsForBasicPopup);
	lSelf->Construct(aMethodCall, aArgForMethod);
	return lSelf;
}

void CPopupProgressBar::Construct(void* (*aMethodCall) (void*), void* aArgForMethod)
{
	//White Background for text
	iProgressBar = CTextureObject::New(TPoint(212, 300), &iImageStore->GetImage(EImageIdFrontendPopUpProgressBar), 600, 80, 0, 0, 1, 0 ,1);

	//create a new Thread to do the method call
	void** lMethodAndArg = new void* [3];
	lMethodAndArg[0] =  (void*) aMethodCall;
	lMethodAndArg[1] =  aArgForMethod;
	lMethodAndArg[2] = iMethodCallComplete; //needs reference to flag

	CNativeThread::CreateNativeThread(MethodCallAndSettingFlag, lMethodAndArg);
}

void CPopupProgressBar::PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent)
{
	//do nothing
}

void CPopupProgressBar::PopupDrawViewExtraCode()
{
	iProgressBar->DrawArrays();
	iProgressBar->ChangeSCoordinate(-0.02);
	iNumberOfTimesCalled++;

	//check if method call is complete
	if(*iMethodCallComplete && iNumberOfTimesCalled > 2) //needs to be called at least twice, 1 for transition, 1 for real view
	{
		delete iMethodCallComplete; //clean up boolean from heap
		Finish(true);
	}
}

