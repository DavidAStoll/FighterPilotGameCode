/*
 * CHighScoresView.cpp
 *
 *  Created on: Oct 12, 2012
 *      Author: dstoll
 */

#include "includes/game/views/CHighscoresView.h"
#include "includes/game/views/CMissionHighscoresView.h"
#include "includes/game/views/CMainMenuView.h"
#include "includes/game/views/CAgreeToTermsScreen.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/popups/CPopupEnterText.h"
#include "includes/core/views/popups/CPopupProgressBar.h"
#include "includes/core/views/popups/CPopupWarning.h"
#include "includes/core/views/CViewKeyboard.h"
#include "includes/core/utility/CSystem.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"
#include "includes/core/views/transitions/CTransitionWithoutEffects.h"
#include "includes/core/views/transitions/CTransitionOpenDoorRotating.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#if AMAZON_BUILD
	#include "includes/native/utility/CAndroidAmazonGameCircleFunctions.h" //need for specific Amazon function call
#endif

TBool CHighscoresView::ServerNotRespondedError;
TBool CHighscoresView::GameOutdated;
TInt CHighscoresView::InitialYOffset = 0;

#define KWhitespace " "
#define KSignalReceiveDone "SOS"
#define KSignalUploadDone "UPDATING SCORES DONE"
#define KSignalGameVersion "Version 1.8"
#define KSignalHandshakeReplyNeeded "Good Version"

CHighscoresView::CHighscoresView(TInt aInitialYOffset)
{
	iYOffsetValue = aInitialYOffset;
	CHighscoresView::ServerNotRespondedError = false;
	CHighscoresView::GameOutdated = false;
}

CHighscoresView::~CHighscoresView()
{
	delete iBackgroundTexture;
	delete iSelectionBackgroundTexture;
	delete iHighscoreBoxTexture;
	delete iMissionImage;

	delete iStringBack;
	delete iStringUpdate;
	delete iStringViewScores;
	delete iStringHighscore;
	delete iStringGlobalHighscores;
	delete iStringTempPlayerName;

	delete iScrollSurface;
	delete iScrollBar;
	delete iButtonBack;
	delete iButtonUpdate;

	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		delete iHighscoreBoxes->Get(lIndex)->MissionName;
		delete iHighscoreBoxes->Get(lIndex)->Highscore;
		delete iHighscoreBoxes->Get(lIndex)->ViewScoreButton;
	}
	iHighscoreBoxes->ClearAndDestroy();
	delete iHighscoreBoxes;
}

CHighscoresView* CHighscoresView::New(TInt aInitialYOffset)
{
	CHighscoresView* lSelf = new CHighscoresView(aInitialYOffset);
	lSelf->Construct();
	return lSelf;
}

void CHighscoresView::Construct()
{
	//Texture Objects
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendCampaignScreen), 1024, 660, 100, 0, 1, 0, 1);
	iSelectionBackgroundTexture = CTextureObject::New(TPoint(80, 100),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenBackgroundScreenSelectionBackground), 865, 367, 100, 0, 1, 0, 1);
	iHighscoreBoxTexture = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendChooseCampaignScreenSelectionBox), 800, 200, 100, 0, 1, 0, 1);;
	iMissionImage = CTextureObject::New(TPoint(0, 0),&iImageStore->GetImage(EImageIdFrontendMissionImageAmericansLandeAtLosNegros), 400, 160, 100, 0, 1, 0, 1);

	//Mission Boxes
	iHighscoreBoxes = CPointerArray<SHighscoreBox>::New();
	SHighscoreBox lHighscoreBox;

	CSaveGameObject* lSaveGameObject = CFighterPilotThePacificWar::FighterGame->iSaveGameObject;
	for(TInt lIndex = 1; lIndex < lSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission1;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission2;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission3;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission4;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
	}

	//Add Aces Of Pacific War Box
	lHighscoreBox.Mission = &lSaveGameObject->iBestOfAllMission;
	lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iBestOfAllMission.MissionNameId);
	lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iBestOfAllMission.HighscoreTable.Rank1.Score);
	lHighscoreBox.ViewScoreButton = NULL;
	iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));

	//Need to update Overall Score
	TInt lOverallScore = 0;
	TInt lTotalTimeInSecs = 0;

	for(TInt lIndex = 1; lIndex < lSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		UpdateOverallScoreEntry(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1, lOverallScore, lTotalTimeInSecs);
		UpdateOverallScoreEntry(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2, lOverallScore, lTotalTimeInSecs);
		UpdateOverallScoreEntry(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3, lOverallScore, lTotalTimeInSecs);
		UpdateOverallScoreEntry(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4, lOverallScore, lTotalTimeInSecs);
	}

	//update OverallScore
	lSaveGameObject->iBestOfAllMission.Score = lOverallScore;

	//Update Time
	CString* lStringOverallTime = new CString("");
	TInt lOverallHours = (lTotalTimeInSecs / 60) / 60;
	TInt lOverallMins = (lTotalTimeInSecs - (lOverallHours * 60 * 60)) / 60;
	TInt lOverallSecs = lTotalTimeInSecs % 60;

	//hopefully will never be true, but I just want to make sure that nothing gets screwed up
	if(lOverallHours > 99)
		lOverallHours = 99;

	//Hours
	if(lOverallHours < 10)
		lStringOverallTime->Append("0");
	lStringOverallTime->AppendNum(lOverallHours);
	lStringOverallTime->Append(":");
	//Mins
	if(lOverallMins < 10)
		lStringOverallTime->Append("0");
	lStringOverallTime->AppendNum(lOverallMins);
	lStringOverallTime->Append(":");
	//Secs
	if(lOverallSecs < 10)
		lStringOverallTime->Append("0");
	lStringOverallTime->AppendNum(lOverallSecs);

	lSaveGameObject->iBestOfAllMission.Time->Replace(lStringOverallTime);
	delete lStringOverallTime;

	//Strings
	iStringBack = iLocalizationEngine->GetString("Frontend.ButtonLabels.Back");
	iStringUpdate = iLocalizationEngine->GetString("Frontend.ButtonLabels.Update");
	iStringViewScores = iLocalizationEngine->GetString("Frontend.ButtonLabels.ViewScores");
	iStringHighscore = iLocalizationEngine->GetString("Frontend.CampaignScreen.Highscore");
	iStringGlobalHighscores = iLocalizationEngine->GetString("Frontend.HighscoresView.GlobalHighscores");
	iStringTempPlayerName = new CString("");

	//---------------------- Fonts -------------------//

	//Find Font for Mission Names
	CPointerArray<CString>* lMissionNameArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		lMissionNameArray->Append(iHighscoreBoxes->Get(lIndex)->MissionName);
	}
	iFontMissionNames = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lMissionNameArray, 780, 30);
	//clean up
	delete lMissionNameArray;

	//Find Font for Highscore
	CPointerArray<CString>* lHighscoreArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		CString* lStringHighScorePlusScore = new CString("");
		lStringHighScorePlusScore->Append(iStringHighscore);
		lStringHighScorePlusScore->Append(iHighscoreBoxes->Get(lIndex)->Highscore);

		lHighscoreArray->Append(lStringHighScorePlusScore);
	}
	iFontHighscores = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lHighscoreArray, 380, 60);
	//clean up
	lHighscoreArray->ClearAndDestroy();
	delete lHighscoreArray;

	//Font for View Score Button
	iFontButtonViewScores = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringViewScores, 360, 50);
	iFontGlobalHighscores = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringGlobalHighscores, 300, 30);

	//Font for Buttons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringBack);
	lButtonStringArray->Append(iStringUpdate);
	iFontButtonBackAndUpdate = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;

	//---------------------- Controls -------------------//

	//View Scores Button
	CButton::SButtonArguments lViewScoresButtonArgs;
	lViewScoresButtonArgs.Location = TPoint(0, 0);
	lViewScoresButtonArgs.SizeOfHitbox = TSize(380, 70);
	lViewScoresButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButton);
	lViewScoresButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected);
	lViewScoresButtonArgs.SizeOfTexture = TSize(380, 70);
	lViewScoresButtonArgs.Label = iStringViewScores;
	lViewScoresButtonArgs.FontForLabel = &iFontButtonViewScores;
	lViewScoresButtonArgs.ColourForLabelDefault = KColourBlack;
	lViewScoresButtonArgs.ColourForLabelSelected = KColourBlack;
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		iHighscoreBoxes->Get(lIndex)->ViewScoreButton = CButton::New(lViewScoresButtonArgs);
	}

	//Back Button
	CButton::SButtonArguments lBackButtonArgs;
	lBackButtonArgs.Location = TPoint(151, 0);
	lBackButtonArgs.SizeOfHitbox = TSize(210, 85);
	lBackButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lBackButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lBackButtonArgs.SizeOfTexture = TSize(210, 85);
	lBackButtonArgs.Label = iStringBack;
	lBackButtonArgs.FontForLabel = &iFontButtonBackAndUpdate;
	lBackButtonArgs.ColourForLabelDefault = KColourBlack;
	lBackButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonBack = CButton::New(lBackButtonArgs);

	//Update Button
	CButton::SButtonArguments lUpdateButtonArgs;
	lUpdateButtonArgs.Location = TPoint(663, 0);
	lUpdateButtonArgs.SizeOfHitbox = TSize(210, 85);
	lUpdateButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lUpdateButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lUpdateButtonArgs.SizeOfTexture = TSize(210, 85);
	lUpdateButtonArgs.Label = iStringUpdate;
	lUpdateButtonArgs.FontForLabel = &iFontButtonBackAndUpdate;
	lUpdateButtonArgs.ColourForLabelDefault = KColourBlack;
	lUpdateButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonUpdate = CButton::New(lUpdateButtonArgs);

	TInt lNumberOfHighscoreBoxes = iHighscoreBoxes->GetCount();
	TInt lMaxYValue = (20 + 230 * lNumberOfHighscoreBoxes) - 360; //360 is the height of the display surface

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
	lScrollBarArgs.ScrollSpeedFactor = 12;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iYOffsetValue);

	//UserStat
	CUserStats::GeneralCheckHighscoreScreen();
}

void CHighscoresView::UpdateHighscoreBoxes()
{
	//delete old highscore boxes
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		delete iHighscoreBoxes->Get(lIndex)->MissionName;
		delete iHighscoreBoxes->Get(lIndex)->Highscore;
		delete iHighscoreBoxes->Get(lIndex)->ViewScoreButton;
	}
	iHighscoreBoxes->ClearAndDestroy();
	delete iHighscoreBoxes;

	//------------------------------------- Mission Boxes ---------------------------//
	iHighscoreBoxes = CPointerArray<SHighscoreBox>::New();
	SHighscoreBox lHighscoreBox;

	CSaveGameObject* lSaveGameObject = CFighterPilotThePacificWar::FighterGame->iSaveGameObject;
	for(TInt lIndex = 1; lIndex < lSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission1;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission2;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission3;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.Enabled)
		{
			lHighscoreBox.Mission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission4;
			lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MissionNameId);
			lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.HighscoreTable.Rank1.Score);
			lHighscoreBox.ViewScoreButton = NULL;
			iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));
		}
	}

	//Add Aces Of Pacific War Box
	lHighscoreBox.Mission = &lSaveGameObject->iBestOfAllMission;
	lHighscoreBox.MissionName = iLocalizationEngine->GetString(lSaveGameObject->iBestOfAllMission.MissionNameId);
	lHighscoreBox.Highscore = (new CString(""))->Append(lSaveGameObject->iBestOfAllMission.HighscoreTable.Rank1.Score);
	lHighscoreBox.ViewScoreButton = NULL;
	iHighscoreBoxes->Append(new SHighscoreBox(lHighscoreBox));

	//Need to update Overall Score
	for(TInt lIndex = 1; lIndex < lSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.Enabled)
		{
			if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.Score > 0)
			{

			}
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.Enabled)
		{
			if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.Score > 0)
			{

			}
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.Enabled)
		{
			if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.Score > 0)
			{

			}
		}
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.Enabled)
		{
			if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.Score > 0)
			{

			}
		}
	}

	//------------------- UPDATE FONTS ----------------------//

	//Find Font for Mission Names
	CPointerArray<CString>* lMissionNameArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		lMissionNameArray->Append(iHighscoreBoxes->Get(lIndex)->MissionName);
	}
	iFontMissionNames = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lMissionNameArray, 780, 30);
	//clean up
	delete lMissionNameArray;

	//Find Font for Highscore
	CPointerArray<CString>* lHighscoreArray = CPointerArray<CString>::New();
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		CString* lStringHighScorePlusScore = new CString("");
		lStringHighScorePlusScore->Append(iStringHighscore);
		lStringHighScorePlusScore->Append(iHighscoreBoxes->Get(lIndex)->Highscore);

		lHighscoreArray->Append(lStringHighScorePlusScore);
	}
	iFontHighscores = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lHighscoreArray, 380, 60);
	//clean up
	lHighscoreArray->ClearAndDestroy();
	delete lHighscoreArray;

	//Font for View Score Button
	iFontButtonViewScores = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringViewScores, 360, 50);

	//View Scores Button
	CButton::SButtonArguments lViewScoresButtonArgs;
	lViewScoresButtonArgs.Location = TPoint(0, 0);
	lViewScoresButtonArgs.SizeOfHitbox = TSize(380, 70);
	lViewScoresButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButton);
	lViewScoresButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendGameSettingsScreenChangeSongButtonSelected);
	lViewScoresButtonArgs.SizeOfTexture = TSize(380, 70);
	lViewScoresButtonArgs.Label = iStringViewScores;
	lViewScoresButtonArgs.FontForLabel = &iFontButtonViewScores;
	lViewScoresButtonArgs.ColourForLabelDefault = KColourBlack;
	lViewScoresButtonArgs.ColourForLabelSelected = KColourBlack;
	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		iHighscoreBoxes->Get(lIndex)->ViewScoreButton = CButton::New(lViewScoresButtonArgs);
	}
}

void* UploadScores(void* aExtraArg)
{
	CHighscoresView* lHighscoreView = static_cast<CHighscoresView*>(aExtraArg);
	CSaveGameObject* lSaveGameObject = CFighterPilotThePacificWar::FighterGame->iSaveGameObject;
	CString* lPlayerName = CFighterPilotThePacificWar::FighterGame->iGameVariables->iStringPlayerName;
	CPointerArray<SRemoteMessageInternalFormat>* lNewHighscoresBuffer = CPointerArray<SRemoteMessageInternalFormat>::New(); //server will stores messages in here
	CTCPConnectionEngine* lTCPConnection = CTCPConnectionEngine::New(GHIGHSCORE_SERVER_IP, GHIGHSCORE_SERVER_PORT, GHIGHSCORE_SERVER_CONNECTION_TIMEOUT);
	lTCPConnection->SetIfWLanNotNeededToConnect(true);

	//Create Send Messages
	SRemoteMessageInternalFormat lHighscoreMessage;
	//Campaign 1
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(1)->Mission1, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(1)->Mission2, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(1)->Mission3, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(1)->Mission4, lPlayerName));
	//Campaign 2
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(2)->Mission1, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(2)->Mission2, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(2)->Mission3, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(2)->Mission4, lPlayerName));
	//Campaign 3
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(3)->Mission1, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(3)->Mission2, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(3)->Mission3, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(3)->Mission4, lPlayerName));
	//Campaign 4
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(4)->Mission1, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(4)->Mission2, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(4)->Mission3, lPlayerName));
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iCampaigns->Get(4)->Mission4, lPlayerName));
	//Best Of All
	lTCPConnection->AddMessage(CHighscoresView::CreateMissionHighscoreMessage(lSaveGameObject->iBestOfAllMission, lPlayerName));

	//send and receive Highscores
	TTCPConnectionEngineSendStatus lSendStatus = lTCPConnection->HandshakeThenSendMessagesAndReceive(KSignalGameVersion, KSignalHandshakeReplyNeeded, lNewHighscoresBuffer, KSignalUploadDone, KSignalReceiveDone);
	if(lSendStatus == ETCPConnectionEngineSendStatusSuccess)
	{
		//update highscore table
		for(TInt lIndex = 0; lIndex < lNewHighscoresBuffer->GetCount(); lIndex++)
		{
			SRemoteMessageInternalFormat* lMessage = lNewHighscoresBuffer->Get(lIndex);
			CString* lMissionName;
			CString* lRank;
			CString* lPlayerName;
			CString* lScore;
			CString* lDate;
			CString* lTime;
			TInt lLocationOfWhitespace = 0;
			TInt lStringStartIndex = 0;

			//Mission Name
			lLocationOfWhitespace = lMessage->MessageContent->FindString(0, KWhitespace);
			lMissionName = lMessage->MessageContent->GetSubString(lStringStartIndex, lLocationOfWhitespace);
			lStringStartIndex = lLocationOfWhitespace + 1;
			//Rank
			lLocationOfWhitespace = lMessage->MessageContent->FindString(lLocationOfWhitespace + 1, KWhitespace); //skip whitespace
			lRank = lMessage->MessageContent->GetSubString(lStringStartIndex, lLocationOfWhitespace - lStringStartIndex);
			lStringStartIndex = lLocationOfWhitespace + 1;
			//Player Name
			lLocationOfWhitespace = lMessage->MessageContent->FindString(lLocationOfWhitespace + 1, KWhitespace); //skip whitespace
			lPlayerName = lMessage->MessageContent->GetSubString(lStringStartIndex, lLocationOfWhitespace - lStringStartIndex);
			lStringStartIndex = lLocationOfWhitespace + 1;
			//Score
			lLocationOfWhitespace = lMessage->MessageContent->FindString(lLocationOfWhitespace + 1, KWhitespace); //skip whitespace
			lScore = lMessage->MessageContent->GetSubString(lStringStartIndex, lLocationOfWhitespace - lStringStartIndex);
			lStringStartIndex = lLocationOfWhitespace + 1;
			//Date
			lLocationOfWhitespace = lMessage->MessageContent->FindString(lLocationOfWhitespace + 1, KWhitespace); //skip whitespace
			lDate = lMessage->MessageContent->GetSubString(lStringStartIndex, lLocationOfWhitespace - lStringStartIndex);
			lStringStartIndex = lLocationOfWhitespace + 1;
			//Time
			lLocationOfWhitespace = lMessage->MessageContent->FindString(lLocationOfWhitespace + 1, KWhitespace); //skip whitespace
			lTime = lMessage->MessageContent->GetSubString(lStringStartIndex, lMessage->MessageContent->GetLength() - lStringStartIndex);
			lStringStartIndex = lLocationOfWhitespace + 1;

			//update table
			CHighscoresView::UpdateHighscoreTable(lMissionName, lRank, lPlayerName, lScore, lDate, lTime);

			//clean up
			delete lMissionName;
			delete lRank;
			delete lPlayerName;
			delete lScore;
			delete lDate;
			delete lTime;
		}

		//no problems
		CHighscoresView::ServerNotRespondedError = false;
		CHighscoresView::GameOutdated = false;
	}
	else
	{
		if(lSendStatus == ETCPConnectionEngineSendStatusFailedHandshake)
		{
			//handshake failed, let user know to update his/her game version
			CHighscoresView::GameOutdated = true;
		}
		else
		{
			//something went wrong, we assume that the server is not online
			CHighscoresView::ServerNotRespondedError = true;
		}
	}

	//all done clean up
	for(TInt lIndex = 0; lIndex < lNewHighscoresBuffer->GetCount(); lIndex++)
	{
		delete lNewHighscoresBuffer->Get(lIndex)->MessageContent;
	}
	lNewHighscoresBuffer->ClearAndDestroy();

	delete lNewHighscoresBuffer;
	delete lTCPConnection;

	return NULL;
}

void ProgressPopupCallBack(TBool aScoresHaveBeenUpdated, void* aExtraArgs)
{
	CHighscoresView* lHighscoreView = static_cast<CHighscoresView*>(aExtraArgs);

	if(aScoresHaveBeenUpdated)
	{
		if(CHighscoresView::GameOutdated)
		{
			//create error popup
			SPopupWarningArgs lWarningMessage;
			lWarningMessage.NumberOfSecondsBeforeClosing = 6;
			lWarningMessage.OwningView = lHighscoreView;
			lWarningMessage.LeftButtonStringId = "Frontend.ButtonLabels.Continue";
			lWarningMessage.TitleStringId = "Frontend.Error.VersionOutdatedTitle";
			lWarningMessage.MessageLine1StringId = "Frontend.Error.VersionOutdatedAdviceLine1";
			lWarningMessage.MessageLine2StringId = "Frontend.Error.VersionOutdatedAdviceLine2";
			lWarningMessage.MessageLine3StringId = NULL;
			CPopupWarning::New(lWarningMessage);

			//set back to false
			CHighscoresView::GameOutdated = false;
		}
		else if(CHighscoresView::ServerNotRespondedError)
		{
			//create error popup
			SPopupWarningArgs lWarningMessage;
			lWarningMessage.NumberOfSecondsBeforeClosing = 6;
			lWarningMessage.OwningView = lHighscoreView;
			lWarningMessage.LeftButtonStringId = "Frontend.ButtonLabels.Continue";
			lWarningMessage.TitleStringId = "Frontend.Error.ServerDownTitle";
			lWarningMessage.MessageLine1StringId = "Frontend.Error.ServerDownAdviceLine1";
			lWarningMessage.MessageLine2StringId = "Frontend.Error.ServerDownAdviceLine2";
			lWarningMessage.MessageLine3StringId = NULL;
			CPopupWarning::New(lWarningMessage);

			//set back to false
			CHighscoresView::ServerNotRespondedError = false;
		}
		else
		{
			//refreshed score data
			lHighscoreView->UpdateHighscoreBoxes();
		}
	}
}

void EnterPlayerNameCallback(TBool aPlayerEnteredName, void* aExtraArgs)
{
	CHighscoresView* lHighscoresView = static_cast<CHighscoresView*> (aExtraArgs);

	if(aPlayerEnteredName)
	{
		//upload scores without WLAN
		CFighterPilotThePacificWar::FighterGame->iUserStatsTrackerDaemon->SetUploadUserStatsWithoutWLan(true);

		//record that User Updated score
		CUserStats::UpdatesHighscores();

		//update player name
		//first need to remove whitespaces since it screws up the server
		while(lHighscoresView->iStringTempPlayerName->FindString(0, KWhitespace) != -1)
		{
			TInt lWhitespaceLocation = lHighscoresView->iStringTempPlayerName->FindString(0, KWhitespace);
			CString* lStringBeforeWhiteSpace =  lHighscoresView->iStringTempPlayerName->GetSubString(0, lWhitespaceLocation);
			CString* lStringAfterWhiteSpace= lHighscoresView->iStringTempPlayerName->GetSubString(lWhitespaceLocation + 1, lHighscoresView->iStringTempPlayerName->GetLength() - (lWhitespaceLocation + 1));

			lHighscoresView->iStringTempPlayerName->Replace("");
			if(lStringBeforeWhiteSpace != NULL)
				lHighscoresView->iStringTempPlayerName->Append(lStringBeforeWhiteSpace);
			lHighscoresView->iStringTempPlayerName->Append("_");
			if(lStringAfterWhiteSpace != NULL)
				lHighscoresView->iStringTempPlayerName->Append(lStringAfterWhiteSpace);
			delete lStringBeforeWhiteSpace;
			delete lStringAfterWhiteSpace;
		}

		//update name
		CFighterPilotThePacificWar::FighterGame->iGameVariables->iStringPlayerName->Replace(lHighscoresView->iStringTempPlayerName);

		//send scores with player name
		SPopupProgressBarArgs lPopArgs = {lHighscoresView, "Frontend.ButtonLabels.Cancel", "Frontend.HighscoresView.UpdatingHighscores", ProgressPopupCallBack, lHighscoresView};
		CPopupProgressBar::New(lPopArgs, UploadScores, lHighscoresView);
	}
}

void CHighscoresView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
{
	//only support one 1 Finger avoid confusion
	if (aTouchEvent.FingerId == 0)
	{
		switch (aTouchEvent.Type)
		{
			case ETouchEventHandlerTouchTypeDown:
			{
				//ScrollSurfaces
				iScrollSurface->CheckIfHit(aTouchEvent);
				iScrollBar->CheckIfHit(aTouchEvent);

				//Back and Start Button
				iButtonBack->CheckIfHit(aTouchEvent);
				iButtonUpdate->CheckIfHit(aTouchEvent);

				//can only be selected if none of the controls buttons are selected
				if(!iButtonBack->GetState() == CButton::EStateSelected &&
					 !iButtonUpdate->GetState() == CButton::EStateSelected)
				{
					//check Highscore buttons
					for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
					{
						iHighscoreBoxes->Get(lIndex)->ViewScoreButton->CheckIfHit(aTouchEvent);
					}
				}
				break;
			}
			case ETouchEventHandlerTouchTypeDrag:
			{
				//check if any button has been touched
				TBool lButtonTouched = false;
				for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
				{
					if(iHighscoreBoxes->Get(lIndex)->ViewScoreButton->GetState() == CButton::EStateSelected)
					{
						lButtonTouched = true;
					}
				}
				if(iButtonBack->GetState() == CButton::EStateSelected)
				{
					lButtonTouched = true;
				}
				if(iButtonUpdate->GetState() == CButton::EStateSelected)
				{
					lButtonTouched = true;
				}

				//enable scroll only if no button has been selected
				if(!lButtonTouched)
				{
					//ScrollSurfaces
					iScrollSurface->CheckIfHit(aTouchEvent);
					iScrollBar->CheckIfHit(aTouchEvent);
				}
				break;
			}
			case ETouchEventHandlerTouchTypeUp:
			{
				//ScrollSurfaces
				iScrollSurface->CheckIfHit(aTouchEvent);
				iScrollBar->CheckIfHit(aTouchEvent);

				//check Highscore buttons
				for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
				{
					if (iHighscoreBoxes->Get(lIndex)->ViewScoreButton->GetState() == CButton::EStateSelected
							&& iHighscoreBoxes->Get(lIndex)->ViewScoreButton->CheckIfHit(aTouchEvent))
					{
						//record UserStat
						CUserStats::MissionHighscoreScreen(lIndex);

						//go to Highscores for this mission
						CHighscoresView::InitialYOffset = iYOffsetValue; //gonna keep it around, to use it when we come back
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CMissionHighscoresView::New(iHighscoreBoxes->Get(lIndex)->Mission), 0.5, true);
						return;
					}
				}
				//Back Button, go back to Main Menu
				if(iButtonBack->GetState() == CButton::EStateSelected && iButtonBack->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CMainMenuView::New(false), 2, 100, 1);
					return;
				}
				//Upload new Scores and Download the most recent ones
				if(iButtonUpdate->GetState() == CButton::EStateSelected && iButtonUpdate->CheckIfHit(aTouchEvent))
				{
					if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iAgreementAccepted)
					{
						iStringTempPlayerName->Replace(CFighterPilotThePacificWar::FighterGame->iGameVariables->iStringPlayerName);
						SPopupBasicArgs lPopArgs = {this, "Frontend.ButtonLabels.Continue", "Frontend.ButtonLabels.Cancel", "Frontend.HighscoresView.YourGamertag", EnterPlayerNameCallback, this};
						CPopupEnterText::New(lPopArgs, iStringTempPlayerName, GMAX_CHARACTERS_FOR_PLAYER_NAME);
						return;
					}
					else
					{
						CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CAgreeToTermsScreen::New(true), 0.5, true);
					}
				}
				break;
			}
		}
	}
}

void CHighscoresView::ChildViewDrawMethod()
{
	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//draw Background for Highscore Boxes
	iSelectionBackgroundTexture->DrawArrays();

	//------------- Draw Mission Boxes ----------------//
	TPoint lBoxLocation (112, 467 + iYOffsetValue); //want some space from the top as well
	lBoxLocation.iY -= 200; //need to make space for the first box
	lBoxLocation.iY -= 20; //some space between start

	for(TInt lIndex = 0; lIndex < iHighscoreBoxes->GetCount(); lIndex++)
	{
		//draw box
		iHighscoreBoxTexture->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iHighscoreBoxTexture->DrawArrays();

		//draw Mission image
		iMissionImage->SetNewLowerLeftCoordinate(TPointIntFloat(TIntFloat::Convert(lBoxLocation.iX), TIntFloat::Convert(lBoxLocation.iY)));
		iMissionImage->AssignTexture(&iImageStore->GetImage(iHighscoreBoxes->Get(lIndex)->Mission->MissionImage));
		iMissionImage->DrawArrays();

		//draw Highscore
		iGraphicsFont->SetFont(iFontHighscores);
		iGraphicsFont->SetColour(KColourDarkRed);
		//draw HighScore:
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lBoxLocation.iX + 410, lBoxLocation.iY + 110), iStringHighscore);
		TInt lLenghtOfScore = iGraphicsFont->GetWidthOfString(iFontHighscores, iHighscoreBoxes->Get(lIndex)->Highscore);
		//draw numerical value,is centered to the right
		iGraphicsFont->DrawStringToScreenHorizontally(TPoint(lBoxLocation.iX + (790 - lLenghtOfScore),lBoxLocation.iY + 110), iHighscoreBoxes->Get(lIndex)->Highscore);

		//Draw View Score Button
		iHighscoreBoxes->Get(lIndex)->ViewScoreButton->SetLowerLeftLocation(TPoint(lBoxLocation.iX + 410, lBoxLocation.iY + 14));
		iHighscoreBoxes->Get(lIndex)->ViewScoreButton->Draw();

		//draw Mission Name
		iGraphicsFont->SetFont(iFontMissionNames);
		iGraphicsFont->SetColour(KColourDarkBlue);
		DrawCenteredString(TPoint(lBoxLocation.iX, lBoxLocation.iY + 160), iFontMissionNames, iHighscoreBoxes->Get(lIndex)->MissionName, 800, 40);

		//add space between boxes
		lBoxLocation.iY -= 200; //offset for box length
		lBoxLocation.iY -= 30; //space between boxes
	}

	iBackgroundTexture->DrawArrays();

	//draw Select Mission String
	iGraphicsFont->SetColour(KColourDarkRed);
	iGraphicsFont->SetFont(iFontGlobalHighscores);
	DrawCenteredString(TPoint(360, 480), iFontGlobalHighscores, iStringGlobalHighscores, 310, 40);

	//draw Controls
	iButtonBack->Draw();
	iButtonUpdate->Draw();
	iScrollSurface->Draw();
	iScrollBar->Draw();
}

SRemoteMessageInternalFormat CHighscoresView::CreateMissionHighscoreMessage(SMission& aMission, CString* aPlayerName)
{
	SRemoteMessageInternalFormat lInternalMessage;
	lInternalMessage.MessageContent = new CString("");

	//MissionName
	lInternalMessage.MessageContent->Append(aMission.MissionNameForHighscoreTable->GetData());
	lInternalMessage.MessageContent->Append(KWhitespace);
	//Phone Id
	lInternalMessage.MessageContent->Append(CFighterPilotThePacificWar::FighterGame->iUserStatsTrackerDaemon->GetPhoneSerialNumber()->GetData());
	lInternalMessage.MessageContent->Append(KWhitespace);
	//Name
	lInternalMessage.MessageContent->Append(aPlayerName->GetData());
	lInternalMessage.MessageContent->Append(KWhitespace);
	//Score
	lInternalMessage.MessageContent->AppendNum(aMission.Score);
	lInternalMessage.MessageContent->Append(KWhitespace);
	//Time
	lInternalMessage.MessageContent->Append(aMission.Time->GetData());

#if AMAZON_BUILD
	CAndroidAmazonGameCircleFunctions::UploadScore(aMission.MissionNameForHighscoreTable->GetData(), aMission.Score);
#endif

	return lInternalMessage;
}

void CHighscoresView::UpdateHighscoreTable(CString* aMissionName, CString* aRank, CString* aPlayerName, CString* aScore, CString* aDate, CString* aTime)
{
	CSaveGameObject* lSaveGameObject = CFighterPilotThePacificWar::FighterGame->iSaveGameObject;

	for(TInt lIndex = 1; lIndex < lSaveGameObject->iCampaigns->GetCount(); lIndex++)
	{
		SMission* lMission = NULL;

		//find the correct entry to update
		if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MissionNameForHighscoreTable->EqualTo(aMissionName))
		{
			lMission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission1;
		}
		else if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MissionNameForHighscoreTable->EqualTo(aMissionName))
		{
			lMission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission2;
		}
		else if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MissionNameForHighscoreTable->EqualTo(aMissionName))
		{
			lMission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission3;
		}
		else if(lSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MissionNameForHighscoreTable->EqualTo(aMissionName))
		{
			lMission = &lSaveGameObject->iCampaigns->Get(lIndex)->Mission4;
		}
		else if(lSaveGameObject->iBestOfAllMission.MissionNameForHighscoreTable->EqualTo(aMissionName))
		{
			lMission = &lSaveGameObject->iBestOfAllMission;
		}

		if(lMission != NULL)
		{
			SMissionHighscoreTableEntry* lMissionEntry = NULL;

			//found correct Mission
			if (aRank->EqualTo("1"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank1;
			}
			else if (aRank->EqualTo("2"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank2;
			}
			else if (aRank->EqualTo("3"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank3;
			}
			else if (aRank->EqualTo("4"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank4;
			}
			else if (aRank->EqualTo("5"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank5;
			}
			else if (aRank->EqualTo("6"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank6;
			}
			else if (aRank->EqualTo("7"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank7;
			}
			else if (aRank->EqualTo("8"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank8;
			}
			else if (aRank->EqualTo("9"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank9;
			}
			else if (aRank->EqualTo("10"))
			{
				lMissionEntry = &lMission->HighscoreTable.Rank10;
			}

			if(lMissionEntry != NULL)
			{
				lMissionEntry->PlayerName->Replace(aPlayerName);
				lMissionEntry->Score->Replace(aScore);
				lMissionEntry->Date->Replace(aDate);
				lMissionEntry->Time->Replace(aTime);
			}
		}
	}
}

void CHighscoresView::UpdateOverallScoreEntry(const SMission& aMission, TInt& aTotalScore, TInt& aTotalSeconds)
{
	if(aMission.Enabled)
	{
		if(aMission.Score > 0)
		{
			aTotalScore += aMission.Score;

			//Add time taken to beat this mission
			//expected format hh:mm:ss
			CString* lStringHours = aMission.Time->GetSubString(0, 2);
			CString* lStringMins = aMission.Time->GetSubString(3, 2);
			CString* lStringSecs = aMission.Time->GetSubString(6, 2);

			TInt lNumberOfHours = lStringHours->GetStringAsInt();
			TInt lNumberOfMins = lStringMins->GetStringAsInt();
			TInt lNumberOfSecs = lStringSecs->GetStringAsInt();

			aTotalSeconds += lNumberOfHours * 60 * 60;
			aTotalSeconds += lNumberOfMins * 60;
			aTotalSeconds += lNumberOfSecs;

			//clean up
			delete lStringHours;
			delete lStringMins;
			delete lStringSecs;
		}
	}
}

