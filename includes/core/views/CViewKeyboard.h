/*
 * CKeyboardView.h
 *
 *  Created on: Oct 16, 2012
 *      Author: dstoll
 */

#ifndef CVIEWKEYBOARD_H_
#define CVIEWKEYBOARD_H_

#include "includes/core/views/CBasicView.h"
#include "includes/core/views/components/CButton.h"
#include "includes/core/views/transitions/CBasicTransition.h"

class CViewKeyboard : public CBasicView
{
private:

	enum TKeyboardMode
	{
		EKeyboardModeText,
		EKeyboardModeNumbers,
	};

	enum TKeyboardButtonType
	{
		EKeyboardButtonTypeDefault,
		EKeyboardButtonTypeBackspace,
		EKeyboardButtonTypeSpace,
		EKeyboardButtonTypeNewLine,
		EKeyboardButtonTypeAccept,
		EKeyboardButtonTypeChangeMode,
		EKeyboardButtonTypeCaps
	};

	struct SKeyboardButton
	{
		TKeyboardButtonType ButtonType;
		CString* TextLableUpperCase;
		CString* TextLableLowerCase;
		CString* NumberLableUpperCase;
		CString* NumberLableLowerCase;
		CButton* Button;
	};

public:
	virtual ~CViewKeyboard();
	static CViewKeyboard* New(CBasicView* aOwnerView, CString* aTextLabel, CString* aTextData, TInt aMaxNumberOfCharacters);
	void Start(CBasicTransition* aTransition);

protected:
	CViewKeyboard(CBasicView* aOwnerView, CString* aTextLabel, CString* aTextData, TInt aMaxNumberOfCharacters);
	void Construct();

	//Appends a button to the ButtonList, makes sure that it is properly displayed on the keyboard(right X and Y offset)
	void AddButton(TKeyboardButtonType aType, const char* aButtonLabelLetterUpperCase, const char* aButtonLabelLetterLowerCase, const char* aButtonLabelNumberUpperCase,const char* aButtonLabelNumberLowerCase, TImageId aDefaultImage, TImageId aSelectedImage, TInt aButtonWidth);
	//this method is called from the framework if a new TouchEvent is available to be consumed
	virtual void ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent);
	virtual void ChildViewDrawMethod();
	//updates the labels of the Buttons depending on the current mode
	void UpdateButtonLabels();

	CString* iStringTextLabel; //gives the user a hint what should be written
	CString* iStringTextData; //contains the text written by the user

	CPointerArray<SKeyboardButton>* iButtons;

	SFont iFontButton;
	SFont iFontText;

	TInt iMaxNumberOfCharacters; //restrict how long the text can be
	TInt iFrameCount;
	TKeyboardMode iKeyboardMode;
	TBool iIsInUpperCase;
	TBool iDrawCursor;
	TPoint iCursorLocation;
	CBasicView* iOwnerView;
};

#endif /* CKEYBOARDVIEW_H_ */
