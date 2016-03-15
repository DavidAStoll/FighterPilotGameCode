/*
 * CChooseCampaignView.cpp
 *
 *  Created on: Jul 14, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CCampaignScreen.h"
#include "includes/game/views/CChooseCampaignView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
#include "includes/core/views/transitions/CTransitionSlideAngled.h"
#include "includes/core/views/transitions/CTransitionSlideOnTop.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"


CChooseCampaignView::CChooseCampaignView()
		: CBasicView()
{
	iYOffsetValue = 0;
}

CChooseCampaignView::~CChooseCampaignView()
{
	delete iBackgroundTexture;
	delete iSelectionBackgroundTexture;
	delete iCampaignImage;
	delete iCampaignBox;
	delete iStringBackButton;
	delete iStringChooseCampaign;
	delete iStringStartButtonLabel;
	delete iStringLocked;
	delete iBackButton;
	delete iScrollSurface;
	delete iScrollBar;

	for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
	{
		delete iStringCampaignNames[lIndex];
		delete iCampaignButtons[lIndex];
	}
	delete[] iStringCampaignNames;
	delete[] iCampaignButtons;
}

CChooseCampaignView* CChooseCampaignView::New()
{
	CChooseCampaignView* lSelf = new CChooseCampaignView();
	lSelf->Construct();
	return lSelf;
}

void CChooseCampaignView::Construct()
{
	iCampaigns = CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns;

	iBackgroundTexture = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenBackgroundScreen), 1024, 660, 100, 0, 1, 0, 1);
	iSelectionBackgroundTexture = CTextureObject::New(TPoint(80, 100),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenBackgroundScreenSelectionBackground), 865, 367, 100, 0, 1, 0, 1);
	iCampaignImage = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendChooseCampaignImageSingleDauntless), 610, 160, 100, 0, 1, 0, 1);
	iCampaignBox = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBox), 800, 200, 100, 0, 1, 0, 1);

	//Strings
	iStringChooseCampaign = iLocalizationEngine->GetString("Frontend.ChooseCampaign.ChooseCampaign");
	iStringLocked = iLocalizationEngine->GetString("Frontend.MissionStatus.Locked");
	iStringBackButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringStartButtonLabel = iLocalizationEngine->GetString("Frontend.ButtonLabels.Start");

	//Font
	iFontChooseCampaign = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringChooseCampaign, 300, 30);
	iFontLocked = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringLocked, 550, 140);
	iFontBackButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringBackButton, 190, 60);
	iFontStartButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringStartButtonLabel, 164, 140);

	//need to be dynamic
	iStringCampaignNames = new CString*[iCampaigns->GetCount()]; //need to find out somewhere else
	iCampaignButtons = new CButton*[iCampaigns->GetCount()];

	//Start Buttons
	CButton::SButtonArguments lStartButtonArgs;
	lStartButtonArgs.Location = TPoint(0, 0); //location is not important, since it will be reset all the time
	lStartButtonArgs.SizeOfHitbox = TSize(190, 160);
	lStartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButton);
	lStartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButtonSelected);
	lStartButtonArgs.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButton);
	lStartButtonArgs.SizeOfTexture = TSize(190, 160);
	lStartButtonArgs.Label = iStringStartButtonLabel;
	lStartButtonArgs.FontForLabel = &iFontStartButton;
	lStartButtonArgs.ColourForLabelDefault = KColourBlack;
	lStartButtonArgs.ColourForLabelSelected = KColourBlack;
	lStartButtonArgs.ColourForLabelDisabled = KColourDarkGray;

	for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
	{
		iStringCampaignNames[lIndex] =  iLocalizationEngine->GetString(iCampaigns->Get(lIndex)->CampaignNameId);
		iCampaignButtons[lIndex] = CButton::New(lStartButtonArgs);

		if(!iCampaigns->Get(lIndex)->Unlocked)
		{
			iCampaignButtons[lIndex]->SetState(CButton::EStateDisabled);
		}
	}

	//Font for Campaigns
	iFontCampaignNames = iGraphicsFont->CreateFontThatFitsHorizontalStrings(iStringCampaignNames, iCampaigns->GetCount(), 780, 30);


	//Back Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(407, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringBackButton;
	lBackButtonArgs.FontForLabel = &iFontBackButton;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iBackButton = CButton::New(lBackButtonArgs);


	TInt lMaxYValue = (20 + 230 * iCampaigns->GetCount()) - 360; //360 is the height of the display surface

	//Scroll surface
	iScrollSurface = CTouchSurface::New(TPoint(0,100), TSize(940, 360),iYOffsetValue, lMaxYValue, 2, CTouchSurface::EScrollAxisY);

	//ScrollBar
	CScrollBar::SScrollBarArguments lScrollBarArgs;
	lScrollBarArgs.Location = TPoint(960, 377);
	lScrollBarArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarArgs.MaxScroll = lMaxYValue;
	lScrollBarArgs.MaxScrollHeight = 368;
	lScrollBarArgs.ScrollSpeedFactor = 4;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iYOffsetValue);

	CUserStats::SelectCampaignMenuOpenedFrontend();
}

//------------------------- functions ------------------------//

void CChooseCampaignView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				iBackButton->CheckIfHit(aTouchEvent);
				iScrollSurface->CheckIfHit(aTouchEvent);
				iScrollBar->CheckIfHit(aTouchEvent);

				for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
				{
					iCampaignButtons[lIndex]->CheckIfHit(aTouchEvent);
				}
				break;
			}

			case ETouchEventHandlerTouchTypeDrag:
			{
				TBool lAnyStartButtonSelected = false;

				for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
				{
					if(iCampaignButtons[lIndex]->GetState() == CButton::EStateSelected)
					{
						lAnyStartButtonSelected = true;
						break;
					}
				}

				if(!lAnyStartButtonSelected) //don't want that scrolling happens if user selects a start button
				{
					iScrollSurface->CheckIfHit(aTouchEvent);
					iScrollBar->CheckIfHit(aTouchEvent);
				}

				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				iScrollSurface->CheckIfHit(aTouchEvent);
				iScrollBar->CheckIfHit(aTouchEvent);

				for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
				{
					if(iCampaignButtons[lIndex]->GetState() == CButton::EStateSelected && iCampaignButtons[lIndex]->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignScreen::New(iCampaigns->Get(lIndex)), 0.5, true);
						return;
					}
				}

				if(iBackButton->GetState() == CButton::EStateSelected && iBackButton->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(false), 2, 1, 4);
					return;
				}
				break;
			}
		}
	}
}

void CChooseCampaignView::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//draw Background for Campaign Boxes
	iSelectionBackgroundTexture->DrawArrays();

	//------------- Draw Camapign Boxes ----------------//
	TPoint lBoxLocation (112, 467 + iYOffsetValue); //want some space from the top as well
	lBoxLocation.iY -= 200; //need to make space for the first box
	lBoxLocation.iY -= 20; //some space between start


	for(TInt lIndex = 0; lIndex < iCampaigns->GetCount(); lIndex++)
	{
		//draw campaign box
		iCampaignBox->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iCampaignBox->DrawArrays();

		//draw campaign image
		iCampaignImage->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iCampaignImage->AssignTexture(&iImageStore->GetImage(iCampaigns->Get(lIndex)->CampaignImage));
		iCampaignImage->DrawArrays();

		//draw Locked Status if Campaign is still locked
		if(!iCampaigns->Get(lIndex)->Unlocked)
		{
			iGraphicsFont->SetFont(iFontLocked);
			iGraphicsFont->SetColour(KColourRed);
			DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY), iFontLocked, iStringLocked, 610, 160);
		}

		//draw Start Button
		CButton* lButton = iCampaignButtons[lIndex];
		lButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 610, lBoxLocation.iY));
		lButton->Draw();

		//draw Mission Name
		iGraphicsFont->SetFont(iFontChooseCampaign);
		iGraphicsFont->SetColour(KColourDarkBlue);
		DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY + 160), iFontCampaignNames, iStringCampaignNames[lIndex], 800, 40);

		//add space between boxes
		lBoxLocation.iY -= 200; //offset for box length
		lBoxLocation.iY -= 30; //space between boxes
	}


	//--------------- Draw Background And Controls -------------//

	iBackgroundTexture->DrawArrays();

	//draw Choose Campaign String
	iGraphicsFont->SetColour(KColourDarkRed);
	iGraphicsFont->SetFont(iFontChooseCampaign);
	DrawCenteredString(TPoint(360, 490), iFontChooseCampaign, iStringChooseCampaign, 310, 40);

	//Scroll Bar
	iScrollBar->Draw();

	//draw Back Button
	iBackButton->Draw();
}
