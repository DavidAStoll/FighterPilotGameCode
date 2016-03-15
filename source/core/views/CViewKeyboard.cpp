/*
 * CKeyboardView.cpp
 *
 *  Created on: Oct 16, 2012
 *      Author: dstoll
 */

#include "includes/core/views/CViewKeyboard.h"
#include "includes/core/views/transitions/CTransitionOpenDoorRotating.h"

CViewKeyboard::CViewKeyboard(CBasicView* aOwnerView, CString* aTextLabel, CString* aTextData, TInt aMaxNumberOfCharacters)
{
	iStringTextLabel = aTextLabel;
	iStringTextData = aTextData;
	iMaxNumberOfCharacters = aMaxNumberOfCharacters;
	iButtons = NULL;
	iKeyboardMode = EKeyboardModeText;
	iIsInUpperCase = false;
	iDrawCursor = true;
	iCursorLocation = TPoint(0,0);
	iFrameCount = 0;
	iOwnerView = aOwnerView;
}

CViewKeyboard::~CViewKeyboard()
{
	if(iStringTextLabel != NULL)
	{
		delete iStringTextLabel;
		iStringTextLabel = NULL;
	}
	for(TInt lIndex = 0; lIndex < iButtons->GetCount(); lIndex++)
	{
		SKeyboardButton* lKeyboardButton = iButtons->Get(lIndex);
		if(lKeyboardButton->TextLableUpperCase != NULL)
			delete lKeyboardButton->TextLableUpperCase;
		if(lKeyboardButton->TextLableLowerCase != NULL)
			delete lKeyboardButton->TextLableLowerCase;
		if(lKeyboardButton->NumberLableUpperCase != NULL)
			delete lKeyboardButton->NumberLableUpperCase;
		if(lKeyboardButton->NumberLableLowerCase != NULL)
			delete lKeyboardButton->NumberLableLowerCase;
		delete lKeyboardButton->Button;
	}
}

CViewKeyboard* CViewKeyboard::New(CBasicView* aOwnerView, CString* aTextLabel, CString* aTextData, TInt aMaxNumberOfCharacters)
{
	CViewKeyboard* lSelf = new CViewKeyboard(aOwnerView, aTextLabel, aTextData, aMaxNumberOfCharacters);
	lSelf->Construct();
	return lSelf;
}

void CViewKeyboard::Construct()
{
	iButtons = CPointerArray<SKeyboardButton>::New();

	//Font size should be small enough to fit all Buttons
	iFontButton = iGraphicsFont->CreateFont(50);
	iFontText = iGraphicsFont->CreateFont(40);

	//First Row
	AddButton(EKeyboardButtonTypeDefault, "Q", "q", ";", "1", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "W", "w", "#", "2", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "E", "e", "@", "3",  EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "R", "r", "¦", "4", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "T", "t", "/", "5", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "Y", "y", "_", "6", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "U", "u", "\"", "7", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "I", "i", "^", "8", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "O", "o", "~", "9", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeBackspace, NULL, NULL, NULL, NULL, EImageIdFrontendKeyboardBackspaceButton, EImageIdFrontendKeyboardBackspaceButtonSelected, 90);
	//Second Row
	AddButton(EKeyboardButtonTypeDefault, "A", "a", "`", "0", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "S", "s", "×", "$", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "D", "d", "÷", "%", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "F", "f", "{", "&", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "G", "g", "}", "*", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "H", "h", "[", "-", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "J", "j", "]", "+", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "K", "k", "•", "(", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "L", "l", "©", ")", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeNewLine, NULL, NULL, NULL, NULL, EImageIdFrontendKeyboardNewLineButton, EImageIdFrontendKeyboardNewLineButtonSelected, 90);
	//Third Row
	AddButton(EKeyboardButtonTypeDefault, "Z", "z", "®", "<", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "X", "x", "µ", ">", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "C", "c", "¢", "=", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "V", "v", "£", ":", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "B", "b", "¥", ".", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "N", "n", "§", ",", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "M", "m", "¡", "!", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeDefault, "P", "p", "¿", "?", EImageIdFrontendKeyboardDefaultButton, EImageIdFrontendKeyboardDefaultButtonSelected, 90);
	AddButton(EKeyboardButtonTypeCaps, NULL, NULL, NULL, NULL, EImageIdFrontendKeyboardCapsButton, EImageIdFrontendKeyboardCapsButtonSelected, 191);
	//Fourth Row
	AddButton(EKeyboardButtonTypeAccept, NULL, NULL, NULL, NULL, EImageIdFrontendKeyboardAcceptButton, EImageIdFrontendKeyboardAcceptButtonSelected, 191);
	AddButton(EKeyboardButtonTypeSpace, NULL, NULL, NULL, NULL, EImageIdFrontendKeyboardSpacebarButton, EImageIdFrontendKeyboardSpacebarButtonSelected, 505);
	AddButton(EKeyboardButtonTypeChangeMode, "123", "123", "ABC", "ABC", EImageIdFrontendKeyboardDefaultBigButton, EImageIdFrontendKeyboardDefaultBigButtonSelected, 191);
	//update button mode
	UpdateButtonLabels();
}

void CViewKeyboard::Start(CBasicTransition* aTransition)
{
	aTransition->SetDeleteOldView(false);
}

void CViewKeyboard::AddButton(TKeyboardButtonType aType, const char* aButtonLabelLetterUpperCase, const char* aButtonLabelLetterLowerCase, const char* aButtonLabelNumberUpperCase,const char* aButtonLabelNumberLowerCase, TImageId aDefaultImage, TImageId aSelectedImage, TInt aButtonWidth)
{
	//need to find out what location the new button has
	TPoint lButtonPosition = TPoint(0,0);
	if(iButtons->GetCount() == 0)
	{
		//first button
		lButtonPosition = TPoint(11, 248);
	}
	else
	{
		//get last added Button
		CButton* lLastButtonAdded = iButtons->Get(iButtons->GetCount() - 1)->Button;
		lButtonPosition = lLastButtonAdded->GetLowerLeftLocation();
		lButtonPosition.iX += lLastButtonAdded->GetSizeOfHitBox().iWidth + 11; //add offset of 11(space between each button)

		//check if we have to start on a new row
		if(lButtonPosition.iX > 1000)
		{
			lButtonPosition.iX = 11;
			lButtonPosition.iY -= 82;
		}
	}

	//Add Button
	SKeyboardButton lKeyboardButton;
	//set Type
	lKeyboardButton.ButtonType = aType;
	//Upper case
	if(aButtonLabelLetterUpperCase != NULL)
		lKeyboardButton.TextLableUpperCase = new CString(aButtonLabelLetterUpperCase);
	else
		lKeyboardButton.TextLableUpperCase = NULL;
	//Lower case
	if(aButtonLabelLetterLowerCase != NULL)
		lKeyboardButton.TextLableLowerCase = new CString(aButtonLabelLetterLowerCase);
	else
		lKeyboardButton.TextLableLowerCase = NULL;
	//Number Upper Case
	if(aButtonLabelNumberUpperCase != NULL)
		lKeyboardButton.NumberLableUpperCase = new CString(aButtonLabelNumberUpperCase);
	else
		lKeyboardButton.NumberLableUpperCase = NULL;
	//Number Lower Case
	if(aButtonLabelNumberLowerCase != NULL)
		lKeyboardButton.NumberLableLowerCase = new CString(aButtonLabelNumberLowerCase);
	else
		lKeyboardButton.NumberLableLowerCase = NULL;

	//Button
	CButton::SButtonArguments lButtonArgs;
	lButtonArgs.Location = lButtonPosition;
	lButtonArgs.SizeOfHitbox = TSize(aButtonWidth, 71);
	lButtonArgs.SizeOfTexture = TSize(aButtonWidth, 71);
	lButtonArgs.TextureIdDefault = &iImageStore->GetImage(aDefaultImage);
	lButtonArgs.TextureIdSelected = &iImageStore->GetImage(aSelectedImage);
	lButtonArgs.TextureIdDisabled = &iImageStore->GetImage(aDefaultImage);
	lButtonArgs.FontForLabel = &iFontButton;
	lButtonArgs.ColourForLabelDefault = KColourBlack;
	lButtonArgs.ColourForLabelSelected = KColourBlack;
	lButtonArgs.Label = lKeyboardButton.TextLableUpperCase;
	//Create Button Object
	lKeyboardButton.Button = CButton::New(lButtonArgs);

	//Add Button to list
	iButtons->Append(new SKeyboardButton(lKeyboardButton));
}

void CViewKeyboard::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				for(TInt lIndex = 0; lIndex < iButtons->GetCount(); lIndex++)
				{
					iButtons->Get(lIndex)->Button->CheckIfHit(aTouchEvent);
				}
				break;
			}

			case ETouchEventHandlerTouchTypeDrag:
			{
				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				for(TInt lIndex = 0; lIndex < iButtons->GetCount(); lIndex++)
				{
					if(iButtons->Get(lIndex)->Button->GetState() == CButton::EStateSelected && iButtons->Get(lIndex)->Button->CheckIfHit(aTouchEvent))
					{
						//Keyboard Button has been pressed
						SKeyboardButton* lKeyboardButton = iButtons->Get(lIndex);

						switch(lKeyboardButton->ButtonType)
						{
							case EKeyboardButtonTypeDefault:
							{
								if(iStringTextData->GetLength() < iMaxNumberOfCharacters)
								{
									if(iKeyboardMode == EKeyboardModeText)
									{
										if(iIsInUpperCase)
											iStringTextData->Append(lKeyboardButton->TextLableUpperCase);
										else
											iStringTextData->Append(lKeyboardButton->TextLableLowerCase);
									}
									else
									{
										if(iIsInUpperCase)
											iStringTextData->Append(lKeyboardButton->NumberLableUpperCase);
										else
											iStringTextData->Append(lKeyboardButton->NumberLableLowerCase);
									}
								}
								break;
							}
							case EKeyboardButtonTypeSpace:
							{
								if(iStringTextData->GetLength() < iMaxNumberOfCharacters)
								{
									iStringTextData->Append(" ");
								}
								break;
							}
							case EKeyboardButtonTypeChangeMode:
							{
								if(iKeyboardMode == EKeyboardModeText)
								{
									iKeyboardMode = EKeyboardModeNumbers;
								}
								else
								{
									iKeyboardMode = EKeyboardModeText;
								}
								iIsInUpperCase = false; //always go in lower case since it it is used needed more often
								UpdateButtonLabels();
								break;
							}
							case EKeyboardButtonTypeCaps:
							{
								iIsInUpperCase = !iIsInUpperCase;
								UpdateButtonLabels();
								break;
							}
							case EKeyboardButtonTypeBackspace:
							{
								if(iStringTextData->GetLength() > 0)
								{
									//remove last character
									if(iStringTextData->GetLength() == 1)
									{
										iStringTextData->Replace("");
									}
									else
									{
										iStringTextData->Replace(iStringTextData->GetSubString(0, iStringTextData->GetLength() - 1));
									}
								}
								break;
							}
							case EKeyboardButtonTypeAccept:
							case EKeyboardButtonTypeNewLine:
							{
								//done go back to previous view
								CTransitionOpenDoorRotating::New(&CGame::Game->iView, this, iOwnerView, 0.25, false);
								return;
							}
						}

						//done, reset cursor
						iFrameCount = 0;
						iDrawCursor = true;
						break;
					}
				}
				break;
			}
		}
	}
}

void CViewKeyboard::ChildViewDrawMethod()
{
	//textbackground
	iGraphicsEngine->SetClearColour(KColourWhite);
	iGraphicsEngine->ClearScreen();

	//draw Text
	TPoint lTextCursor = TPoint(10, 610); //start at the top of the screen
	iGraphicsFont->SetFont(iFontText);
	iGraphicsFont->SetColour(KColourBlack);
	//draw Label for Text
	if(iStringTextLabel != NULL)
	{
		iGraphicsFont->DrawStringToScreenHorizontally(lTextCursor, iStringTextLabel);
		lTextCursor.iY -= 50;
	}
	//draw Text, do a little hack, just gonna work with one line
	iGraphicsFont->DrawStringToScreenHorizontally(lTextCursor, iStringTextData);

	//draw Text Cursor
	iCursorLocation.iY = lTextCursor.iY;
	iCursorLocation.iX = lTextCursor.iX + iGraphicsFont->GetWidthOfString(iFontText, iStringTextData) + 2; //some minor addtional offset
	iGraphicsEngine->SetDrawColour(KColourBlack);
	iFrameCount++;
	if(iFrameCount > CGame::Game->iFramesPerSecondCounter->GetFramesPerSecond())
	{
		iDrawCursor = !iDrawCursor;
		iFrameCount = 0;
	}
	if(iDrawCursor)
	{
		iGraphicsEngine->FillRect(TRectIntFloat(TPointIntFloat(TIntFloat::Convert(iCursorLocation.iX), TIntFloat::Convert(iCursorLocation.iY + 40)), TIntFloat::Convert(5), TIntFloat::Convert(50)));
	}

	//draw KeyboardBackground
	TRectIntFloat lKeyboardBackground = TRectIntFloat(TPointIntFloat(TIntFloat::Convert(0),TIntFloat::Convert(330)), TIntFloat::Convert(1024), TIntFloat::Convert(330));
	iGraphicsEngine->FillRect(lKeyboardBackground);

	for(TInt lIndex = 0; lIndex < iButtons->GetCount(); lIndex++)
	{
		iButtons->Get(lIndex)->Button->Draw();
	}
}

void CViewKeyboard::UpdateButtonLabels()
{
	for(TInt lIndex = 0; lIndex < iButtons->GetCount(); lIndex++)
	{
		SKeyboardButton* lKeyboardButton = iButtons->Get(lIndex);

		if(iKeyboardMode == EKeyboardModeText)
		{
			if(iIsInUpperCase)
				lKeyboardButton->Button->SetLabel(lKeyboardButton->TextLableUpperCase);
			else
				lKeyboardButton->Button->SetLabel(lKeyboardButton->TextLableLowerCase);
		}
		else
		{
			if(iIsInUpperCase)
				lKeyboardButton->Button->SetLabel(lKeyboardButton->NumberLableUpperCase);
			else
				lKeyboardButton->Button->SetLabel(lKeyboardButton->NumberLableLowerCase);
		}
	}
}



