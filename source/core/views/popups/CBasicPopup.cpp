/*
 * CBasicPopup.cpp
 *
 *  Created on: Oct 14, 2012
 *      Author: dstoll
 */

#include "includes/core/views/popups/CBasicPopup.h"

CBasicPopup::CBasicPopup(const SPopupBasicArgs& aArgs)
{
	iStringLeftButtonLabel = NULL;
	iStringRightButtonLabel = NULL;
	iStringTitle = NULL;
	iOwningView = aArgs.OwningView;
	iReturnMethod = (void*) aArgs.ReturnMethodCall;
	iExtraArgs = aArgs.ReturnMethodArgs;
	Construct(aArgs);
}

CBasicPopup::~CBasicPopup()
{
	if(iPopUpBackground != NULL)
	{
		delete iPopUpBackground;
	}
	if(iStringLeftButtonLabel != NULL)
	{
		delete iStringLeftButtonLabel;
	}
	if(iStringRightButtonLabel != NULL)
	{
		delete iStringRightButtonLabel;
	}
	if(iStringTitle != NULL)
	{
		delete iStringTitle;
	}
}

void CBasicPopup::Construct(const SPopupBasicArgs& aArgs)
{
	iPopUpBackground = CTextureObject::New(TPoint(150, 100), &iImageStore->GetImage(EImageIdFrontendPopUpBackground), 724, 460, 0, 0, 1, 0 ,1);

	if(aArgs.LeftButtonStringId != NULL)
		iStringLeftButtonLabel = iLocalizationEngine->GetString(aArgs.LeftButtonStringId);
	if(aArgs.RightButtonStringId != NULL)
		iStringRightButtonLabel = iLocalizationEngine->GetString(aArgs.RightButtonStringId);
	if(aArgs.TitleStringId != NULL)
		iStringTitle = iLocalizationEngine->GetString(aArgs.TitleStringId);

	//-------------------- FONTS -------------------//
	if(iStringTitle != NULL)
		iFontForPrompt = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringTitle, 600, 70);

	//ForButtons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	if(iStringLeftButtonLabel != NULL)
		lButtonStringArray->Append(iStringLeftButtonLabel);
	if(iStringRightButtonLabel != NULL)
		lButtonStringArray->Append(iStringRightButtonLabel);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 330, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//Left Button
	CButton::SButtonArguments lLeftButtonArgs;
	lLeftButtonArgs.Location = TPoint(157, 106);
	lLeftButtonArgs.SizeOfHitbox = TSize(355, 80);
	lLeftButtonArgs.SizeOfTexture = TSize(355, 80);
	lLeftButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendPopUpButton);
	lLeftButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendPopUpButtonSelected);
	lLeftButtonArgs.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendPopUpButton);
	lLeftButtonArgs.Label = iStringLeftButtonLabel;
	lLeftButtonArgs.FontForLabel = &iFontForButtons;
	lLeftButtonArgs.ColourForLabelDefault = KColourWhite;
	lLeftButtonArgs.ColourForLabelSelected = KColourWhite;
	iLeftButton = CButton::New(lLeftButtonArgs);
	//check if we have to disable
	if(iStringLeftButtonLabel == NULL)
		iLeftButton->SetState(CButton::EStateDisabled);

	//Right Button
	CButton::SButtonArguments lRightButtonArgs;
	lRightButtonArgs.Location = TPoint(512, 106);
	lRightButtonArgs.SizeOfHitbox = TSize(355, 80);
	lRightButtonArgs.SizeOfTexture = TSize(355, 80);
	lRightButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendPopUpButton);
	lRightButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendPopUpButtonSelected);
	lRightButtonArgs.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendPopUpButton);
	lRightButtonArgs.Label = iStringRightButtonLabel;
	lRightButtonArgs.FontForLabel = &iFontForButtons;
	lRightButtonArgs.ColourForLabelDefault = KColourWhite;
	lRightButtonArgs.ColourForLabelSelected = KColourWhite;
	iRightButton = CButton::New(lRightButtonArgs);
	//check if we have to disable
	if(iStringRightButtonLabel == NULL)
		iRightButton->SetState(CButton::EStateDisabled);

	//change view to Popup
	CGame::Game->iView = this;
}

void CBasicPopup::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//Back and Start Button
				iRightButton->CheckIfHit(aTouchEvent);
				iLeftButton->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//Back Button, go back to Main Menu
				if(iLeftButton->GetState() == CButton::EStateSelected && iLeftButton->CheckIfHit(aTouchEvent))
				{
					//send scores
					Finish(true);
					return;
				}
				//Upload new Scores and Download the most recent ones
				if(iRightButton->GetState() == CButton::EStateSelected && iRightButton->CheckIfHit(aTouchEvent))
				{
					Finish(false);
					return;
				}
				break;
			}
		}

		//pass on to child
		PopupConsumeTouchEventExtraCode(aTouchEvent);
	}
}

void CBasicPopup::ChildViewDrawMethod()
{
	iOwningView->Draw();
	iPopUpBackground->DrawArrays();

	//draw title for popup
	if(iStringTitle != NULL)
	{
		iGraphicsFont->SetFont(iFontForPrompt);
		iGraphicsFont->SetColour(KColourWhite);
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(162, 490), iStringTitle);
	}

	//draw Buttons
	iLeftButton->Draw();
	iRightButton->Draw();

	//let the child draw
	PopupDrawViewExtraCode();
}

void CBasicPopup::Finish(TBool aLeftButtonPressed)
{
	//give control back to owning view
	CGame::Game->iView = iOwningView;
	//call method from owning view
	((void (*)(TBool, void*))iReturnMethod)(aLeftButtonPressed, iExtraArgs);
	//delete Popup
	delete this;
}
