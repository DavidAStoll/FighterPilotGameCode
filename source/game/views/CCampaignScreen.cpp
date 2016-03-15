/*
 * CCampaignScreen.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CCampaignScreen.h"
#include "includes/game/views/CChooseCampaignView.h"
#include "includes/game/views/CCampaignBriefingScreen.h"
#include "includes/game/views/CMissionBriefingView.h"
#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/common/CUserStats.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"

CCampaignScreen::CCampaignScreen(SCampaign* aCampaign)
{
	iCampaign = aCampaign;
	iYOffsetValue = 0;
}

CCampaignScreen::~CCampaignScreen()
{
	delete iBackgroundTexture;
	delete iSelectionBackgroundTexture;
	delete iMissionImage;
	delete iMissionDifficultyImage;
	delete iBriefingImage;
	delete iMissionBox;

	delete iScrollSurface;
	delete iScrollBar;

	delete iStringSelectMission;
	delete iStringLocked;
	delete iStringBackButton;
	delete iStringStartButton;
	delete iStringLoadButton;
	delete iStringInfoButton;
	delete iStringHighscore;
	delete iStringBriefingScreen;
	delete iStringDebriefingScreen;

	delete iBackButton;
	delete iBriefingButton;
	delete iDebriefingButton;

	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		delete iMissionBoxes->Get(lIndex)->LoadButton;
		delete iMissionBoxes->Get(lIndex)->StartButton;
		delete iMissionBoxes->Get(lIndex)->MissionName;
		delete iMissionBoxes->Get(lIndex)->MissionScore;
	}
	iMissionBoxes->ClearAndDestroy();
	delete iMissionBoxes;
}

CCampaignScreen* CCampaignScreen::New(SCampaign* aCampaign)
{
	CCampaignScreen* lSelf = new CCampaignScreen(aCampaign);
	lSelf->Construct();
	return lSelf;
}

void CCampaignScreen::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendCampaignScreen), 1024, 660, 100, 0, 1, 0, 1);
	iSelectionBackgroundTexture = CTextureObject::New(TPoint(80, 100),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenBackgroundScreenSelectionBackground), 865, 367, 100, 0, 1, 0, 1);
	iMissionImage = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendMissionImageAmericansLandeAtLosNegros), 400, 160, 100, 0, 1, 0, 1);
	iMissionDifficultyImage = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendCampaignScreenBabySymbol), 71, 36, 100, 0, 1, 0, 1);
	iBriefingImage = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendCampaignBriefingScreenMap1941Small), 610, 160, 100, 0, 1, 0, 1);
	iMissionBox = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBox), 800, 200, 100, 0, 1, 0, 1);

	TInt lNumberMissionBoxes = 0;
	iMissionBoxes = CPointerArray<SMissionBox>::New();
	SMissionBox lMissionBox;

	if(iCampaign->Mission1.Enabled)
	{
		lMissionBox.Mission = &iCampaign->Mission1;
		lMissionBox.MissionName = iLocalizationEngine->GetString(iCampaign->Mission1.MissionNameId);
		lMissionBox.MissionScore = (new CString(""))->AppendNum(iCampaign->Mission1.Score);
		iMissionBoxes->Append(new SMissionBox(lMissionBox));
	}
	if(iCampaign->Mission2.Enabled)
	{
		lMissionBox.Mission = &iCampaign->Mission2;
		lMissionBox.MissionName = iLocalizationEngine->GetString(iCampaign->Mission2.MissionNameId);
		lMissionBox.MissionScore = (new CString(""))->AppendNum(iCampaign->Mission2.Score);
		iMissionBoxes->Append(new SMissionBox(lMissionBox));
	}
	if(iCampaign->Mission3.Enabled)
	{
		lMissionBox.Mission = &iCampaign->Mission3;
		lMissionBox.MissionName = iLocalizationEngine->GetString(iCampaign->Mission3.MissionNameId);
		lMissionBox.MissionScore = (new CString(""))->AppendNum(iCampaign->Mission3.Score);
		iMissionBoxes->Append(new SMissionBox(lMissionBox));
	}
	if(iCampaign->Mission4.Enabled)
	{
		lMissionBox.Mission = &iCampaign->Mission4;
		lMissionBox.MissionName = iLocalizationEngine->GetString(iCampaign->Mission4.MissionNameId);
		lMissionBox.MissionScore = (new CString(""))->AppendNum(iCampaign->Mission4.Score);
		iMissionBoxes->Append(new SMissionBox(lMissionBox));
	}

	lNumberMissionBoxes += iMissionBoxes->GetCount();

	//Strings
	iStringSelectMission = iLocalizationEngine->GetString("Frontend.CampaignScreen.SelectMission");
	iStringLocked = iLocalizationEngine->GetString("Frontend.MissionStatus.Locked");
	iStringHighscore = iLocalizationEngine->GetString("Frontend.CampaignScreen.Highscore");
	iStringBackButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringStartButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Start");
	iStringInfoButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Info");
	iStringLoadButton = iLocalizationEngine->GetString("Frontend.ButtonLabels.Load");
	iStringBriefingScreen = NULL;
	iStringDebriefingScreen = NULL;

	//Fonts
	iFontSelectMission = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringSelectMission, 300, 30);
	iFontLocked = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringLocked, 380, 140);
	iFontBackButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringBackButton, 190, 60);
	iFontForBigStartButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringInfoButton, 144, 140);

	//find Font For Mission Names
	CPointerArray<CString>* lMissionNamesArray = CPointerArray<CString>::New();
	if(iCampaign->CampaignBriefingScreen.Enabled)
	{
		lNumberMissionBoxes++;
		iStringBriefingScreen = iLocalizationEngine->GetString(iCampaign->CampaignBriefingScreen.BriefingScreenNameId);
		lMissionNamesArray->Append(iStringBriefingScreen);
	}
	if(iCampaign->CampaignDebriefingScreen.Enabled)
	{
		lNumberMissionBoxes++;
		iStringDebriefingScreen = iLocalizationEngine->GetString(iCampaign->CampaignDebriefingScreen.BriefingScreenNameId);
		lMissionNamesArray->Append(iStringDebriefingScreen);
	}
	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		lMissionNamesArray->Append(iMissionBoxes->Get(lIndex)->MissionName);
	}
	iFontForMissionNames = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lMissionNamesArray, 780, 30);
	lMissionNamesArray->Clear();
	delete lMissionNamesArray;

	//Find Font for Highscore
	CPointerArray<CString>* lHighscoreArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		CString* lStringHighScorePlusScore = new CString("");
		lStringHighScorePlusScore->Append(iStringHighscore);
		lStringHighScorePlusScore->Append(iMissionBoxes->Get(lIndex)->MissionScore);

		lHighscoreArray->Append(lStringHighScorePlusScore);
	}
	iFontForHighscore = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lHighscoreArray, 380, 30);
	lHighscoreArray->ClearAndDestroy();
	delete lHighscoreArray;

	//find Font small Start And Load
	CPointerArray<CString>* lSmallButtonsStrings = CPointerArray<CString>::New();
	lSmallButtonsStrings->Append(iStringStartButton);
	lSmallButtonsStrings->Append(iStringLoadButton);
	iFontForStartAndLoadButton = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lSmallButtonsStrings, 144, 50);
	lSmallButtonsStrings->Clear();
	delete lSmallButtonsStrings;

	//Small Start And LoadButton
	CButton::SButtonArguments lStartAndLoadButton;
	lStartAndLoadButton.SizeOfHitbox = TSize(164, 70);
	lStartAndLoadButton.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendCampaignScreenStartLoadButton);
	lStartAndLoadButton.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendCampaignScreenStartLoadButtonSelected);
	lStartAndLoadButton.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendCampaignScreenStartLoadButton);
	lStartAndLoadButton.SizeOfTexture = TSize(164, 70);
	lStartAndLoadButton.Label = iStringStartButton;
	lStartAndLoadButton.FontForLabel = &iFontForStartAndLoadButton;
	lStartAndLoadButton.ColourForLabelDefault = KColourBlack;
	lStartAndLoadButton.ColourForLabelSelected = KColourBlack;
	lStartAndLoadButton.ColourForLabelDisabled = KColourDarkGray;
	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		iMissionBoxes->Get(lIndex)->StartButton = CButton::New(lStartAndLoadButton);

		if(!iMissionBoxes->Get(lIndex)->Mission->Unlocked)
		{
			iMissionBoxes->Get(lIndex)->StartButton->SetState(CButton::EStateDisabled);
		}
	}
	lStartAndLoadButton.Label = iStringLoadButton;
	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		iMissionBoxes->Get(lIndex)->LoadButton = CButton::New(lStartAndLoadButton);

		if(!iMissionBoxes->Get(lIndex)->Mission->Unlocked || !CMap::CheckIfSaveGameExists(iMissionBoxes->Get(lIndex)->Mission->SaveGameFile->GetData()))
		{
			iMissionBoxes->Get(lIndex)->LoadButton->SetState(CButton::EStateDisabled);
		}
	}

	//Start Big Buttons
	CButton::SButtonArguments lStartButtonArgs;
	lStartButtonArgs.Location = TPoint(0, 0); //location is not important, since it will be reset all the time
	lStartButtonArgs.SizeOfHitbox = TSize(190, 160);
	lStartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButton);
	lStartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButtonSelected);
	lStartButtonArgs.TextureIdDisabled = &iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBoxButton);
	lStartButtonArgs.SizeOfTexture = TSize(190, 160);
	lStartButtonArgs.Label = iStringInfoButton;
	lStartButtonArgs.FontForLabel = &iFontForBigStartButton;
	lStartButtonArgs.ColourForLabelDefault = KColourBlack;
	lStartButtonArgs.ColourForLabelSelected = KColourBlack;
	lStartButtonArgs.ColourForLabelDisabled = KColourDarkGray;
	iBriefingButton = CButton::New(lStartButtonArgs);
	iDebriefingButton = CButton::New(lStartButtonArgs);

	if(!iCampaign->CampaignBriefingScreen.Unlocked)
		iBriefingButton->SetState(CButton::EStateDisabled);
	if(!iCampaign->CampaignDebriefingScreen.Unlocked)
		iDebriefingButton->SetState(CButton::EStateDisabled);

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

	TInt lMaxYValue = (20 + 230 * lNumberMissionBoxes) - 360; //360 is the height of the display surface

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

	for(TInt lIndex = 0; lIndex < CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex) == iCampaign)
		{
			//means that this is the campaign screen campaign
			CUserStats::SelectedCampaign(lIndex);
			break; // no need to check the others
		}
	}
}


//----------------------- functions ---------------------//

void CCampaignScreen::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if(aTouchEvent.FingerId == 0)
	{
		switch(aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				iScrollSurface->CheckIfHit(aTouchEvent);
				iScrollBar->CheckIfHit(aTouchEvent);

				//Back button
				iBackButton->CheckIfHit(aTouchEvent);
				if(!iBackButton->GetState() == CButton::EStateSelected)
				{
					//check Mission buttons
					for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
					{
						iMissionBoxes->Get(lIndex)->StartButton->CheckIfHit(aTouchEvent);
						iMissionBoxes->Get(lIndex)->LoadButton->CheckIfHit(aTouchEvent);
					}

					//BriefingButtons
					if(iCampaign->CampaignBriefingScreen.Enabled)
					{
						iBriefingButton->CheckIfHit(aTouchEvent);
					}
					if(iCampaign->CampaignDebriefingScreen.Enabled)
					{
						iDebriefingButton->CheckIfHit(aTouchEvent);
					}
				}

				break;
			}

			case ETouchEventHandlerTouchTypeDrag:
			{
				//check if any button has been touched
				TBool lButtonTouched = false;
				for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
				{
					if(iMissionBoxes->Get(lIndex)->StartButton->GetState() == CButton::EStateSelected
						 || iMissionBoxes->Get(lIndex)->LoadButton->GetState() == CButton::EStateSelected)
					{
						lButtonTouched = true;
					}
				}
				if(iCampaign->CampaignBriefingScreen.Enabled)
				{
					if(iBriefingButton->GetState() == CButton::EStateSelected)
					{
						lButtonTouched = true;
					}
				}
				if(iCampaign->CampaignDebriefingScreen.Enabled)
				{
					if(iDebriefingButton->GetState() == CButton::EStateSelected)
					{
						lButtonTouched = true;
					}
				}
				if(iBackButton->GetState() == CButton::EStateSelected)
				{
					lButtonTouched = true;
				}

				//enable scroll only if no button has been selected
				if(!lButtonTouched)
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

				//check Mission buttons
				for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
				{
					if (iMissionBoxes->Get(lIndex)->StartButton->GetState() == CButton::EStateSelected
							&& iMissionBoxes->Get(lIndex)->StartButton->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CMissionBriefingView::New(iCampaign, iMissionBoxes->Get(lIndex)->Mission, false), 0.5 , true);
						return;
					}
					if (iMissionBoxes->Get(lIndex)->LoadButton->GetState() == CButton::EStateSelected
							&& iMissionBoxes->Get(lIndex)->LoadButton->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CMissionBriefingView::New(iCampaign, iMissionBoxes->Get(lIndex)->Mission, true), 0.5 , true);
						return;
					}
				}
				//Back Button
				if(iBackButton->GetState() == CButton::EStateSelected && iBackButton->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CChooseCampaignView::New(), 0.5 , false);
					return;
				}
				//BriefingButtons
				if(iCampaign->CampaignBriefingScreen.Enabled)
				{
					if(iBriefingButton->GetState() == CButton::EStateSelected && iBriefingButton->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignBriefingScreen::New(&iCampaign->CampaignBriefingScreen, iCampaign), 0.5 , true);
					}
				}
				if(iCampaign->CampaignDebriefingScreen.Enabled)
				{
					if(iDebriefingButton->GetState() == CButton::EStateSelected && iDebriefingButton->CheckIfHit(aTouchEvent))
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignBriefingScreen::New(&iCampaign->CampaignDebriefingScreen, iCampaign), 0.5 , true);
					}
				}

				break;
			}
		}
	}
}

void CCampaignScreen::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//draw Background for Campaign Boxes
	iSelectionBackgroundTexture->DrawArrays();

	//------------- Draw Mission Boxes ----------------//
	TPoint lBoxLocation (112, 467 + iYOffsetValue); //want some space from the top as well
	lBoxLocation.iY -= 200; //need to make space for the first box
	lBoxLocation.iY -= 20; //some space between start

	//draw Briefing Screen
	if(iCampaign->CampaignBriefingScreen.Enabled)
	{
		//draw box
		iMissionBox->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iMissionBox->DrawArrays();

		//draw campaign image
		iBriefingImage->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iBriefingImage->AssignTexture(&iImageStore->GetImage(iCampaign->CampaignBriefingScreen.SmallMapImage));
		iBriefingImage->DrawArrays();

		//check if not unlocked
		if(!iCampaign->CampaignBriefingScreen.Unlocked)
		{
			iGraphicsFont->SetFont(iFontLocked);
			iGraphicsFont->SetColour(KColourRed);
			DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY), iFontLocked, iStringLocked, 610, 160);
		}

		//draw Start Button
		iBriefingButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 610, lBoxLocation.iY));
		iBriefingButton->Draw();

		//draw Mission Name
		iGraphicsFont->SetFont(iFontForMissionNames);
		iGraphicsFont->SetColour(KColourDarkBlue);
		DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY + 160), iFontForMissionNames, iStringBriefingScreen, 800, 40);

		//add space between boxes
		lBoxLocation.iY -= 200; //offset for box length
		lBoxLocation.iY -= 30; //space between boxes
	}

	//draw Mission Boxes
	for(TInt lIndex = 0; lIndex < iMissionBoxes->GetCount(); lIndex++)
	{
		//draw box
		iMissionBox->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iMissionBox->DrawArrays();

		//draw Mission image
		iMissionImage->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iMissionImage->AssignTexture(&iImageStore->GetImage(iMissionBoxes->Get(lIndex)->Mission->MissionImage));
		iMissionImage->DrawArrays();

		//check if not unlocked
		if(!iMissionBoxes->Get(lIndex)->Mission->Unlocked)
		{
			iGraphicsFont->SetFont(iFontLocked);
			iGraphicsFont->SetColour(KColourRed);
			DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY), iFontLocked, iStringLocked, 400, 160);
		}

		//draw difficulty Images
		TPoint lDifficultyImageLocation = TPoint(lBoxLocation.iX + 400 + 58, lBoxLocation.iY + 125);
		//draw Baby
		if(iMissionBoxes->Get(lIndex)->Mission->CompletedBaby)
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenBabySymbol));
		else
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenBabySymbolGreyedOut));
		iMissionDifficultyImage->SetNewLowerLeftCoordinate((TPointIntFloat(TIntFloat::Convert(lDifficultyImageLocation.iX), TIntFloat::Convert(lDifficultyImageLocation.iY))));
		iMissionDifficultyImage->DrawArrays();
		lDifficultyImageLocation.iX += 71;
		//draw Easy
		if(iMissionBoxes->Get(lIndex)->Mission->CompletedEasy)
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenEasySymbol));
		else
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenEasySymbolGreyedOut));
		iMissionDifficultyImage->SetNewLowerLeftCoordinate((TPointIntFloat(TIntFloat::Convert(lDifficultyImageLocation.iX), TIntFloat::Convert(lDifficultyImageLocation.iY))));
		iMissionDifficultyImage->DrawArrays();
		lDifficultyImageLocation.iX += 71;
		//draw Medium
		if(iMissionBoxes->Get(lIndex)->Mission->CompletedMedium)
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenMediumSymbol));
		else
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenMediumSymbolGreyedOut));
		iMissionDifficultyImage->SetNewLowerLeftCoordinate((TPointIntFloat(TIntFloat::Convert(lDifficultyImageLocation.iX), TIntFloat::Convert(lDifficultyImageLocation.iY))));
		iMissionDifficultyImage->DrawArrays();
		lDifficultyImageLocation.iX += 71;
		//draw Hard
		if(iMissionBoxes->Get(lIndex)->Mission->CompletedHard)
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenHardSymbol));
		else
			iMissionDifficultyImage->AssignTexture(&iImageStore->GetImage(EImageIdFrontendCampaignScreenHardSymbolGreyedOut));
		iMissionDifficultyImage->SetNewLowerLeftCoordinate((TPointIntFloat(TIntFloat::Convert(lDifficultyImageLocation.iX), TIntFloat::Convert(lDifficultyImageLocation.iY))));
		iMissionDifficultyImage->DrawArrays();

		//draw Highscore
		iGraphicsFont->SetFont(iFontForHighscore);
		iGraphicsFont->SetColour(KColourDarkRed);
		//draw HighScore:
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lBoxLocation.iX + 410, lBoxLocation.iY + 90), iStringHighscore);
		TInt lLenghtOfScore = iGraphicsFont->GetWidthOfString(iFontForHighscore, iMissionBoxes->Get(lIndex)->MissionScore);
		//draw numerical value,is centered to the right
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lBoxLocation.iX + (790 - lLenghtOfScore),lBoxLocation.iY + 90), iMissionBoxes->Get(lIndex)->MissionScore);

		//Draw Load Button
		iMissionBoxes->Get(lIndex)->LoadButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 410, lBoxLocation.iY + 14));
		iMissionBoxes->Get(lIndex)->LoadButton->Draw();
		//Draw Start Button
		iMissionBoxes->Get(lIndex)->StartButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 625, lBoxLocation.iY + 14));
		iMissionBoxes->Get(lIndex)->StartButton->Draw();

		//draw Mission Name
		iGraphicsFont->SetFont(iFontForMissionNames);
		iGraphicsFont->SetColour(KColourDarkBlue);
		DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY + 160), iFontForMissionNames, iMissionBoxes->Get(lIndex)->MissionName, 800, 40);

		//add space between boxes
		lBoxLocation.iY -= 200; //offset for box length
		lBoxLocation.iY -= 30; //space between boxes
	}

	//draw Debriefing Screen
	if(iCampaign->CampaignDebriefingScreen.Enabled)
	{
		//draw box
		iMissionBox->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iMissionBox->DrawArrays();

		//draw campaign image
		iBriefingImage->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iBriefingImage->AssignTexture(&iImageStore->GetImage(iCampaign->CampaignDebriefingScreen.SmallMapImage));
		iBriefingImage->DrawArrays();

		//check if not unlocked
		if(!iCampaign->CampaignDebriefingScreen.Unlocked)
		{
			iGraphicsFont->SetFont(iFontLocked);
			iGraphicsFont->SetColour(KColourRed);
			DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY), iFontLocked, iStringLocked, 610, 160);
		}

		//draw Start Button
		iDebriefingButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 610, lBoxLocation.iY));
		iDebriefingButton->Draw();

		//draw Mission Name
		iGraphicsFont->SetFont(iFontForMissionNames);
		iGraphicsFont->SetColour(KColourDarkBlue);
		DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY + 160), iFontForMissionNames, iStringDebriefingScreen, 800, 40);

		//add space between boxes
		lBoxLocation.iY -= 200; //offset for box length
		lBoxLocation.iY -= 30; //space between boxes
	}

	//--------------- Draw Background And Controls -------------//

	iBackgroundTexture->DrawArrays();

	//draw Select Mission String
	iGraphicsFont->SetColour(KColourDarkRed);
	iGraphicsFont->SetFont(iFontSelectMission);
	DrawCenteredString(TPoint(360, 480), iFontSelectMission, iStringSelectMission, 310, 40);

	//Scroll Bar
	iScrollBar->Draw();

	//draw Back Button
	iBackButton->Draw();
}
