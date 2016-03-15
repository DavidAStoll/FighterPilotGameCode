/*
 * CCampaignBriefingScreen.cpp
 *
 *  Created on: Aug 5, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CCampaignBriefingScreen.h"
#include "includes/game/views/CCampaignScreen.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"

CCampaignBriefingScreen::CCampaignBriefingScreen(SBriefingScreen* aBriefingScreenData, SCampaign* aCampaignScreenToReturnTo)
{
	iBriefingScreenData = aBriefingScreenData;
	iCampaignScreenToReturnTo = aCampaignScreenToReturnTo;
	iImageOffsetValue = 0;
	iDescriptionOffsetValue = 0;
}

CCampaignBriefingScreen::~CCampaignBriefingScreen()
{
	iImageStore->UnloadImage(iBriefingScreenData->BigMapImage);

	delete iBackgroundTexture;
	delete iImageTexture;
	delete iDescriptionBackgroundTexture;
	delete iStringTitle;
	delete iStringContinue;
	delete iStringDescription;

	delete iScrollSurfaceImage;
	delete iScrollBarImage;
	delete iScrollSurfaceDescription;
	delete iScrollBarDescription;
	delete iContinueButton;
}

CCampaignBriefingScreen* CCampaignBriefingScreen::New(SBriefingScreen* aBriefingScreenData, SCampaign* aCampaignScreenToReturnTo)
{
	CCampaignBriefingScreen* lSelf = new CCampaignBriefingScreen(aBriefingScreenData, aCampaignScreenToReturnTo);
	lSelf->Construct();
	return lSelf;
}

void CCampaignBriefingScreen::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendCampaignBriefingScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackgroundTexture = CTextureObject::New(TPoint(80, 100),&iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground), 865, 230, 100, 0, 1, 0, 1);

	//Strings
	iStringTitle = iLocalizationEngine->GetString(iBriefingScreenData->BriefingScreenNameId);
	iStringContinue = iLocalizationEngine->GetString("Frontend.ButtonLabels.Continue");
	iStringDescription = iLocalizationEngine->GetString(iBriefingScreenData->BriefingDescriptionId);

	//Fonts
	iFontTitle = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringTitle, 865, 30);
	iFontContinue = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringContinue, 190, 60);
	iFontDescription = iGraphicsFont->CreateFont(PREFERRED_FONT_SIZE_FOR_TEXT_IN_BOX);

	//need to load image first
	iImageStore->LoadImage(iBriefingScreenData->BigMapImage);
	iSizeOfImage = iImageStore->GetImageSize(iBriefingScreenData->BigMapImage);
	TInt lMaxYScrollForImage = iSizeOfImage.iHeight - 280; //280 is the height of the display area for the image
	iImageTexture = CTextureObject::New(TPoint(80, 340),&iImageStore->GetImage(iBriefingScreenData->BigMapImage), 865, 280, 100, 0, 1, 0, 1);

	//allocate Scroll Surfaces For the image
	//Scroll surface
	iScrollSurfaceImage = CTouchSurface::New(TPoint(0,340), TSize(945, 320),iImageOffsetValue, lMaxYScrollForImage, 2, CTouchSurface::EScrollAxisY);
	//ScrollBar
	CScrollBar::SScrollBarArguments lScrollBarImageArgs;
	lScrollBarImageArgs.Location = TPoint(960, 530);
	lScrollBarImageArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarImageArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarImageArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarImageArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarImageArgs.MaxScroll = lMaxYScrollForImage;
	lScrollBarImageArgs.MaxScrollHeight = 280;
	lScrollBarImageArgs.ScrollSpeedFactor = 4;
	lScrollBarImageArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBarImage = CScrollBar::New(lScrollBarImageArgs, iImageOffsetValue);

	//get Max Height for Description
	TInt lMaxYScrollForDescription = GetMaxHeightOfTextInBox(845, iFontDescription, iStringDescription);
	lMaxYScrollForDescription -= 230; //230 is the height of the display box for the description
	if(lMaxYScrollForDescription < 0)
		lMaxYScrollForDescription = 0;
	//allocate Scroll Surfaces For the Description
	//Scroll surface
	iScrollSurfaceDescription= CTouchSurface::New(TPoint(0,100), TSize(945, 230), iDescriptionOffsetValue, lMaxYScrollForDescription, 2, CTouchSurface::EScrollAxisY);
	//ScrollBar
	CScrollBar::SScrollBarArguments lScrollBarDescriptionArgs;
	lScrollBarDescriptionArgs.Location = TPoint(960, 240);
	lScrollBarDescriptionArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarDescriptionArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarDescriptionArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarDescriptionArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarDescriptionArgs.MaxScroll = lMaxYScrollForDescription;
	lScrollBarDescriptionArgs.MaxScrollHeight = 230;
	lScrollBarDescriptionArgs.ScrollSpeedFactor = 4;
	lScrollBarDescriptionArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBarDescription = CScrollBar::New(lScrollBarDescriptionArgs, iDescriptionOffsetValue);

	//Contiue Button
	CButton::SButtonArguments lContinueButtonArgs;
	lContinueButtonArgs.Location = TPoint(407, 0);
	lContinueButtonArgs.SizeOfHitbox = TSize(210, 85);
	lContinueButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lContinueButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lContinueButtonArgs.SizeOfTexture = TSize(210, 85);
	lContinueButtonArgs.Label = iStringContinue;
	lContinueButtonArgs.FontForLabel = &iFontContinue;
	lContinueButtonArgs.ColourForLabelDefault = KColourBlack;
	lContinueButtonArgs.ColourForLabelSelected = KColourBlack;
	iContinueButton = CButton::New(lContinueButtonArgs);

	//User Stat
	TInt lCampaignIndex = 0;
	for(TInt lIndex = 0; lIndex < CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		if(iCampaignScreenToReturnTo == CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex))
		{
			lCampaignIndex = lIndex;
			break;
		}
	}

	CUserStats::CampaignBriefingScreenOpenend(lCampaignIndex, iBriefingScreenData->IsDebriefingScreen);
}

void CCampaignBriefingScreen::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				iContinueButton->CheckIfHit(aTouchEvent);
				iScrollSurfaceImage->CheckIfHit(aTouchEvent);
				iScrollBarImage->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				break;
			}

			case ETouchEventHandlerTouchTypeDrag:
			{
				iScrollSurfaceImage->CheckIfHit(aTouchEvent);
				iScrollBarImage->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				iScrollSurfaceImage->CheckIfHit(aTouchEvent);
				iScrollBarImage->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);

				if(iContinueButton->GetState() == CButton::EStateSelected && iContinueButton->CheckIfHit(aTouchEvent))
				{

					if(iBriefingScreenData->IsDebriefingScreen)
					{
						CFighterPilotThePacificWar::FighterGame->iSaveGameObject->UnlockNextCampaign(iCampaignScreenToReturnTo);
					}
					else
					{
						iCampaignScreenToReturnTo->Mission1.Unlocked = true;
					}

					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignScreen::New(iCampaignScreenToReturnTo), 0.5 , false);
				}

				break;
			}
		}
	}
}

void CCampaignBriefingScreen::ChildViewDrawMethod()
{
	CFramesPerSecondCounter* lFramesPerSecondCounter = CGame::Game->iFramesPerSecondCounter;

	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//draw Background for Description
	iDescriptionBackgroundTexture->DrawArrays();

	//draw Description
	iGraphicsFont->SetColour(KColourBlack);
	DrawStringIntoBox(TPoint(90, 320 + iDescriptionOffsetValue), 845, TRect(TPoint(80,330), TPoint(945,100)), iFontDescription, iStringDescription);

	//draw Image for Map
	TFloat lTValueOffset = (TFloat) iImageOffsetValue / iSizeOfImage.iHeight;
	TFloat lAmountDisplayable = (TFloat) 280 / iSizeOfImage.iHeight; //280 is the height of the display box
	iImageTexture->SetNewTValue(lTValueOffset, lTValueOffset + lAmountDisplayable);
	iImageTexture->DrawArrays();

	//draw Screen Background
	iBackgroundTexture->DrawArrays();

	//draw Title
	iGraphicsFont->SetColour(KColourWhite);
	iGraphicsFont->SetFont(iFontTitle);
	DrawCenteredString(TPoint(80, 620), iFontTitle, iStringTitle, 865, 40);

	//draw Controls
	iScrollSurfaceImage->Draw();
	iScrollBarImage->Draw();
	iScrollSurfaceDescription->Draw();
	iScrollBarDescription->Draw();
	iContinueButton->Draw();
}


