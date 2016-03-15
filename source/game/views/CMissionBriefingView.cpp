/*
 * CMissionBriefingView.cpp
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CMissionBriefingView.h"
#include "includes/game/views/CCampaignScreen.h"
#include "includes/game/views/CLoadingScreen.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CMissionBriefingView::CMissionBriefingView(SCampaign* aCampaignData, SMission* aMissionData, TBool aLoadingSaveGame)
{
	iCampaignData = aCampaignData;
	iMissionData = aMissionData;
	iLoadingSaveGame = aLoadingSaveGame;
	iDescriptionOffsetValue = 0;
	iMapOffsetValue = 0;
}

CMissionBriefingView::~CMissionBriefingView()
{
	delete iBackgroundTexture;
	delete iDescriptionBackgroundTexture;
	delete iOverSizedImage;

	delete iStringMissionTitle;
	delete iStringBackButton;
	delete iStringStartButton;
	delete iStringDescription;
	delete iStringBaby;
	delete iStringEasy;
	delete iStringMedium;
	delete iStringHard;

	delete iBackButton;
	delete iStartButton;
	delete iRadioButtonsDifficulty;
	delete iScrollBarDescription;
	delete iScrollSurfaceDescription;
	delete iScrollSurfaceMap;
	delete iScrollBarMap;

	//need to delete the original Map Image
	iImageStore->UnloadImage(iMissionData->MapImageId);
}


CMissionBriefingView* CMissionBriefingView::New(SCampaign* aCampaignData, SMission* aMissionData, TBool aLoadingSaveGame)
{
	CMissionBriefingView* lSelf = new CMissionBriefingView(aCampaignData, aMissionData, aLoadingSaveGame);
	lSelf->Construct();
	return lSelf;
}

void CMissionBriefingView::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackgroundTexture = CTextureObject::New(TPoint(43, 100), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground), 721, 270, 100, 0, 1, 0, 1);

	iImageStore->LoadOverSizedImage(iMissionData->MapImageId);
	iOverSizedImage = COverSizedImage::New(TSize(1024, 180), *iImageStore->GetOverSizeImage(iMissionData->MapImageId));

	//get Strings
	iStringMissionTitle = iLocalizationEngine->GetString(iMissionData->MissionNameId);
	iStringBackButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringStartButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Start");
	iStringDescription = iLocalizationEngine->GetString(iMissionData->MissionDescriptionId);
	iStringBaby = iLocalizationEngine->GetString("Frontend.MissionBriefing.Baby");
	iStringEasy = iLocalizationEngine->GetString("Frontend.MissionBriefing.Easy");
	iStringMedium = iLocalizationEngine->GetString("Frontend.MissionBriefing.Medium");
	iStringHard = iLocalizationEngine->GetString("Frontend.MissionBriefing.Hard");

	//Fonts
	iFontMissionTitle = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringMissionTitle, 865, 30);
	iFontDescription = iGraphicsFont->CreateFont(PREFERRED_FONT_SIZE_FOR_TEXT_IN_BOX);

	//Font for Difficulty
	CPointerArray<CString>* lDifficultyStringArray = CPointerArray<CString>::New();
	lDifficultyStringArray->Append(iStringBaby);
	lDifficultyStringArray->Append(iStringEasy);
	lDifficultyStringArray->Append(iStringMedium);
	lDifficultyStringArray->Append(iStringHard);
	iFontDifficulty = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lDifficultyStringArray, 118, 40);
	//clean up
	delete lDifficultyStringArray;
	lDifficultyStringArray = NULL;

	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringBackButton);
	lButtonStringArray->Append(iStringStartButton);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//Back Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(151, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringBackButton;
	lBackButtonArgs.FontForLabel = &iFontForButtons;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iBackButton = CButton::New(lBackButtonArgs);

	//Start Button
	CButton::SButtonArguments lStartButtonArgs;
	lStartButtonArgs.Location = TPoint(663, 0);
	lStartButtonArgs.SizeOfHitbox = TSize(210, 85);
	lStartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lStartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lStartButtonArgs.SizeOfTexture = TSize(210, 85);
	lStartButtonArgs.Label = iStringStartButton;
	lStartButtonArgs.FontForLabel = &iFontForButtons;
	lStartButtonArgs.ColourForLabelDefault = KColourBlack;
	lStartButtonArgs.ColourForLabelSelected = KColourBlack;
	iStartButton = CButton::New(lStartButtonArgs);

	//get Max Height for Description
	TInt lMaxYScrollForDescription = GetMaxHeightOfTextInBox(700, iFontDescription, iStringDescription);
	lMaxYScrollForDescription -= 270; //270 is the height of the display box for the description
	if(lMaxYScrollForDescription < 0)
		lMaxYScrollForDescription = 0;
	//allocate Scroll Surfaces For the Description
	//Scroll surface
	iScrollSurfaceDescription = CTouchSurface::New(TPoint(0,100), TSize(750, 270), iDescriptionOffsetValue, lMaxYScrollForDescription, 2, CTouchSurface::EScrollAxisY);
	//ScrollBar
	CScrollBar::SScrollBarArguments lScrollBarDescriptionArgs;
	lScrollBarDescriptionArgs.Location = TPoint(6, 280);
	lScrollBarDescriptionArgs.SizeOfHitbox = TSize(37, 100);
	lScrollBarDescriptionArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarDescriptionArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarDescriptionArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarDescriptionArgs.MaxScroll = lMaxYScrollForDescription;
	lScrollBarDescriptionArgs.MaxScrollHeight = 270;
	lScrollBarDescriptionArgs.ScrollSpeedFactor = 8;
	lScrollBarDescriptionArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBarDescription = CScrollBar::New(lScrollBarDescriptionArgs, iDescriptionOffsetValue);

	//get Max Width for Map
	TInt lMaxXOffsetForMap = iImageStore->GetImageSize(iMissionData->MapImageId).iWidth - 1024; //1024 is the width of map display surface
	if(lMaxXOffsetForMap < 0 )
		lMaxXOffsetForMap = 0;
	//we want to start up at the right most location of the map
	iMapOffsetValue = lMaxXOffsetForMap;
	iOverSizedImage->ChangeXCutOutLocation(iMapOffsetValue); //update X Location
	//allocate Scroll Surfaces For the Map
	//Scroll surface
	iScrollSurfaceMap = CTouchSurface::New(TPoint(0,434), TSize(1024, 180), iMapOffsetValue, lMaxXOffsetForMap, 4, CTouchSurface::EScrollAxisX);
	//ScrollBar
	CScrollBar::SScrollBarArguments lScrollBarMapArgs;
	lScrollBarMapArgs.Location = TPoint(14, 394);
	lScrollBarMapArgs.SizeOfHitbox = TSize(200, 40);
	lScrollBarMapArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedHorizontalScrollBarSmall);
	lScrollBarMapArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedHorizontalScrollBarSmallSelected);
	lScrollBarMapArgs.SizeOfTexture = TSize(90, 28);
	lScrollBarMapArgs.MaxScroll = lMaxXOffsetForMap;
	lScrollBarMapArgs.MaxScrollHeight = 1001;
	lScrollBarMapArgs.ScrollSpeedFactor = 10;
	lScrollBarMapArgs.ScrollAxis = CScrollBar::EScrollAxisX;
	iScrollBarMap = CScrollBar::New(lScrollBarMapArgs, iMapOffsetValue);

	//RadioButtons for difficulty
	CRadioButton::SRadioButtonArguments lRadioButtonArgs;
	iRadioButtonsDifficulty = CRadioButtonGroup::New();

	//default values
	lRadioButtonArgs.SizeOfHitbox = TSize(244, 49);
	lRadioButtonArgs.SizeOfTexture = TSize(98, 49);
	lRadioButtonArgs.Label = NULL;
	//Baby
	lRadioButtonArgs.Location = TPoint(780, 321);
	lRadioButtonArgs.TextureIdDefault = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyBaby);
	lRadioButtonArgs.TextureIdSelected = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyBabySelected);
	lRadioButtonArgs.TextureIdDisabled = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyBabyGreyedOut);
	iRadioButtonsDifficulty->AddRadioButton(CRadioButton::New(lRadioButtonArgs), EGameDifficultyBaby);
	//Easy
	lRadioButtonArgs.Location = TPoint(780, 272);
	lRadioButtonArgs.TextureIdDefault = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyEasy);
	lRadioButtonArgs.TextureIdSelected = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyEasySelected);
	lRadioButtonArgs.TextureIdDisabled = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyEasyGreyedOut);
	iRadioButtonsDifficulty->AddRadioButton(CRadioButton::New(lRadioButtonArgs), EGameDifficultyEasy);
	//Medium
	lRadioButtonArgs.Location = TPoint(780, 223);
	lRadioButtonArgs.TextureIdDefault = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyMedium);
	lRadioButtonArgs.TextureIdSelected = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyMediumSelected);
	lRadioButtonArgs.TextureIdDisabled = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyMediumGreyedOut);
	iRadioButtonsDifficulty->AddRadioButton(CRadioButton::New(lRadioButtonArgs), EGameDifficultyMedium);
	//Hard
	lRadioButtonArgs.Location = TPoint(780, 174);
	lRadioButtonArgs.TextureIdDefault = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyHard);
	lRadioButtonArgs.TextureIdSelected = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyHardSelected);
	lRadioButtonArgs.TextureIdDisabled = &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionBriefingScreenDifficultyHardGreyedOut);
	iRadioButtonsDifficulty->AddRadioButton(CRadioButton::New(lRadioButtonArgs), EGameDifficultyHard);

	//disable all other difficulties
	if(iLoadingSaveGame)
	{
		TGameDifficulty lSaveGameDifficulty = CMap::GetGameDifficulty(iMissionData->SaveGameFile->GetData());

		for(TInt lIndex = 0; lIndex < iRadioButtonsDifficulty->GetCount(); lIndex++)
		{
			iRadioButtonsDifficulty->GetRadioButton(lIndex)->SetState(CRadioButton::EStateDisabled);
		}

		//select the button that has game save difficulty
		iRadioButtonsDifficulty->GetRadioButton(lSaveGameDifficulty)->SetState(CRadioButton::EStateSelected);
	}
	else
	{
		//select radio button with current difficutly
		iRadioButtonsDifficulty->GetRadioButton(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty)->SetState(CRadioButton::EStateSelected);
	}
}

//----------------------- functions ---------------------//

void CMissionBriefingView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//ScrollSurfaces
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceMap->CheckIfHit(aTouchEvent);
				iScrollBarMap->CheckIfHit(aTouchEvent);

				//Back and Start Button
				iBackButton->CheckIfHit(aTouchEvent);
				iStartButton->CheckIfHit(aTouchEvent);
				//Difficulty Radio Buttons
				iRadioButtonsDifficulty->CheckIfHit(aTouchEvent);
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//ScrollSurfaces
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceMap->CheckIfHit(aTouchEvent);
				iScrollBarMap->CheckIfHit(aTouchEvent);
				//Difficulty Radio Buttons
				iRadioButtonsDifficulty->CheckIfHit(aTouchEvent);

				//update SubImage if any of the map surfaces is touched
				if(iScrollSurfaceMap->GetIsTouched()|| iScrollBarMap->GetState() == CButton::EStateSelected)
				{
					iOverSizedImage->ChangeXCutOutLocation(iMapOffsetValue);
				}
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//ScrollSurfaces
				iScrollSurfaceDescription->CheckIfHit(aTouchEvent);
				iScrollBarDescription->CheckIfHit(aTouchEvent);
				iScrollSurfaceMap->CheckIfHit(aTouchEvent);
				iScrollBarMap->CheckIfHit(aTouchEvent);
				//Difficulty Radio Buttons
				iRadioButtonsDifficulty->CheckIfHit(aTouchEvent);

				//Back Button, go back to campaign View
				if(iBackButton->GetState() == CButton::EStateSelected && iBackButton->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignScreen::New(iCampaignData), 0.5 , false);
				}
				//Start the Mission
				if(iStartButton->GetState() == CButton::EStateSelected && iStartButton->CheckIfHit(aTouchEvent))
				{
					//update difficulty
					CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty = static_cast<TGameDifficulty>(iRadioButtonsDifficulty->GetValueOfSelectedButton());

					//need to find out Mission and Campaign Index
					TInt lMissionIndex = 0;
					TInt lCampaignIndex = 0;
					for(TInt lIndex = 0; lIndex < CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->GetCount(); lIndex++)
					{
						if(iCampaignData == CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex))
						{
							lCampaignIndex = lIndex;
							if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MapId == iMissionData->MapId)
							{
								lMissionIndex = 0;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MapId == iMissionData->MapId)
							{
								lMissionIndex = 1;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MapId == iMissionData->MapId)
							{
								lMissionIndex = 2;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MapId == iMissionData->MapId)
							{
								lMissionIndex = 3;
							}
							break; //all done
						}
					}
					//record user stat
					CUserStats::PlaysMission(lCampaignIndex, lMissionIndex, CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty, iLoadingSaveGame);
					CUserStats::EndMissionInit(lCampaignIndex, lMissionIndex, CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty, iLoadingSaveGame);

					//start Map
					if(iLoadingSaveGame)
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CLoadingScreen::New(iCampaignData, iMissionData, ELoadingScreenModeLoadMap), 5, 4, 4, 30);
					}
					else
					{
						CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CLoadingScreen::New(iCampaignData, iMissionData, ELoadingScreenModeStartMap), 5, 4, 4, 30);
					}
				}
				break;
			}
		}
	}
}

void CMissionBriefingView::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//draw Description
	iDescriptionBackgroundTexture->DrawArrays();

	//draw Description
	iGraphicsFont->SetColour(KColourBlack);
	DrawStringIntoBox(TPoint(53, 360 + iDescriptionOffsetValue), 700, TRect(TPoint(43,370), TPoint(764,100)), iFontDescription, iStringDescription);

	//draw Screen Background
	iBackgroundTexture->DrawArrays();

	//draw Map
	iOverSizedImage->iTextureObject->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(0), TIntFloat::Convert(434)));
	iOverSizedImage->iTextureObject->DrawArrays();

	//draw Title
	iGraphicsFont->SetColour(KColourDarkRed);
	iGraphicsFont->SetFont(iFontMissionTitle);
	DrawCenteredString(TPoint(80, 620), iFontMissionTitle, iStringMissionTitle, 865, 40);

	//draw Difficulty labels
	iGraphicsFont->SetFont(iFontDifficulty);

	//Baby
	if(iRadioButtonsDifficulty->GetRadioButton(0)->GetState() == CRadioButton::EStateDefault)
	{
		iGraphicsFont->SetColour(KColourBlack);
	}
	else if(iRadioButtonsDifficulty->GetRadioButton(0)->GetState() == CRadioButton::EStateSelected)
	{
		iGraphicsFont->SetColour(KColourDarkBlue);
	}
	else
	{
		iGraphicsFont->SetColour(KColourDarkGray);
	}
	DrawCenteredString(TPoint(878, 324), iFontDifficulty, iStringBaby, 138, 49);

	//Easy
	if(iRadioButtonsDifficulty->GetRadioButton(1)->GetState() == CRadioButton::EStateDefault)
	{
		iGraphicsFont->SetColour(KColourBlack);
	}
	else if(iRadioButtonsDifficulty->GetRadioButton(1)->GetState() == CRadioButton::EStateSelected)
	{
		iGraphicsFont->SetColour(KColourDarkBlue);
	}
	else
	{
		iGraphicsFont->SetColour(KColourDarkGray);
	}
	DrawCenteredString(TPoint(878, 275), iFontDifficulty, iStringEasy, 138, 49);

	//Medium
	if(iRadioButtonsDifficulty->GetRadioButton(2)->GetState() == CRadioButton::EStateDefault)
	{
		iGraphicsFont->SetColour(KColourBlack);
	}
	else if(iRadioButtonsDifficulty->GetRadioButton(2)->GetState() == CRadioButton::EStateSelected)
	{
		iGraphicsFont->SetColour(KColourDarkBlue);
	}
	else
	{
		iGraphicsFont->SetColour(KColourDarkGray);
	}
	DrawCenteredString(TPoint(878, 226), iFontDifficulty, iStringMedium, 138, 49);

	//Hard
	if(iRadioButtonsDifficulty->GetRadioButton(3)->GetState() == CRadioButton::EStateDefault)
	{
		iGraphicsFont->SetColour(KColourBlack);
	}
	else if(iRadioButtonsDifficulty->GetRadioButton(3)->GetState() == CRadioButton::EStateSelected)
	{
		iGraphicsFont->SetColour(KColourDarkBlue);
	}
	else
	{
		iGraphicsFont->SetColour(KColourDarkGray);
	}
	DrawCenteredString(TPoint(878, 177), iFontDifficulty, iStringHard, 138, 49);

	//draw Controls
	iBackButton->Draw();
	iStartButton->Draw();
	iRadioButtonsDifficulty->Draw();
	iScrollSurfaceDescription->Draw();
	iScrollBarDescription->Draw();
	iScrollSurfaceMap->Draw();
	iScrollBarMap->Draw();
}
