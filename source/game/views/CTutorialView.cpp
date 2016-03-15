/*
 * CTutorialView.cpp
 *
 *  Created on: Oct 2, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CTutorialView.h"
#include "includes/game/views/CGameView.h"
#include "includes/game/views/CMissionVictoryView.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"

CTutorialView::CTutorialView()
{
	iDescriptionOffsetValue = 0;
}

CTutorialView::~CTutorialView()
{
	delete iBackgroundTexture;
	delete iTutorialTexture;
	delete iDescriptionBackground;

	delete iStringTutorialDescription;
	delete iStringSkip;
	delete iStringContinue;

	delete iButtonSkip;
	delete iButtonContinue;
	delete iScrollSurfaceDescription;
	delete iScrollBarDescription;
}

CTutorialView* CTutorialView::New()
{
	CTutorialView* lSelf = new CTutorialView();
	lSelf->Construct();
	return lSelf;
}

void CTutorialView::Construct()
{
	//need to get data from the current Tutorial Map
	CBasicTutorialMap* lTutorialMap = (CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap();
	const STutorialScreenData& lTutorialScreenData = lTutorialMap->GetTutorialScreenData();

	//get Method Calls
	iMethodCallWhenContinue = lTutorialScreenData.MethodCallIdIfContinue;
	iMethodCallWhenSkip = lTutorialScreenData.MethodCallIdIfSkip;

	//Texture Objects
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdTutorialScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackground = CTextureObject::New(TPoint(80, 100), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground),  865, 230, 100, 0, 1, 0, 1);
	iTutorialTexture = CTextureObject::New(TPoint(80, 340), &CGame::Game->iImageStore->GetImage(lTutorialScreenData.TutorialImage),  865, 280, 100, 0, 1, 0, 1);

	//Strings
	iStringTutorialDescription = iLocalizationEngine->GetString(lTutorialScreenData.DescriptionStringId->GetData());
	iStringSkip = iLocalizationEngine->GetString("Frontend.ButtonLabels.Skip");
	iStringContinue = iLocalizationEngine->GetString("Frontend.ButtonLabels.Continue");

	//Font
	iFontForDescription = iGraphicsFont->CreateFont(PREFERRED_FONT_SIZE_FOR_TEXT_IN_BOX);

	//get font for buttons
	CPointerArray<CString>* lStringButtonArray = CPointerArray<CString>::New();
	lStringButtonArray->Append(iStringSkip);
	lStringButtonArray->Append(iStringContinue);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lStringButtonArray, 190, 60);
	delete lStringButtonArray;

	//get Max Height for Description
	TInt lMaxYScrollForDescription = GetMaxHeightOfTextInBox(845, iFontForDescription, iStringTutorialDescription);
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

	//Skip Button
	CButton::SButtonArguments lSkipButtonArgs;
	lSkipButtonArgs.Location = TPoint(151, 0);
	lSkipButtonArgs.SizeOfHitbox = TSize(210, 85);
	lSkipButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lSkipButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lSkipButtonArgs.SizeOfTexture = TSize(210, 85);
	lSkipButtonArgs.Label = iStringSkip;
	lSkipButtonArgs.FontForLabel = &iFontForButtons;
	lSkipButtonArgs.ColourForLabelDefault = KColourBlack;
	lSkipButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonSkip = CButton::New(lSkipButtonArgs);

	//Continue Button
	CButton::SButtonArguments lContinueButtonArgs;
	lContinueButtonArgs.Location = TPoint(663, 0);
	lContinueButtonArgs.SizeOfHitbox = TSize(210, 85);
	lContinueButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lContinueButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lContinueButtonArgs.SizeOfTexture = TSize(210, 85);
	lContinueButtonArgs.Label = iStringContinue;
	lContinueButtonArgs.FontForLabel = &iFontForButtons;
	lContinueButtonArgs.ColourForLabelDefault = KColourBlack;
	lContinueButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonContinue = CButton::New(lContinueButtonArgs);
}

void CTutorialView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//ScrollSurfaces
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);

				//Back and Start Button
				iButtonSkip->CheckIfHit(aTouchEvent);
				iButtonContinue->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//ScrollSurfaces
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//ScrollSurfaces
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);

				//Back Button, go back to campaign View
				if(iButtonSkip->GetState() == CButton::EStateSelected && iButtonSkip->CheckIfHit(aTouchEvent))
				{
					//need keep track of Skip event
					CUserStats::TutorialSectionSkipped(iMethodCallWhenContinue - 1); // minus 1 to get the current section
					((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCallWhenSkip);
				}
				//Start the Mission
				if(iButtonContinue->GetState() == CButton::EStateSelected && iButtonContinue->CheckIfHit(aTouchEvent))
				{
					((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCallWhenContinue);
				}
				break;
			}
		}
	}
}

void CTutorialView::ChildViewDrawMethod()
{
	//draw Desciption Background
	iDescriptionBackground->DrawArrays();

	//draw Description
	iGraphicsFont->SetColour(KColourBlack);
	DrawStringIntoBox(TPoint(90, 320 + iDescriptionOffsetValue), 845, TRect(TPoint(80,330), TPoint(945,100)), iFontForDescription, iStringTutorialDescription);

	//draw Tutorial Image
	iTutorialTexture->DrawArrays();
	//draw Background
	iBackgroundTexture->DrawArrays();

	//controls
	iScrollBarDescription->Draw();
	iScrollSurfaceDescription->Draw();
	iButtonSkip->Draw();
	iButtonContinue->Draw();

}

