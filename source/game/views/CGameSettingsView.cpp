/*
 * CGameSettingsView.cpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CGameSettingsView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/views/CChangeSongView.h"
#include "includes/game/views/CGamePauseView.h"
#include "includes/game/views/CGameView.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"

CGameSettingsView::CGameSettingsView(TBool aReturnToGameView)
{
	iMusicPercentage = 0;
	iSoundEffectsPercentage = 0;
	iReturnToGameView = aReturnToGameView;
}

CGameSettingsView::~CGameSettingsView()
{
	if(iBackgroundTexture != NULL)
	{
		delete iBackgroundTexture;
		iBackgroundTexture = NULL;
	}

	delete iStringCancelButton;
	delete iStringSaveButton;
	delete iStringChangeSongButton;
	delete iStringUseIconsForButtons;

	delete iCancelButton;
	delete iSaveButton;
	delete iChangeSongButton;
	delete iUseIconsButton;
	delete iScrollBarMusic;
	delete iScrollBarSoundEffects;
}

CGameSettingsView* CGameSettingsView::New(TBool aReturnToGameView)
{
	CGameSettingsView* lSelf = new CGameSettingsView(aReturnToGameView);
	lSelf->Construct();
	return lSelf;
}

void CGameSettingsView::Construct()
{
	iMusicPercentage = CGame::Game->iAudioEngine->GetMusicPlayer().GetVolume();
	iSoundEffectsPercentage = CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->GetVolume();

	//create texture Object
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendGameSettingsScreen), 1024, 660, 100, 0, 1, 0, 1);

	//Strings
	iStringCancelButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Cancel");
	iStringSaveButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Save");
	iStringChangeSongButton = iLocalizationEngine->GetString("Frontend.GameSettings.ChangeSong");
	iStringUseIconsForButtons = iLocalizationEngine->GetString("Frontend.GameSettings.UseIcons");

	//--------------------------- Fonts -------------------------//
	//ForOptions
	CPointerArray<CString>* lOptionsStringArray = CPointerArray<CString>::New();
	lOptionsStringArray->Append(iStringChangeSongButton);
	lOptionsStringArray->Append(iStringUseIconsForButtons);
	iFontForOptions = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lOptionsStringArray, 550, 90);
	//clean up
	delete lOptionsStringArray;
	lOptionsStringArray = NULL;

	//ForButtons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringCancelButton);
	lButtonStringArray->Append(iStringSaveButton);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//--------------------------- Controls -------------------------//
	CScrollBar::SScrollBarArguments lScrollBarMusicArgs;
	lScrollBarMusicArgs.Location = TPoint(100, 521);
	lScrollBarMusicArgs.SizeOfHitbox = TSize(65, 97);
	lScrollBarMusicArgs.SizeOfTexture = TSize(65, 97);
	lScrollBarMusicArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenScrollBarButton);
	lScrollBarMusicArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenScrollBarButtonSelected);
	lScrollBarMusicArgs.MaxScroll = 100;
	lScrollBarMusicArgs.MaxScrollHeight = 641;
	lScrollBarMusicArgs.ScrollSpeedFactor = 0.2;
	lScrollBarMusicArgs.ScrollAxis = CScrollBar::EScrollAxisX;
	iScrollBarMusic = CScrollBar::New(lScrollBarMusicArgs, iMusicPercentage);

	CScrollBar::SScrollBarArguments lScrollBarSoundEffectsArgs;
	lScrollBarSoundEffectsArgs.Location = TPoint(100, 363);
	lScrollBarSoundEffectsArgs.SizeOfHitbox = TSize(65, 97);
	lScrollBarSoundEffectsArgs.SizeOfTexture = TSize(65, 97);
	lScrollBarSoundEffectsArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenScrollBarButton);
	lScrollBarSoundEffectsArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenScrollBarButtonSelected);
	lScrollBarSoundEffectsArgs.MaxScroll = 100;
	lScrollBarSoundEffectsArgs.MaxScrollHeight = 641;
	lScrollBarSoundEffectsArgs.ScrollSpeedFactor = 0.2;
	lScrollBarSoundEffectsArgs.ScrollAxis = CScrollBar::EScrollAxisX;
	iScrollBarSoundEffects = CScrollBar::New(lScrollBarSoundEffectsArgs, iSoundEffectsPercentage);

	//Use Icon Button
	CCheckBoxButton::SCheckBoxButtonArguments lCheckBoxButtonArgs;
	lCheckBoxButtonArgs.SizeOfHitbox = TSize(150, 90);
	lCheckBoxButtonArgs.SizeOfTexture = TSize(150, 90);
	lCheckBoxButtonArgs.Label = NULL;
	lCheckBoxButtonArgs.Location = TPoint(800, 250);
	lCheckBoxButtonArgs.TextureIdDefault = &CGame::Game->iImageStore->GetImage(EImageIdFrontendGameSettingsScreenUseButtonIcon);
	lCheckBoxButtonArgs.TextureIdSelected = &CGame::Game->iImageStore->GetImage(EImageIdFrontendGameSettingsScreenUseButtonIconChecked);
	lCheckBoxButtonArgs.TextureIdDisabled = &CGame::Game->iImageStore->GetImage(EImageIdFrontendGameSettingsScreenUseButtonIconChecked);
	iUseIconsButton = CCheckBoxButton::New(lCheckBoxButtonArgs);

	//setState of button, depinding if we use currently icons for buttons or not
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iUseIconButton)
	{
		iUseIconsButton->SetState(CCheckBoxButton::EStateSelected);
	}
	else
	{
		iUseIconsButton->SetState(CCheckBoxButton::EStateDefault);
	}

	//Change Song Button
	CButton::SButtonArguments lChangeSongButtonArgs;
	lChangeSongButtonArgs.Location = TPoint(212, 109);
	lChangeSongButtonArgs.SizeOfHitbox = TSize(600, 100);
	lChangeSongButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButton);
	lChangeSongButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected);
	lChangeSongButtonArgs.SizeOfTexture = TSize(600, 100);
	lChangeSongButtonArgs.Label = iStringChangeSongButton;
	lChangeSongButtonArgs.FontForLabel = &iFontForOptions;
	lChangeSongButtonArgs.ColourForLabelDefault = KColourBlack;
	lChangeSongButtonArgs.ColourForLabelSelected = KColourBlack;
	iChangeSongButton = CButton::New(lChangeSongButtonArgs);

	//Cancel Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(151, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringCancelButton;
	lBackButtonArgs.FontForLabel = &iFontForButtons;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iCancelButton = CButton::New(lBackButtonArgs);

	//Save Button
	CButton::SButtonArguments lStartButtonArgs;
	lStartButtonArgs.Location = TPoint(663, 0);
	lStartButtonArgs.SizeOfHitbox = TSize(210, 85);
	lStartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lStartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lStartButtonArgs.SizeOfTexture = TSize(210, 85);
	lStartButtonArgs.Label = iStringSaveButton;
	lStartButtonArgs.FontForLabel = &iFontForButtons;
	lStartButtonArgs.ColourForLabelDefault = KColourBlack;
	lStartButtonArgs.ColourForLabelSelected = KColourBlack;
	iSaveButton = CButton::New(lStartButtonArgs);

	if(iReturnToGameView)
	{
		CUserStats::GameSettingsMenuOpenedInGame();
	}
	else
	{
		CUserStats::GameSettingsMenuOpenedFrontend();
	}
}

//-------------------- functions ---------------------//
void CGameSettingsView::LoadImagesNeededForView()
{
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedMenuNavigationButton);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenChangeSongButton);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenUseButtonIcon);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenUseButtonIconChecked);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenScrollBarButton);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreenScrollBarButtonSelected);
	CGame::Game->iImageStore->LoadImage(EImageIdFrontendGameSettingsScreen);
}

void CGameSettingsView::UnloadImagesNeededForView()
{
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedMenuNavigationButton);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenChangeSongButton);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenUseButtonIcon);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenUseButtonIconChecked);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenScrollBarButton);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreenScrollBarButtonSelected);
	CGame::Game->iImageStore->UnloadImage(EImageIdFrontendGameSettingsScreen);
}

void CGameSettingsView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//ScrollBar
				iScrollBarMusic->CheckIfHit(aTouchEvent);
				iScrollBarSoundEffects->CheckIfHit(aTouchEvent);

				//Buttons
				iCancelButton->CheckIfHit(aTouchEvent);
				iSaveButton->CheckIfHit(aTouchEvent);
				iChangeSongButton->CheckIfHit(aTouchEvent);
				iUseIconsButton->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//ScrollBar
				iScrollBarMusic->CheckIfHit(aTouchEvent);
				iScrollBarSoundEffects->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//check if the Music or SoundEffects volume changed
				if(iScrollBarMusic->GetState() == CButton::EStateSelected)
				{
					//need to update Music Volume
					CGame::Game->iAudioEngine->GetMusicPlayer().SetVolume(iMusicPercentage);
				}
				if(iScrollBarSoundEffects->GetState() == CButton::EStateSelected)
				{
					//need to update Sound Effects
					CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->SetVolume(iSoundEffectsPercentage);
					CGame::Game->iAudioEngine->Play(ESoundFileId_Explosion_Large);
				}

				//ScrollBar
				iScrollBarMusic->CheckIfHit(aTouchEvent);
				iScrollBarSoundEffects->CheckIfHit(aTouchEvent);
				iUseIconsButton->CheckIfHit(aTouchEvent);

				//go Song Selection Screen
				if(iChangeSongButton->GetState() == CButton::EStateSelected && iChangeSongButton->CheckIfHit(aTouchEvent))
				{
					//update Music, because User might have adjusted it for the song
					CGame::Game->iAudioEngine->GetMusicPlayer().SetVolume(iMusicPercentage);
					//go to Song Selection Screen
					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CChangeSongView::New(iReturnToGameView), 0.5, true);
				}
				//go back to Main Menu, don't save changes
				if (iCancelButton->GetState() == CButton::EStateSelected && iCancelButton->CheckIfHit(aTouchEvent))
				{
					//reset to previous settings
					CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->SetVolume(CFighterPilotThePacificWar::FighterGame->iGameVariables->iSoundEffectsPercentage);
					CGame::Game->iAudioEngine->GetMusicPlayer().SetVolume(CFighterPilotThePacificWar::FighterGame->iGameVariables->iMusicPercentage);

					if(iReturnToGameView)
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CGamePauseView::New(*CFighterPilotThePacificWar::FighterGame->iGameData), 2, 2, 2, 45);
					}
					else
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(false), 2, 2, 2, 45);
					}
				}
				//go back to Main Menu, Save Changes
				if (iSaveButton->GetState() == CButton::EStateSelected && iSaveButton->CheckIfHit(aTouchEvent))
				{
					//update Music and Sound Effects
					CFighterPilotThePacificWar::FighterGame->iGameVariables->iMusicPercentage = iMusicPercentage;
					CFighterPilotThePacificWar::FighterGame->iGameVariables->iSoundEffectsPercentage = iSoundEffectsPercentage;
					//update Button Icons
					if(iUseIconsButton->GetState() == CCheckBoxButton::EStateSelected)
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iUseIconButton = true;
					}
					else
					{
						CFighterPilotThePacificWar::FighterGame->iGameVariables->iUseIconButton = false;
					}
					CFighterPilotThePacificWar::FighterGame->iGameVariables->SaveSettingsToDisk();

					//create user Events
					CUserStats::SoundEffectsVolumeChanged(!iReturnToGameView, iSoundEffectsPercentage);
					CUserStats::MusicVolumeChanged(!iReturnToGameView, iSoundEffectsPercentage);
					CUserStats::UseIconButtons(CFighterPilotThePacificWar::FighterGame->iGameVariables->iUseIconButton);

					if(iReturnToGameView)
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CGamePauseView::New(*CFighterPilotThePacificWar::FighterGame->iGameData), 2, 2, 2, 45);
					}
					else
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(false), 2, 2, 2, 45);
					}
				}
				break;
			}
		}
	}
}

void CGameSettingsView::ChildViewDrawMethod()
{
	//draw Background
	iBackgroundTexture->DrawArrays();

	//draw Use Icon Button String
	iGraphicsFont->SetFont(iFontForOptions);
	iGraphicsFont->SetColourWhite();

	TPoint lUseIconButtonLocation(60, 250);
	TInt lHeightOffSet = 90;
	lHeightOffSet	-= iFontForOptions.HeightAndDescentOfFont.GetIntInBaseInt();
	lHeightOffSet -= lHeightOffSet / 2;
	lUseIconButtonLocation.iY += lHeightOffSet;
	iGraphicsFont->DrawStringToScreenHorizontally(lUseIconButtonLocation, iStringUseIconsForButtons);

	//draw Buttons
	iScrollBarMusic->Draw();
	iScrollBarSoundEffects->Draw();
	iUseIconsButton->Draw();
	iChangeSongButton->Draw();
	iCancelButton->Draw();
	iSaveButton->Draw();
}
