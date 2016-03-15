/*
 * CAgreeToTermsScreen.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CAgreeToTermsScreen.h"
#include "includes/game/views/CHighscoresView.h"
#include "includes/game/views/CSelectLanguageView.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CAgreeToTermsScreen::CAgreeToTermsScreen(TBool aReturnToHighscoreScreen)
{
	iAgreementOffset = 0;
	iReturnToHighscoreScreen = aReturnToHighscoreScreen;
}

CAgreeToTermsScreen::~CAgreeToTermsScreen()
{
	// TODO Auto-generated destructor stub
}

CAgreeToTermsScreen* CAgreeToTermsScreen::New(TBool aReturnToHighscoreScreen)
{
	CAgreeToTermsScreen* lSelf = new CAgreeToTermsScreen(aReturnToHighscoreScreen);
	lSelf->Construct();
	return lSelf;
}

void CAgreeToTermsScreen::Construct()
{
	//need to have English Language, since only here it is defined
	iPreviousLanguage = CFighterPilotThePacificWar::FighterGame->iGameVariables->iLanguage;
	iLocalizationEngine->SetLanguage(ELanguageEnglish);

	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionVictoryScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackgroundTexture = CTextureObject::New(TPoint(64, 100), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground), 896, 280, 100, 0, 1, 0, 1);

	//Strings
	iStringYes = iLocalizationEngine->GetString("Frontend.ButtonLabels.Yes");;
	iStringNo = iLocalizationEngine->GetString("Frontend.ButtonLabels.No");;
	iStringAgreementTitle = iLocalizationEngine->GetString("Frontend.Agreement.Agreement_Title");;
	iStringAgreement = iLocalizationEngine->GetString("Frontend.Agreement.Agreement_Description");;

	//Font
	iFontForTitle= iGraphicsFont->CreateFontThatFitsHorizontalString(iStringAgreementTitle, 600, 30);
	iFontForAgreement = iGraphicsFont->CreateFont(30);

	TInt lMaxYOffset = GetMaxHeightOfTextInBox(876, iFontForAgreement, iStringAgreement);
	lMaxYOffset = CMath::Abs(lMaxYOffset - 280); //need to substract the box height

	//ForButtons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringNo);
	lButtonStringArray->Append(iStringYes);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//Restart Button
	CButton::SButtonArguments lNoButtonArgs;
	lNoButtonArgs.Location = TPoint(151, 0);
	lNoButtonArgs.SizeOfHitbox = TSize(210, 85);
	lNoButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lNoButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lNoButtonArgs.SizeOfTexture = TSize(210, 85);
	lNoButtonArgs.Label = iStringNo;
	lNoButtonArgs.FontForLabel = &iFontForButtons;
	lNoButtonArgs.ColourForLabelDefault = KColourBlack;
	lNoButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonNo = CButton::New(lNoButtonArgs);

	//Continue Button
	CButton::SButtonArguments lYesButtonArgs;
	lYesButtonArgs.Location = TPoint(663, 0);
	lYesButtonArgs.SizeOfHitbox = TSize(210, 85);
	lYesButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lYesButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lYesButtonArgs.SizeOfTexture = TSize(210, 85);
	lYesButtonArgs.Label = iStringYes;
	lYesButtonArgs.FontForLabel = &iFontForButtons;
	lYesButtonArgs.ColourForLabelDefault = KColourBlack;
	lYesButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonYes = CButton::New(lYesButtonArgs);

	//ScrollBar for Stats
	CScrollBar::SScrollBarArguments lScrollBarArgs;
	lScrollBarArgs.Location = TPoint(970, 290);
	lScrollBarArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarArgs.MaxScroll = lMaxYOffset;
	lScrollBarArgs.MaxScrollHeight = 280;
	lScrollBarArgs.ScrollSpeedFactor = 6;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iAgreementOffset);

	iTouchSurface = CTouchSurface::New(TPoint(0,100), TSize(960, 280), iAgreementOffset, lMaxYOffset, 2, CTouchSurface::EScrollAxisY);
}

//------------------ functions -------------------//
void CAgreeToTermsScreen::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//Touch Surfaces
				iScrollBar->CheckIfHit(aTouchEvent);
				iTouchSurface->CheckIfHit(aTouchEvent);

				//Buttons
				iButtonNo->CheckIfHit(aTouchEvent);
				iButtonYes->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//ScrollBar
				iScrollBar->CheckIfHit(aTouchEvent);
				iTouchSurface->CheckIfHit(aTouchEvent);

				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//ScrollBar
				iScrollBar->CheckIfHit(aTouchEvent);
				iTouchSurface->CheckIfHit(aTouchEvent);

				//go back to GameSettings, reset the old Song
				if (iButtonNo->GetState() == CButton::EStateSelected && iButtonNo->CheckIfHit(aTouchEvent))
				{
					//reset language
					iLocalizationEngine->SetLanguage(iPreviousLanguage);

					if(iReturnToHighscoreScreen)
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CHighscoresView::New(0), 0.5, false);
					}
					else
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CSelectLanguageView::New(true), 2, 1, 40);
					}
				}
				//return back to the previous screen
				if (iButtonYes->GetState() == CButton::EStateSelected && iButtonYes->CheckIfHit(aTouchEvent))
				{
					//reset language
					iLocalizationEngine->SetLanguage(iPreviousLanguage);

					//update agreement
					CFighterPilotThePacificWar::FighterGame->iGameVariables->iAgreementAccepted = true;
					CFighterPilotThePacificWar::FighterGame->iUserStatsTrackerDaemon->SetAgreedToUploadStats(true);
					CFighterPilotThePacificWar::FighterGame->iGameVariables->SaveSettingsToDisk();

					if(iReturnToHighscoreScreen)
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CHighscoresView::New(0), 0.5, false);
					}
					else
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CSelectLanguageView::New(true), 2, 1, 40);
					}
				}
				break;
			}
		}
	}
}

void CAgreeToTermsScreen::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//Background for Stats
	iDescriptionBackgroundTexture->DrawArrays();

	//draw Agreement
	iGraphicsFont->SetFont(iFontForAgreement);
	iGraphicsFont->SetColour(KColourBlack);

	DrawStringIntoBox(TPoint(70,370 + iAgreementOffset), 876, TRect(TPoint(70,380), 877, 280), iFontForAgreement, iStringAgreement);

	//draw Background Texture
	iBackgroundTexture->DrawArrays();

	//draw Title
	iGraphicsFont->SetFont(iFontForTitle);
	iGraphicsFont->SetColour(KColourDarkRed);
	DrawCenteredString(TPoint(200,420), iFontForTitle, iStringAgreementTitle, 625, 40);

	//draw Controls
	iButtonNo->Draw();
	iButtonYes->Draw();
	iScrollBar->Draw();
	iTouchSurface->Draw();

}

