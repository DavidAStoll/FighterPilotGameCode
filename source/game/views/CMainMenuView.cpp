/*
 * CMainMenuView.cpp
 *
 *  Created on: Jul 6, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CMainMenuView.h"
#include "includes/game/views/CMainMenuExitView.h"
#include "includes/game/views/CChooseCampaignView.h"
#include "includes/game/views/CGameSettingsView.h"
#include "includes/game/views/CCreditsView.h"
#include "includes/game/views/CSelectLanguageView.h"
#include "includes/game/views/CHighscoresView.h"
#include "includes/game/views/CMissionBriefingView.h"
#include "includes/game/common/CUserStats.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CSystem.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlip.h"
#include "includes/core/views/transitions/CTransitionSlideAngled.h"
#include "includes/core/views/transitions/CTransitionSlideOnTop.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CMainMenuView::CMainMenuView(TBool aShowTitleScreen)
		: CBasicView()
{
	iActiveMenuSelection = ESelectNone;
	iShowTitleScreen = aShowTitleScreen;
	iBackgroundTexture = NULL;
}

CMainMenuView::~CMainMenuView()
{
	if (iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}

	//delete Strings
	delete iSelectCampaignString;
	delete iContinueLastSaveString;
	delete iGameSettingsString;
	delete iCheckHighscoresString;
	delete iSelectLanguageString;
	delete iAboutString;
	delete iExitString;
	delete iTouchScreenString;
}

CMainMenuView* CMainMenuView::New(TBool aShowTitleScreen)
{
	CMainMenuView* lSelf = new CMainMenuView(aShowTitleScreen);
	lSelf->Construct();
	return lSelf;
}

void CMainMenuView::Construct()
{
#if GENABLE_BUY_NOW_BUTTON
	//check if we have saved a game
	iLoadLastSaveEnabled = true;
#else
	//check if we have saved a game
	iLoadLastSaveEnabled = CFighterPilotThePacificWar::FighterGame->iGameVariables->iLastSaveGame != EGameMapNone;
#endif

	//Strings
#if GENABLE_BUY_NOW_BUTTON
	iContinueLastSaveString = iLocalizationEngine->GetString("Frontend.MainMenu.BuyNow");
#else
	iContinueLastSaveString = iLocalizationEngine->GetString("Frontend.MainMenu.ContinueFromLastSave");
#endif
	iSelectCampaignString = iLocalizationEngine->GetString("Frontend.MainMenu.SelectCampaign");
	iGameSettingsString = iLocalizationEngine->GetString("Frontend.MainMenu.GameSettings");
	iCheckHighscoresString = iLocalizationEngine->GetString("Frontend.MainMenu.CheckHighscores");
	iSelectLanguageString = iLocalizationEngine->GetString("Frontend.MainMenu.SelectLanguage");
	iAboutString = iLocalizationEngine->GetString("Frontend.MainMenu.Credits");
	iExitString = iLocalizationEngine->GetString("Frontend.MainMenu.Exit");
	iTouchScreenString = iLocalizationEngine->GetString("Frontend.MainMenu.TabScreen");

	//find proper Fonts
	//menu selection
	CPointerArray<CString>* lMenuSelectionStrings = CPointerArray<CString>::New();
	lMenuSelectionStrings->Append(iSelectCampaignString);
	lMenuSelectionStrings->Append(iContinueLastSaveString);
	lMenuSelectionStrings->Append(iGameSettingsString);
	lMenuSelectionStrings->Append(iCheckHighscoresString);
	lMenuSelectionStrings->Append(iSelectLanguageString);
	lMenuSelectionStrings->Append(iAboutString);
	lMenuSelectionStrings->Append(iExitString);
	iFontMenuSelection = CGame::Game->iGraphicsFont->CreateFontThatFitsHorizontalStrings(lMenuSelectionStrings, 900, 40);
	//not needed anymore
	lMenuSelectionStrings->Clear();
	delete lMenuSelectionStrings;

	//Touch Screen reminder
	iFontForTouchScreenReminder = CGame::Game->iGraphicsFont->CreateFontThatFitsHorizontalString(iTouchScreenString, 500, 100);

	//create Hitboxes for options
	iSelectCampaignRect = TRect(TPoint(62, 426), 900, 50);
	iContinueLastSaveRect = TRect(TPoint(62, 366), 900, 50);
	iGameSettingsRect = TRect(TPoint(62, 306), 900, 50);
	iCheckHighscoresRect = TRect(TPoint(62, 246), 900, 50);
	iSelectLanguageRect = TRect(TPoint(62, 186), 900, 50);
	iAboutRect = TRect(TPoint(62, 126), 900, 50);
	iExitRect = TRect(TPoint(62, 66), 900, 50);
	iGooglePlusRect = TRect(TPoint(950, 630), 60, 60);
	iFacebookRect = TRect(TPoint(950, 540), 60, 60);

	iFacebookSelected = false;
	iGooglePlusSelected = false;


	//background Texture
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0),
			&CGame::Game->iImageStore->GetImage(EImageIdFrontendMainMenuBackgroundScreen), 1024, 660, 100, 0, 1, 0, 1);

	//user Stat
	CUserStats::MainMenu();
}

//------------------------- functions ------------------------//

void CMainMenuView::ChildViewDrawMethod()
{
	if (iShowTitleScreen)
	{
		iBackgroundTexture->AssignTexture(&iImageStore->GetImage(EImageIdFrontendTileScreen));
		iBackgroundTexture->DrawArrays();

		//draw Touch Screen Reminder
		TInt lWidthOfString = iGraphicsFont->GetWidthOfString(iFontForTouchScreenReminder, iTouchScreenString);
		TPoint lDrawLocation = TPoint((1024 - lWidthOfString) / 2, 100);
		iGraphicsFont->SetFont(iFontForTouchScreenReminder);
		iGraphicsFont->SetColour(KColourDarkBlue);
		iGraphicsFont->DrawStringToScreenHorizontally(lDrawLocation, iTouchScreenString);
	}
	else
	{
		iBackgroundTexture->AssignTexture(&iImageStore->GetImage(EImageIdFrontendMainMenuBackgroundScreen));
		iBackgroundTexture->DrawArrays();

		//draw Menu Selection
		iGraphicsFont->SetFont(iFontMenuSelection);
		TPoint lStartLocation(62, 376);

		//Select Campaing
		if(iActiveMenuSelection == ESelectCampaign) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iSelectCampaignString, 900, 50);
		//Last Save
		lStartLocation.iY -= 60;
		if(!iLoadLastSaveEnabled) iGraphicsFont->SetColour(KColourDarkGray);
		else if(iActiveMenuSelection == ESelectContinueCampaign) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iContinueLastSaveString, 900, 50);
		//Game Setting
		lStartLocation.iY -= 60;
		if(iActiveMenuSelection == ESelectGameSettings) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iGameSettingsString, 900, 50);
		//HighScore
		lStartLocation.iY -= 60;
		if(iActiveMenuSelection == ESelectCheckHighscore) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iCheckHighscoresString, 900, 50);
		//Language
		lStartLocation.iY -= 60;
		if(iActiveMenuSelection == ESelectLanguage) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iSelectLanguageString, 900, 50);
		//About
		lStartLocation.iY -= 60;
		if(iActiveMenuSelection == ESelectAbout) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iAboutString, 900, 50);
		//Exit
		lStartLocation.iY -= 60;
		if(iActiveMenuSelection == ESelectExit) iGraphicsFont->SetColourSkyBlue(); else iGraphicsFont->SetColourWhite();
		DrawCenteredString(lStartLocation, iFontMenuSelection, iExitString, 900, 50);
	}
}

void CMainMenuView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			case ETouchEventHandlerTouchTypeDrag:
			{
				if(iShowTitleScreen)
				{
					//do nothing
				}
				else if(iSelectCampaignRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection =  ESelectCampaign;
				}
				else if(iContinueLastSaveRect.Contains(aTouchEvent.VirtualLocation) && iLoadLastSaveEnabled)
				{
					iActiveMenuSelection = ESelectContinueCampaign;
				}
				else if(iGameSettingsRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection = ESelectGameSettings;
				}
				else if(iCheckHighscoresRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection = ESelectCheckHighscore;
				}
				else if(iSelectLanguageRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection = ESelectLanguage;
				}
				else if(iAboutRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection = ESelectAbout;
				}
				else if(iExitRect.Contains(aTouchEvent.VirtualLocation))
				{
					iActiveMenuSelection = ESelectExit;
				}
				else if(iGooglePlusRect.Contains(aTouchEvent.VirtualLocation))
				{
					iGooglePlusSelected = true;
				}
				else if(iFacebookRect.Contains(aTouchEvent.VirtualLocation))
				{
					iFacebookSelected = true;
				}
				else
				{
					iActiveMenuSelection = ESelectNone;
					iFacebookSelected = false;
					iGooglePlusSelected = false;
				}

				break;
			}

			case ETouchEventHandlerTouchTypeUp:
			{
				if(iShowTitleScreen)
				{
					iShowTitleScreen = false; //show menu selection
				}
				else if(iActiveMenuSelection ==  ESelectCampaign && iSelectCampaignRect.Contains(aTouchEvent.VirtualLocation))
				{
					//go to campaign
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CChooseCampaignView::New(), 2, 1, 4);
				}
				else if(iActiveMenuSelection == ESelectContinueCampaign && iContinueLastSaveRect.Contains(aTouchEvent.VirtualLocation))
				{
#if	GENABLE_BUY_NOW_BUTTON
					//go to Store Front Page
					CUserStats::BuyGameMenu();
					CSystem::OpenWebsite(STORE_URL);
#else
					//need to get Mision and Campaign for MapId
					TGameMap lMissionId = CFighterPilotThePacificWar::FighterGame->iGameVariables->iLastSaveGame;
					SCampaign* lCampaignToLoad = NULL;
					SMission* lMissionToLoad = NULL;

					for(TInt lIndex = 0; lIndex < CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->GetCount(); lIndex++)
					{
						if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MapId == lMissionId)
						{
							lMissionToLoad = &CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission1;
						}
						else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MapId == lMissionId)
						{
							lMissionToLoad = &CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission2;
						}
						else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MapId == lMissionId)
						{
							lMissionToLoad = &CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission3;
						}
						else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MapId == lMissionId)
						{
							lMissionToLoad = &CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission4;
						}

						if(lMissionToLoad != NULL)
						{
							lCampaignToLoad = CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex);
							break;
						}
					}

					//load last save
					CUserStats::ContinueFromLastSaveMenuOpenedFrontend();
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMissionBriefingView::New(lCampaignToLoad, lMissionToLoad, true), 2, 4, 1, 20);
#endif
				}
				else if(iActiveMenuSelection == ESelectGameSettings && iGameSettingsRect.Contains(aTouchEvent.VirtualLocation))
				{
					//go to game settings
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CGameSettingsView::New(false), 2, 2, 2, 45);
				}
				else if(iActiveMenuSelection == ESelectCheckHighscore && iCheckHighscoresRect.Contains(aTouchEvent.VirtualLocation))
				{
					//go to Highscore Screen
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CHighscoresView::New(0), 2, 100, 1);
				}
				else if(iActiveMenuSelection == ESelectLanguage && iSelectLanguageRect.Contains(aTouchEvent.VirtualLocation))
				{
					//go to language
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CSelectLanguageView::New(false), 2, 1, 40);
				}
				else if(iActiveMenuSelection == ESelectAbout && iAboutRect.Contains(aTouchEvent.VirtualLocation))
				{
					//go to credits
					CTransitionSlideAngled::New(&CGame::Game->iView, this, CCreditsView::New(), 1, false);
				}
				else if(iActiveMenuSelection == ESelectExit && iExitRect.Contains(aTouchEvent.VirtualLocation))
				{
					//exit game
					CTransitionSlideOnTop::New(&CGame::Game->iView, this, CMainMenuExitView::New(), 0.5, false);

				}
				else if(iFacebookSelected && iFacebookRect.Contains(aTouchEvent.VirtualLocation))
				{
					//Go to Facebook page
					CSystem::OpenWebsite(URL_FACEBOOK);
					iFacebookSelected = false;
				}
				else if(iGooglePlusSelected && iGooglePlusRect.Contains(aTouchEvent.VirtualLocation))
				{
					//Go to GooglePlus page
					CSystem::OpenWebsite(URL_YOUTUBE);
					iGooglePlusSelected = false;
				}
				else
				{
					iActiveMenuSelection = ESelectNone;
					iFacebookSelected = false;
					iGooglePlusSelected = false;
				}

				break;
			}
		}
	}
}

