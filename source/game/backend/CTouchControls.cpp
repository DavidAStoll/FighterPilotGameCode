/*
 * CTouchControls.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/CTouchControls.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/CPlayer.h"

CTouchControls::CTouchControls(TBool aUseButtonIcons)
{
	iUseIconButtons = aUseButtonIcons;
}

CTouchControls::~CTouchControls()
{
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		delete iTouchButtons->Get(lIndex)->TextureObject;
		if(iTouchButtons->Get(lIndex)->ButtonLabel != NULL)
		{
			delete iTouchButtons->Get(lIndex)->ButtonLabel;
		}
	}
	iTouchButtons->ClearAndDestroy();
	delete iTouchButtons;
}

CTouchControls* CTouchControls::New(TBool aUseButtonIcons)
{
	CTouchControls* lSelf = new CTouchControls(aUseButtonIcons);
	lSelf->Construct();
	return lSelf;
}

void CTouchControls::Construct()
{
	CImageStore* lImageStore = CGame::Game->iImageStore;
	iTouchButtons = CPointerArray<STouchButton>::New();
	CPointerArray<CString>* lStringArray = CPointerArray<CString>::New();

	STouchButton lTouchButton;
	lTouchButton.ButtonState = EDefault;
	lTouchButton.FramesSincePushed = 0;
	lTouchButton.FingerId = -1; //not a possible Id
	lTouchButton.OutsideHud = false;
	lTouchButton.ButtonLabel = NULL;

	//--------------- Left Side Buttons -------------------//
	//add Fire Primary
	lTouchButton.ControlType = EGameControlTypeFirePrimary;
	lTouchButton.HitBox = TRect(TPoint(0,535), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonPrimaryFire);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonPrimaryFirePushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonPrimaryFireDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.Fire1");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(0,435), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Fire Secondary
	lTouchButton.ControlType = EGameControlTypeFireSecondary;
	lTouchButton.HitBox = TRect(TPoint(0,420), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonSecondaryFire);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonSecondaryFirePushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonSecondaryFireDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.Fire2");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(0,320), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Increase Speed
	lTouchButton.ControlType = EGameControlTypeIncreaseSpeed;
	lTouchButton.HitBox = TRect(TPoint(0,290), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeed);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeedPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeedDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.IncreaseSpeed");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(0,190), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Decrease Speed
	lTouchButton.ControlType = EGameControlTypeDecreaseSpeed;
	lTouchButton.HitBox = TRect(TPoint(0,175), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeed);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeedPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeedDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.DecreaseSpeed");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(0,75), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//--------------- Right Side Buttons -------------------//
	//add Up
	lTouchButton.ControlType = EGameControlTypeUp;
	lTouchButton.HitBox = TRect(TPoint(957,535), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonUp);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonUpPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonUpDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.TurnUp");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(957,435), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Down
	lTouchButton.ControlType = EGameControlTypeDown;
	lTouchButton.HitBox = TRect(TPoint(957,420), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonDown);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonDownPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonDownDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.TurnDown");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(957,320), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Left
	lTouchButton.ControlType = EGameControlTypeLeft;
	lTouchButton.HitBox = TRect(TPoint(957,290), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonLeft);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonLeftPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonLeftDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.TurnLeft");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(957,190), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//Add Right
	lTouchButton.ControlType = EGameControlTypeRight;
	lTouchButton.HitBox = TRect(TPoint(957,175), 67, 100);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonRight);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonRightPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonRightDisabled);
	lTouchButton.ButtonLabel = CGame::Game->iLocalizationEngine->GetString("Frontend.InGameControls.TurnRight");
	lStringArray->Append(lTouchButton.ButtonLabel);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(957,75), lTouchButton.TextureIdDefault, 67, 100, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//--------------- Outside HUD Buttons -------------------//
	lTouchButton.OutsideHud = true;
	lTouchButton.ButtonLabel = NULL; //these buttons have no label
	//Menu Button
	lTouchButton.ControlType = EGameControlTypePauseMenu;
	lTouchButton.HitBox = TRect(TPoint(825,585), 64, 64);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonMenu);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonMenuPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonMenuDisabled);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(825,521), lTouchButton.TextureIdDefault, 64, 64, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));
	//Zoom In Button
	lTouchButton.ControlType = EGameControlTypeZoomIn;
	lTouchButton.HitBox = TRect(TPoint(807,490), 100, 64);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonZoomIn);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonZoomInPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonZoomInDisabled);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(807,426), lTouchButton.TextureIdDefault, 100, 64, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));
	//Zoom In Button
	lTouchButton.ControlType = EGameControlTypeZoomOut;
	lTouchButton.HitBox = TRect(TPoint(807,395), 100, 64);
	lTouchButton.TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonZoomOut);
	lTouchButton.TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonZoomOutPushed);
	lTouchButton.TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonZoomOutDisabled);
	lTouchButton.TextureObject = CTextureObject::New(TPoint(807,331), lTouchButton.TextureIdDefault, 100, 64, 1, 0, 1, 0, 1);
	lTouchButton.FingerId = -1; //not a possible Id
	iTouchButtons->Append(new STouchButton(lTouchButton));

	//set what button images we should use
	SetUseIconButtons(iUseIconButtons);

	//find a good Font
	iFontForButtons = CGame::Game->iGraphicsFont->CreateFontThatFitsVerticalStrings(lStringArray, 57, 84);
	delete lStringArray;
}

void CTouchControls::Draw()
{
	CheckIfButtonsNeedToBeDisabled();
	//draw Black Rectangles for the controls
	glColor4f(0,0,0,1);
	//for Left Controls
	CGame::Game->iGraphicsEngine->FillRect(TRectIntFloat(TPointIntFloat(TIntFloat::Convert(0),TIntFloat::Convert(610)), TIntFloat::Convert(67), TIntFloat::Convert(610)));
	//for Right Controls
	CGame::Game->iGraphicsEngine->FillRect(TRectIntFloat(TPointIntFloat(TIntFloat::Convert(957),TIntFloat::Convert(610)), TIntFloat::Convert(67), TIntFloat::Convert(610)));

	//draw Buttons
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		STouchButton* lTouchButton = iTouchButtons->Get(lIndex);

		if(!lTouchButton->OutsideHud)
		{
			if(lTouchButton->ButtonState == EDefault)
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdDefault);
			}
			else if(lTouchButton->ButtonState == EPushed)
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdPushed);
			}
			else
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdDisabled);
			}

			lTouchButton->TextureObject->DrawArrays();

			if(!iUseIconButtons)
			{
				if(lTouchButton->ButtonLabel != NULL)
				{
					CGame::Game->iGraphicsFont->SetFont(iFontForButtons);
					if(lTouchButton->ButtonState == EDisabled)
					{
						CGame::Game->iGraphicsFont->SetColour(KColourDarkGray);
					}
					else
					{
						CGame::Game->iGraphicsFont->SetColourBlack();
					}

					//get Top Left location and center String inside the Button
					TInt lLeftMostX = lTouchButton->TextureObject->ReturnTopLeftCoordinate().iX.GetIntInBaseInt();
					TInt lTopY = lTouchButton->TextureObject->ReturnTopLeftCoordinate().iY.GetIntInBaseInt();
					//Center it
					lLeftMostX += (67 - iFontForButtons.HeightAndDescentOfFont.GetIntInBaseInt()) / 2;
					lTopY -= (100 - CGame::Game->iGraphicsFont->GetHeightOfVerticalString(iFontForButtons, lTouchButton->ButtonLabel)) / 2;
					//draw String
					CGame::Game->iGraphicsFont->DrawStringToScreenVertically(TPoint(lLeftMostX, lTopY), lTouchButton->ButtonLabel);
				}
			}
		}
	}
}

void CTouchControls::DrawButtonsOutsideHud()
{
	//draw Buttons that outside the Hud
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		STouchButton* lTouchButton = iTouchButtons->Get(lIndex);

		if(lTouchButton->OutsideHud)
		{
			if(lTouchButton->ButtonState == EDefault)
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdDefault);
			}
			else if(lTouchButton->ButtonState == EPushed)
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdPushed);
			}
			else
			{
				lTouchButton->TextureObject->AssignTexture(lTouchButton->TextureIdDisabled);
			}

			lTouchButton->TextureObject->DrawArrays();
		}
	}
}

void CTouchControls::HandleTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		STouchButton* lTouchButton = iTouchButtons->Get(lIndex);

		if(aTouchEvent.Type == ETouchEventHandlerTouchTypeDown)
		{
			if(lTouchButton->HitBox.Contains(aTouchEvent.VirtualLocation))
			{
				if(lTouchButton->FingerId == -1 && lTouchButton->ButtonState != CTouchControls::EDisabled)
				{
					//has no fingered associated yet
					lTouchButton->FramesSincePushed = 0;
					lTouchButton->FingerId = aTouchEvent.FingerId;
					lTouchButton->ButtonState = CTouchControls::EPushed;
					return;
				}
			}
		}
		else if(aTouchEvent.Type == ETouchEventHandlerTouchTypeDrag)
		{
			if(lTouchButton->ButtonState == CTouchControls::EDefault && lTouchButton->HitBox.Contains(aTouchEvent.VirtualLocation))
			{
				TBool lSetButton = true;

				//check if this finger has already a button touched
				for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
				{
					STouchButton* lOtherTouchButtons = iTouchButtons->Get(lIndex);
					if(lOtherTouchButtons->ButtonState ==  CTouchControls::EPushed && lOtherTouchButtons->FingerId == aTouchEvent.FingerId)
					{
						//this Finger already Touched another button
						lSetButton = false;
						break;
					}
				}

				if(lSetButton)
				{
					//no other touch buttons have been selected by this finger yet
					//select this touch Button with this finger now
					lTouchButton->FramesSincePushed = 0;
					lTouchButton->FingerId = aTouchEvent.FingerId;
					lTouchButton->ButtonState = CTouchControls::EPushed;
					return;
				}
			}
		}
		else if(aTouchEvent.Type == ETouchEventHandlerTouchTypeUp)
		{
			if(aTouchEvent.FingerId == lTouchButton->FingerId)
			{
				//reset button
				ResetButton(lTouchButton);
			}
		}
	}
}

TInt CTouchControls::GetFramesSincePushed(TGameControlType aControlType)
{
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		STouchButton* lTouchButton = iTouchButtons->Get(lIndex);

		if(lTouchButton->ControlType == aControlType)
		{
			if(lTouchButton->ButtonState == EPushed)
			{
				return lTouchButton->FramesSincePushed;
			}
			else
			{
				return -1;
			}
		}
	}

	return -1;
}

void CTouchControls::SetUseIconButtons(TBool aUseIconButtons)
{
	iUseIconButtons = aUseIconButtons;
	CImageStore* lImageStore = CGame::Game->iImageStore;

	if(iUseIconButtons)
	{
		//Fire Primary
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonPrimaryFire);
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonPrimaryFirePushed);
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonPrimaryFireDisabled);
		//Fire Secondary
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonSecondaryFire);
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonSecondaryFirePushed);
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonSecondaryFireDisabled);
		//Increase Speed
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeed);
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeedPushed);
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonIncreaseSpeedDisabled);
		//Decrease Speed
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeed);
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeedPushed);
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonDecreasedSpeedDisabled);
		//Up
		GetTouchButton(EGameControlTypeUp)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonUp);
		GetTouchButton(EGameControlTypeUp)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonUpPushed);
		GetTouchButton(EGameControlTypeUp)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonUpDisabled);
		//Down
		GetTouchButton(EGameControlTypeDown)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonDown);
		GetTouchButton(EGameControlTypeDown)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonDownPushed);
		GetTouchButton(EGameControlTypeDown)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonDownDisabled);
		//Left
		GetTouchButton(EGameControlTypeLeft)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonLeft);
		GetTouchButton(EGameControlTypeLeft)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonLeftPushed);
		GetTouchButton(EGameControlTypeLeft)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonLeftDisabled);
		//Right
		GetTouchButton(EGameControlTypeRight)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonRight);
		GetTouchButton(EGameControlTypeRight)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonRightPushed);
		GetTouchButton(EGameControlTypeRight)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonRightDisabled);
	}
	else
	{
		//Fire Primary
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankGreen);
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeFirePrimary)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Fire Secondary
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankGreen);
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeFireSecondary)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Increase Speed
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankRed);
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeIncreaseSpeed)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Decrease Speed
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankRed);
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeDecreaseSpeed)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Up
		GetTouchButton(EGameControlTypeUp)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankGreen);
		GetTouchButton(EGameControlTypeUp)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeUp)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Down
		GetTouchButton(EGameControlTypeDown)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankGreen);
		GetTouchButton(EGameControlTypeDown)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeDown)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Left
		GetTouchButton(EGameControlTypeLeft)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankRed);
		GetTouchButton(EGameControlTypeLeft)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeLeft)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
		//Right
		GetTouchButton(EGameControlTypeRight)->TextureIdDefault = &lImageStore->GetImage(EImageIdHudButtonBlankRed);
		GetTouchButton(EGameControlTypeRight)->TextureIdPushed = &lImageStore->GetImage(EImageIdHudButtonBlankPushed);
		GetTouchButton(EGameControlTypeRight)->TextureIdDisabled = &lImageStore->GetImage(EImageIdHudButtonBlankDisabled);
	}
}


void CTouchControls::Update()
{
	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		STouchButton* lTouchButton = iTouchButtons->Get(lIndex);

		if(lTouchButton->ButtonState == EPushed)
		{
			lTouchButton->FramesSincePushed++;
		}
	}
}


void CTouchControls::InitilizeTouchControls()
{
	CImageStore* lImageStore = CGame::Game->iImageStore;
	//load Images that we need for the controls
	//blanks
	lImageStore->LoadImage(EImageIdHudButtonBlankRed);
	lImageStore->LoadImage(EImageIdHudButtonBlankGreen);
	lImageStore->LoadImage(EImageIdHudButtonBlankPushed);
	lImageStore->LoadImage(EImageIdHudButtonBlankDisabled);
	//decrease Speed
	lImageStore->LoadImage(EImageIdHudButtonDecreasedSpeed);
	lImageStore->LoadImage(EImageIdHudButtonDecreasedSpeedPushed);
	lImageStore->LoadImage(EImageIdHudButtonDecreasedSpeedDisabled);
	//increase Speed
	lImageStore->LoadImage(EImageIdHudButtonIncreaseSpeed);
	lImageStore->LoadImage(EImageIdHudButtonIncreaseSpeedPushed);
	lImageStore->LoadImage(EImageIdHudButtonIncreaseSpeedDisabled);
	//Up
	lImageStore->LoadImage(EImageIdHudButtonUp);
	lImageStore->LoadImage(EImageIdHudButtonUpPushed);
	lImageStore->LoadImage(EImageIdHudButtonUpDisabled);
	//Down
	lImageStore->LoadImage(EImageIdHudButtonDown);
	lImageStore->LoadImage(EImageIdHudButtonDownPushed);
	lImageStore->LoadImage(EImageIdHudButtonDownDisabled);
	//Left
	lImageStore->LoadImage(EImageIdHudButtonLeft);
	lImageStore->LoadImage(EImageIdHudButtonLeftPushed);
	lImageStore->LoadImage(EImageIdHudButtonLeftDisabled);
	//Right
	lImageStore->LoadImage(EImageIdHudButtonRight);
	lImageStore->LoadImage(EImageIdHudButtonRightPushed);
	lImageStore->LoadImage(EImageIdHudButtonRightDisabled);
	//Fire Primary
	lImageStore->LoadImage(EImageIdHudButtonPrimaryFire);
	lImageStore->LoadImage(EImageIdHudButtonPrimaryFirePushed);
	lImageStore->LoadImage(EImageIdHudButtonPrimaryFireDisabled);
	//Fire Secondary
	lImageStore->LoadImage(EImageIdHudButtonSecondaryFire);
	lImageStore->LoadImage(EImageIdHudButtonSecondaryFirePushed);
	lImageStore->LoadImage(EImageIdHudButtonSecondaryFireDisabled);
	//MenuButton
	lImageStore->LoadImage(EImageIdHudButtonMenu);
	lImageStore->LoadImage(EImageIdHudButtonMenuPushed);
	lImageStore->LoadImage(EImageIdHudButtonMenuDisabled);
	//ZoomIn
	lImageStore->LoadImage(EImageIdHudButtonZoomIn);
	lImageStore->LoadImage(EImageIdHudButtonZoomInPushed);
	lImageStore->LoadImage(EImageIdHudButtonZoomInDisabled);
	//ZoomOut
	lImageStore->LoadImage(EImageIdHudButtonZoomOut);
	lImageStore->LoadImage(EImageIdHudButtonZoomOutPushed);
	lImageStore->LoadImage(EImageIdHudButtonZoomOutDisabled);
}

void CTouchControls::RemoveTouchControls()
{
	CImageStore* lImageStore = CGame::Game->iImageStore;
	//need to unload images as well
	//blanks
	lImageStore->UnloadImage(EImageIdHudButtonBlankRed);
	lImageStore->UnloadImage(EImageIdHudButtonBlankGreen);
	lImageStore->UnloadImage(EImageIdHudButtonBlankPushed);
	lImageStore->UnloadImage(EImageIdHudButtonBlankDisabled);
	//decrease Speed
	lImageStore->UnloadImage(EImageIdHudButtonDecreasedSpeed);
	lImageStore->UnloadImage(EImageIdHudButtonDecreasedSpeedPushed);
	lImageStore->UnloadImage(EImageIdHudButtonDecreasedSpeedDisabled);
	//increase Speed
	lImageStore->UnloadImage(EImageIdHudButtonIncreaseSpeed);
	lImageStore->UnloadImage(EImageIdHudButtonIncreaseSpeedPushed);
	lImageStore->UnloadImage(EImageIdHudButtonIncreaseSpeedDisabled);
	//Up
	lImageStore->UnloadImage(EImageIdHudButtonUp);
	lImageStore->UnloadImage(EImageIdHudButtonUpPushed);
	lImageStore->UnloadImage(EImageIdHudButtonUpDisabled);
	//Down
	lImageStore->UnloadImage(EImageIdHudButtonDown);
	lImageStore->UnloadImage(EImageIdHudButtonDownPushed);
	lImageStore->UnloadImage(EImageIdHudButtonDownDisabled);
	//Left
	lImageStore->UnloadImage(EImageIdHudButtonLeft);
	lImageStore->UnloadImage(EImageIdHudButtonLeftPushed);
	lImageStore->UnloadImage(EImageIdHudButtonLeftDisabled);
	//Right
	lImageStore->UnloadImage(EImageIdHudButtonRight);
	lImageStore->UnloadImage(EImageIdHudButtonRightPushed);
	lImageStore->UnloadImage(EImageIdHudButtonRightDisabled);
	//Fire Primary
	lImageStore->UnloadImage(EImageIdHudButtonPrimaryFire);
	lImageStore->UnloadImage(EImageIdHudButtonPrimaryFirePushed);
	lImageStore->UnloadImage(EImageIdHudButtonPrimaryFireDisabled);
	//Fire Secondary
	lImageStore->UnloadImage(EImageIdHudButtonSecondaryFire);
	lImageStore->UnloadImage(EImageIdHudButtonSecondaryFirePushed);
	lImageStore->UnloadImage(EImageIdHudButtonSecondaryFireDisabled);
	//MenuButton
	lImageStore->UnloadImage(EImageIdHudButtonMenu);
	lImageStore->UnloadImage(EImageIdHudButtonMenuPushed);
	lImageStore->UnloadImage(EImageIdHudButtonMenuDisabled);
	//ZoomIn
	lImageStore->UnloadImage(EImageIdHudButtonZoomIn);
	lImageStore->UnloadImage(EImageIdHudButtonZoomInPushed);
	lImageStore->UnloadImage(EImageIdHudButtonZoomInDisabled);
	//ZoomOut
	lImageStore->UnloadImage(EImageIdHudButtonZoomOut);
	lImageStore->UnloadImage(EImageIdHudButtonZoomOutPushed);
	lImageStore->UnloadImage(EImageIdHudButtonZoomOutDisabled);
}

void CTouchControls::CheckIfButtonsNeedToBeDisabled()
{
	//ZOOM IN
	if(CPlayer::ZoomFactor > 1)
	{
		if(GetTouchButton(EGameControlTypeZoomIn)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeZoomIn)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeZoomIn));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeZoomIn)->ButtonState = EDisabled;
	}
	//ZOOM OUT
	if(CPlayer::ZoomFactor < 2)
	{
		if(GetTouchButton(EGameControlTypeZoomOut)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeZoomOut)->ButtonState = EDefault;
		ResetButton(GetTouchButton(EGameControlTypeZoomOut));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeZoomOut)->ButtonState = EDisabled;
	}
	//Increase SPEED
	//GetPlayer Aircraft
	CAircraft* lPlayerAircraft = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetCurrentAircaft();
	if(lPlayerAircraft->GetCurrentEngineAcceleration() < lPlayerAircraft->GetMaxEngineAccelerationWithCurrentHealth())
	{
		if(GetTouchButton(EGameControlTypeIncreaseSpeed)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeIncreaseSpeed)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeIncreaseSpeed));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeIncreaseSpeed)->ButtonState = EDisabled;
	}
	//Decrease Speed
	if(lPlayerAircraft->GetCurrentEngineAcceleration() > 0)
	{
		if(GetTouchButton(EGameControlTypeDecreaseSpeed)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeDecreaseSpeed)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeDecreaseSpeed));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeDecreaseSpeed)->ButtonState = EDisabled;
	}
	//Left and Right
	//can't turn at all if at either 90 or 270 degrees
	if(*lPlayerAircraft->GetFlyingAngel() == 90 || *lPlayerAircraft->GetFlyingAngel() == 270)
	{
		GetTouchButton(EGameControlTypeLeft)->ButtonState = EDisabled;
		GetTouchButton(EGameControlTypeRight)->ButtonState = EDisabled;
	}
	else //if turned Left or Right, one of the buttons must be disabled
	{
		//reset both buttons
		if(GetTouchButton(EGameControlTypeLeft)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeLeft)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeLeft));
		}
		if(GetTouchButton(EGameControlTypeRight)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeRight)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeRight));
		}

		//check which one has to be disabled
		if(lPlayerAircraft->IsReflected())
		{
			GetTouchButton(EGameControlTypeRight)->ButtonState = EDisabled;
		}
		else
		{
			GetTouchButton(EGameControlTypeLeft)->ButtonState = EDisabled;
		}
	}
	//Secondary Fire
	if(lPlayerAircraft->GetSecondaryWeapon()->GetAmmunitionLeft() > 0 && lPlayerAircraft->IsFlying()) //need to check IsFlying because of next condition
	{
		if(GetTouchButton(EGameControlTypeFireSecondary)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeFireSecondary)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeFireSecondary));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeFireSecondary)->ButtonState = EDisabled;
	}
	//Primary and Secondary Fire
	//disable if not flying. Otherwise can cheat the game when on a landing surface
	if(lPlayerAircraft->IsFlying())
	{
		if(GetTouchButton(EGameControlTypeFirePrimary)->ButtonState == EDisabled)
		{
			GetTouchButton(EGameControlTypeFirePrimary)->ButtonState = EDefault;
			ResetButton(GetTouchButton(EGameControlTypeFirePrimary));
		}
	}
	else
	{
		GetTouchButton(EGameControlTypeFirePrimary)->ButtonState = EDisabled;
	}
}

CTouchControls::STouchButton* CTouchControls::GetTouchButton(TGameControlType aControlType)
{
	STouchButton* lTouchButton = NULL;

	for(TInt lIndex = 0; lIndex < iTouchButtons->GetCount(); lIndex++)
	{
		lTouchButton = iTouchButtons->Get(lIndex);

		if(lTouchButton->ControlType == aControlType)
		{
			break;
		}
	}

	return lTouchButton;
}


void CTouchControls::ResetButton(STouchButton* aButton)
{
	aButton->FramesSincePushed = 0;
	aButton->FingerId = -1;
	if(aButton->ButtonState != CTouchControls::EDisabled)
	{
		aButton->ButtonState = CTouchControls::EDefault;
	}
}


