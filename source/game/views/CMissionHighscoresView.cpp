/*
 * CMissionHighscoresView.cpp
 *
 *  Created on: Oct 19, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CMissionHighscoresView.h"
#include "includes/game/views/CHighscoresView.h"
#include "includes/game/views/CAgreeToTermsScreen.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"


CMissionHighscoresView::CMissionHighscoresView(SMission* aMissionToDisplay)
{
	iTableYOffset = 0;
	iMission = aMissionToDisplay;
}

CMissionHighscoresView::~CMissionHighscoresView()
{

}

CMissionHighscoresView* CMissionHighscoresView::New(SMission* aMissionToDisplay)
{
	CMissionHighscoresView* lSelf = new CMissionHighscoresView(aMissionToDisplay);
	lSelf->Construct();
	return lSelf;
}

void CMissionHighscoresView::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionVictoryScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackgroundTexture = CTextureObject::New(TPoint(64, 100), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground), 896, 280, 100, 0, 1, 0, 1);

	iStringBack = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringTitle = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Top10Highscores");
	iStringMissionName = iLocalizationEngine->GetString(iMission->MissionNameId);
	//categories
	iStringRank = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Rank");
	iStringName = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Gamertag");
	iStringScore = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Score");
	iStringTime = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Time");
	iStringDate = iLocalizationEngine->GetString("Frontend.MissionHighscoresView.Date");

	//Font
	iFontForTitle = iGraphicsFont->CreateFont(40); //always have enough space
	iFontForMissionName = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringMissionName, 600, 30);
	iFontForButton = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringBack, 190, 60);
	iFontForHighscoresCategories = iGraphicsFont->CreateFont(35);
	iFontForHighscoresRows = iGraphicsFont->CreateFont(28);

	TInt lMaxYTableOffset = 70;

	//Back Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(407, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.Label = iStringBack;
	lBackButtonArgs.FontForLabel = &iFontForButton;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonBack = CButton::New(lBackButtonArgs);

	//ScrollBar for Stats
	CScrollBar::SScrollBarArguments lScrollBarArgs;
	lScrollBarArgs.Location = TPoint(970, 290);
	lScrollBarArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarArgs.MaxScroll = lMaxYTableOffset;
	lScrollBarArgs.MaxScrollHeight = 280;
	lScrollBarArgs.ScrollSpeedFactor = 4;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iTableYOffset);

	iTouchSurface = CTouchSurface::New(TPoint(0,100), TSize(960, 280), iTableYOffset, lMaxYTableOffset, 2, CTouchSurface::EScrollAxisY);
}

void CMissionHighscoresView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//Touch Surfaces
				iScrollBar->CheckIfHit(aTouchEvent);
				iTouchSurface->CheckIfHit(aTouchEvent);

				//Buttons
				iButtonBack->CheckIfHit(aTouchEvent);
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

				//go back to HighscoresView
				if (iButtonBack->GetState() == CButton::EStateSelected && iButtonBack->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CHighscoresView::New(CHighscoresView::InitialYOffset), 0.5, false);
				}
				break;
			}
		}
	}
}

void CMissionHighscoresView::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//Background for Stats
	iDescriptionBackgroundTexture->DrawArrays();

	//draw Highscores
	iGraphicsFont->SetColour(KColourBlack);

	TPoint lTableLocation (70, 340 + iTableYOffset);
	DrawCategories(lTableLocation);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank1, 1);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank2, 2);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank3, 3);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank4, 4);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank5, 5);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank6, 6);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank7, 7);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank8, 8);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank9, 9);
	DrawRow(lTableLocation, iMission->HighscoreTable.Rank10, 10);


	//draw Background Texture
	iBackgroundTexture->DrawArrays();

	//draw Mission Status
	iGraphicsFont->SetFont(iFontForTitle);
	iGraphicsFont->SetColour(KColourDarkRed);
	DrawCenteredString(TPoint(0,610), iFontForTitle, iStringTitle, 1024, 50);

	//draw Mission Name
	iGraphicsFont->SetFont(iFontForMissionName);
	iGraphicsFont->SetColour(KColourDarkRed);
	DrawCenteredString(TPoint(200,420), iFontForMissionName, iStringMissionName, 625, 40);

	//draw Controls
	iButtonBack->Draw();
	iScrollBar->Draw();
	iTouchSurface->Draw();
}

void CMissionHighscoresView::DrawCategories(TPoint& aLocation)
{
	iGraphicsFont->SetFont(iFontForHighscoresCategories);
	TInt lFontHeight = iFontForHighscoresCategories.HeightAndDescentOfFont.GetIntInBaseInt();
	TPoint lTempLocation = aLocation;

	DrawCenteredString(lTempLocation, iFontForHighscoresCategories, iStringRank, 100, lFontHeight);
	lTempLocation.iX += 100;
	DrawCenteredString(lTempLocation, iFontForHighscoresCategories, iStringName, 260, lFontHeight);
	lTempLocation.iX += 260;
	DrawCenteredString(lTempLocation, iFontForHighscoresCategories, iStringScore, 180, lFontHeight);
	lTempLocation.iX += 180;
	DrawCenteredString(lTempLocation, iFontForHighscoresCategories, iStringTime, 180, lFontHeight);
	lTempLocation.iX += 180;
	DrawCenteredString(lTempLocation, iFontForHighscoresCategories, iStringDate, 150, lFontHeight);
	lTempLocation.iX += 150;

	aLocation.iY -= lFontHeight;
}

void CMissionHighscoresView::DrawRow(TPoint& aLocation, const SMissionHighscoreTableEntry& aEntry, TInt aRankNumber)
{
	iGraphicsFont->SetFont(iFontForHighscoresRows);
	TInt lFontHeight = iFontForHighscoresRows.HeightAndDescentOfFont.GetIntInBaseInt();
	TPoint lTempLocation = aLocation;
	CString* lStringRank = (new CString(""))->AppendNum(aRankNumber);

	DrawCenteredString(lTempLocation, iFontForHighscoresRows, lStringRank, 100, lFontHeight);
	lTempLocation.iX += 100;
	DrawCenteredString(lTempLocation, iFontForHighscoresRows, aEntry.PlayerName, 260, lFontHeight);
	lTempLocation.iX += 260;
	DrawCenteredString(lTempLocation, iFontForHighscoresRows, aEntry.Score, 180, lFontHeight);
	lTempLocation.iX += 180;
	DrawCenteredString(lTempLocation, iFontForHighscoresRows, aEntry.Time, 180, lFontHeight);
	lTempLocation.iX += 180;
	DrawCenteredString(lTempLocation, iFontForHighscoresRows, aEntry.Date, 150, lFontHeight);
	lTempLocation.iX += 150;

	aLocation.iY -= lFontHeight;
	delete lStringRank;
}

