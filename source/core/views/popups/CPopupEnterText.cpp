/*
 * CPopupEnterText.cpp
 *
 *  Created on: Oct 14, 2012
 *      Author: dstoll
 */

#include "includes/core/views/popups/CPopupEnterText.h"
#include "includes/core/views/CViewKeyboard.h"
#include "includes/core/views/transitions/CTransitionOpenDoorRotating.h"

CPopupEnterText::CPopupEnterText(const SPopupBasicArgs& aArgs, CString* aStringData, TInt aMaxNumberOfCharacters)
: CBasicPopup(aArgs)
{
	iStringData = aStringData;
	iMaxNumberOfCharacters = aMaxNumberOfCharacters;
}

CPopupEnterText::~CPopupEnterText()
{
	delete iTextBackground;
}

CPopupEnterText* CPopupEnterText::New(const SPopupBasicArgs& aArgs, CString* aStringData, TInt aMaxNumberOfCharacters)
{
	CPopupEnterText* lSelf = new CPopupEnterText(aArgs, aStringData, aMaxNumberOfCharacters);
	lSelf->Construct();
	return lSelf;
}

void CPopupEnterText::Construct()
{
	//White Background for text
	iTextBackground = CTextureObject::New(TPoint(200, 340), &iImageStore->GetImage(EImageIdFrontendPopUpTextBackground), 624, 90, 0, 0, 1, 0 ,1);

	//Font
	iFontForText = iGraphicsFont->CreateFont(60);

	//hitbox to change text
	iTextBackgroundHitbox = TRect(TPoint(200, 430), 624, 90);
}

void CPopupEnterText::ChildRunLogicCode()
{
	if(iStringData->GetLength() > iMaxNumberOfCharacters || iStringData->GetLength() == 0)
	{
		iLeftButton->SetLabel(NULL);
		iLeftButton->SetState(CButton::EStateDisabled);
	}
	else if(iLeftButton->GetState() == CButton::EStateDisabled)
	{
		iLeftButton->SetLabel(iStringLeftButtonLabel);
		iLeftButton->SetState(CButton::EStateDefault);
	}
}

void CPopupEnterText::PopupConsumeTouchEventExtraCode(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				if(iTextBackgroundHitbox.Contains(aTouchEvent.VirtualLocation))
				{
					//go to keyboard view to change String
					CString* lStringTitle = NULL;
					if(iStringTitle != NULL)
						lStringTitle = new CString(iStringTitle->GetData());
					CViewKeyboard* lKeyboardView = CViewKeyboard::New(this, lStringTitle, iStringData, iMaxNumberOfCharacters);
					CBasicTransition* lTransition = CTransitionOpenDoorRotating::New(&CGame::Game->iView, this, lKeyboardView, 0.25, true);
					lKeyboardView->Start(lTransition);
					return;
				}
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				break;
			}
		}
	}
}

void CPopupEnterText::PopupDrawViewExtraCode()
{
	iTextBackground->DrawArrays();

	//draw String data
	iGraphicsFont->SetFont(iFontForText);
	iGraphicsFont->SetColour(KColourBlack);
	CString* lTextDisplayString = new CString(iStringData->GetData());
	if(iGraphicsFont->GetWidthOfString(iFontForText, lTextDisplayString) > 550)
	{
		//keep removing characters until it is short enough
		while(iGraphicsFont->GetWidthOfString(iFontForText, lTextDisplayString) > 550)
		{
			lTextDisplayString->Replace(lTextDisplayString->GetSubString(0, lTextDisplayString->GetLength() - 1));
		}

		//remove last three characters
		lTextDisplayString->Replace(lTextDisplayString->GetSubString(0, lTextDisplayString->GetLength() - 3));
		//add ... characters
		lTextDisplayString->Append("...");
	}
	iGraphicsFont->DrawStringToScreenHorizontally(TPoint(210, 360), lTextDisplayString);
	delete lTextDisplayString;
}


