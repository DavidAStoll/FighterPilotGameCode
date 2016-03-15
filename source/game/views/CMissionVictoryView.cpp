/*
 * CMissionVictoryView.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: dstoll
 */
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/views/CMissionVictoryView.h"
#include "includes/game/views/CLoadingScreen.h"
#include "includes/game/views/CCampaignScreen.h"
#include "includes/game/common/CUserStats.h"
#include "includes/core/views/popups/CPopupWarning.h"
#include "includes/core/views/transitions/CTransitionHorizontalFlippingTiles.h"
#include "includes/core/views/transitions/CTransitionHorizontalPeelOff.h"
#if AMAZON_BUILD
	#include "includes/native/utility/CAndroidAmazonGameCircleFunctions.h" //need for specific Amazon function call
#endif

#define SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION 70
#define SUCCESSFUL_MISSION_SCREEN_STATS_X_COUNTER_LOCATION 543
#define SUCCESSFUL_MISSION_SCREEN_STATS_X_MULTIPLIER_LOCATION 640
#define SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION 795

#define SUCCESSFUL_MISSION_SCREEN_STAT_BONUS_FOR_EXTRA_LIVES 500
#define SUCCESSFUL_MISSION_SCREEN_MISSION_TIME_DIVION_CONSTANT 10000

CMissionVictoryView::CMissionVictoryView(TBool aMissionWon, TBool aGameSaved, TInt aLifesLeft, TInt aScore, TInt aExpectedMissionTimeInMins, CStatsTracker* aStatsTracker)
{
	iMissionWon = aMissionWon;
	iGameSaved = aGameSaved;
	iLivesLeft = aLifesLeft;
	iTotalScore = aScore;
	iExpectedMissionTimeInMins = aExpectedMissionTimeInMins;
	iStatsYOffset = 0;
	iStatsTracker = aStatsTracker;
	iMission = CFighterPilotThePacificWar::FighterGame->iGameData->iMissionData;
	iCampaign = CFighterPilotThePacificWar::FighterGame->iGameData->iCampaignData;
}

CMissionVictoryView::~CMissionVictoryView()
{
	// TODO Auto-generated destructor stub
}

CMissionVictoryView* CMissionVictoryView::New(TBool aMissionWon, TBool aGameSaved, TInt aLifesLeft, TInt aScore, TInt aExpectedMissionTimeInMins, CStatsTracker* aStatsTracker)
{
	CMissionVictoryView* lSelf = new CMissionVictoryView(aMissionWon, aGameSaved, aLifesLeft, aScore, aExpectedMissionTimeInMins, aStatsTracker);
	lSelf->Construct();
	return lSelf;
}

void CMissionVictoryView::Construct()
{
	iBackgroundTexture = CTextureObject::New(TPoint(0, 0), &CGame::Game->iImageStore->GetImage(EImageIdFrontendMissionVictoryScreen), 1024, 660, 100, 0, 1, 0, 1);
	iDescriptionBackgroundTexture = CTextureObject::New(TPoint(64, 100), &CGame::Game->iImageStore->GetImage(EImageIdFrontendSharedDescriptionBackground), 896, 280, 100, 0, 1, 0, 1);

	//---------- Update Score -----------//
	//For Mission Time
	TInt lMissionTimeInSec = (iStatsTracker->iGameTimeHours * 60) * 60;
	lMissionTimeInSec += (iStatsTracker->iGameTimeMins * 60);
	lMissionTimeInSec += iStatsTracker->iGameTimeSecs;
	//Formula:
	//Ta - Te = Td (Time difference)
	//(Td^3) / Constant = Bonus/Penality
	//should avoid that player that drift of from the normal game time get a huge bonus or penalty
	//while people that are relative close only see a moderate change in their score
	//this should avoid that timeing becomes the single most element, but is rather just one more element to consider
	TInt lMissionDifferenceTime = (iExpectedMissionTimeInMins * 60) - lMissionTimeInSec; //negative if it took longer
	//we do the division first to avoid an overflow
	TFloat lMissionDifferenceTimeSquaredAndDivided = (TFloat) (lMissionDifferenceTime*lMissionDifferenceTime) / SUCCESSFUL_MISSION_SCREEN_MISSION_TIME_DIVION_CONSTANT;
	iTimePenaltyBonus = (lMissionDifferenceTimeSquaredAndDivided) * lMissionDifferenceTime;

	//For Bonus Objectives
	for(TInt lIndex = 0; lIndex < iStatsTracker->iBonusObjectives->GetCount(); lIndex++)
	{
		iTotalScore += iStatsTracker->iBonusObjectives->Get(lIndex)->ExtraScore;
	}

	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyBaby)
		iLivesLeft = 30;

	//Add Extra Lives
	iTotalScore += SUCCESSFUL_MISSION_SCREEN_STAT_BONUS_FOR_EXTRA_LIVES * iLivesLeft;
	//Mission Time
	iTotalScore += iTimePenaltyBonus;
	//SaveGame Multiplier
	if(!iGameSaved)
		iTotalScore *= 2;
	//Add difficulty if not negative
	//add difficulty mutliplier to it if it is not negative
	if(iTotalScore > 0)
	{
		if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyBaby)
			iTotalScore *= 1;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyEasy)
			iTotalScore *= 3;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyMedium)
			iTotalScore *= 4;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyHard)
			iTotalScore *= 6;
	}

	//String
	iStringRestartMission = iLocalizationEngine->GetString("Frontend.ButtonLabels.Restart");
	iStringContinue = iLocalizationEngine->GetString("Frontend.ButtonLabels.Continue");
	if(iMissionWon)
		iStringMissionStatus = iLocalizationEngine->GetString("Frontend.StatsScreen.Victory");
	else
		iStringMissionStatus = iLocalizationEngine->GetString("Frontend.StatsScreen.Defeat");
	iStringMissionName = iLocalizationEngine->GetString(iMission->MissionNameId->GetData());
	//MissionTime
	iStringUserMissionTime = new CString();
	//Hours
	if(iStatsTracker->iGameTimeHours < 10)
		iStringUserMissionTime->AppendNum(0);
	iStringUserMissionTime->AppendNum(iStatsTracker->iGameTimeHours);
	iStringUserMissionTime->Append(":");
	//Mins
	if(iStatsTracker->iGameTimeMins < 10)
		iStringUserMissionTime->AppendNum(0);
	iStringUserMissionTime->AppendNum(iStatsTracker->iGameTimeMins);
	iStringUserMissionTime->Append(":");
	//Secs
	if(iStatsTracker->iGameTimeSecs < 10)
		iStringUserMissionTime->AppendNum(0);
	iStringUserMissionTime->AppendNum(iStatsTracker->iGameTimeSecs);
	//for Stats
	iStringMultiplierSymbol = new CString("× ");
	iStringKills = iLocalizationEngine->GetString("Frontend.StatsScreen.Kills");
	iStringLoses = iLocalizationEngine->GetString("Frontend.StatsScreen.Loses");
	iStringHeavyBomber = iLocalizationEngine->GetString("Frontend.StatsScreen.HeavyBomber");
	iStringDiveBomber = iLocalizationEngine->GetString("Frontend.StatsScreen.DiveBomber");
	iStringAircraftCarrier = iLocalizationEngine->GetString("Frontend.StatsScreen.AircraftCarrier");
	iStringDestroyer = iLocalizationEngine->GetString("Frontend.StatsScreen.Destroyer");
	iStringBattleshipYamato = iLocalizationEngine->GetString("Frontend.StatsScreen.BattleshipYamato");
	iStringLctLoaded = iLocalizationEngine->GetString("Frontend.StatsScreen.LCTLoaded");
	iStringLctEmpty = iLocalizationEngine->GetString("Frontend.StatsScreen.LCTEmpty");
	iStringTroopTransporter = iLocalizationEngine->GetString("Frontend.StatsScreen.TroopTransporter");
	iStringOilTanker = iLocalizationEngine->GetString("Frontend.StatsScreen.OilTanker");
	iStringAirport = iLocalizationEngine->GetString("Frontend.StatsScreen.Airport");
	iStringComStation = iLocalizationEngine->GetString("Frontend.StatsScreen.ComStation");
	iStringHangar = iLocalizationEngine->GetString("Frontend.StatsScreen.Hangar");
	iStringHQ = iLocalizationEngine->GetString("Frontend.StatsScreen.Hq");
	iStringAA20mmMachineGun = iLocalizationEngine->GetString("Frontend.StatsScreen.LightAAGun");
	iStringAA75mmType88 = iLocalizationEngine->GetString("Frontend.StatsScreen.MediumAAGun");
	iStringAAConcrete = iLocalizationEngine->GetString("Frontend.StatsScreen.HeavyAAGun");
	iStringMediumTank = iLocalizationEngine->GetString("Frontend.StatsScreen.MediumTank");
	iStringAATank = iLocalizationEngine->GetString("Frontend.StatsScreen.AATank");
	iStringRifleman = iLocalizationEngine->GetString("Frontend.StatsScreen.Rifleman");
	iStringAntiTankman = iLocalizationEngine->GetString("Frontend.StatsScreen.AntiTankman");
	iStringUSSVictory = iLocalizationEngine->GetString("Frontend.StatsScreen.USSVictory");
	iStringBattleshipUSSTennessee = iLocalizationEngine->GetString("Frontend.StatsScreen.BattleshipUSSTennessee");
	iStringBonus = iLocalizationEngine->GetString("Frontend.StatsScreen.Bonus");
	iStringLivesLeft = iLocalizationEngine->GetString("Frontend.StatsScreen.ExtraLives");
	iStringMissionTime = iLocalizationEngine->GetString("Frontend.StatsScreen.MissionTime");
	iStringSaveGameMultiplier = iLocalizationEngine->GetString("Frontend.StatsScreen.SaveGame");
	iStringDifficultyMultiplier = iLocalizationEngine->GetString("Frontend.StatsScreen.DifficultyMultiplier");
	iStringTotalScore = iLocalizationEngine->GetString("Frontend.StatsScreen.TotalScore");

	//for user stats tracking
	CUserStats::EndMission(iLivesLeft, iMissionWon, iTotalScore);

	//Font
	iFontForMissionStatus = iGraphicsFont->CreateFont(40); //always have enough space
	iFontForMissionName = iGraphicsFont->CreateFontThatFitsHorizontalString(iStringMissionName, 600, 30);
	if(iLocalizationEngine->GetLanguage() == ELanguageChineseSimplified || iLocalizationEngine->GetLanguage() == ELanguageChineseTraditional)
	{
		iFontForStats = iGraphicsFont->CreateFont(40);
	}
	else
	{
		iFontForStats = iGraphicsFont->CreateFont(35);
	}


	TInt lMaxYOffset = FindMaxY(iFontForMissionStatus);

	//ForButtons
	CPointerArray<CString>* lButtonStringArray = CPointerArray<CString>::New();
	lButtonStringArray->Append(iStringRestartMission);
	lButtonStringArray->Append(iStringContinue);
	iFontForButtons = iGraphicsFont->CreateFontThatFitsHorizontalStrings(lButtonStringArray, 190, 60);
	//clean up
	delete lButtonStringArray;
	lButtonStringArray = NULL;

	//Restart Button
	CButton::SButtonArguments lRestartButtonArgs;
	lRestartButtonArgs.Location = TPoint(151, 0);
	lRestartButtonArgs.SizeOfHitbox = TSize(210, 85);
	lRestartButtonArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButton);
	lRestartButtonArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedMenuNavigationButtonSelected);
	lRestartButtonArgs.SizeOfTexture = TSize(210, 85);
	lRestartButtonArgs.Label = iStringRestartMission;
	lRestartButtonArgs.FontForLabel = &iFontForButtons;
	lRestartButtonArgs.ColourForLabelDefault = KColourBlack;
	lRestartButtonArgs.ColourForLabelSelected = KColourBlack;
	iButtonRestart = CButton::New(lRestartButtonArgs);

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

	//ScrollBar for Stats
	CScrollBar::SScrollBarArguments lScrollBarArgs;
	lScrollBarArgs.Location = TPoint(970, 290);
	lScrollBarArgs.SizeOfHitbox = TSize(80, 200);
	lScrollBarArgs.SizeOfTexture = TSize(28, 90);
	lScrollBarArgs.TextureIdDefault = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmall);
	lScrollBarArgs.TextureIdSelected = &iImageStore->GetImage(EImageIdFrontendSharedVerticalScrollBarSmallSelected);
	lScrollBarArgs.MaxScroll = lMaxYOffset;
	lScrollBarArgs.MaxScrollHeight = 280;
	lScrollBarArgs.ScrollSpeedFactor = 4;
	lScrollBarArgs.ScrollAxis = CScrollBar::EScrollAxisY;
	iScrollBar = CScrollBar::New(lScrollBarArgs, iStatsYOffset);

	iTouchSurface = CTouchSurface::New(TPoint(0,100), TSize(960, 280), iStatsYOffset, lMaxYOffset, 2, CTouchSurface::EScrollAxisY);


	//if Mission is won, update Score and unlock next map
	if(iMissionWon)
	{
		if(iTotalScore > iMission->Score)
		{
			iMission->Score = iTotalScore;
			iMission->Time = new CString(iStringUserMissionTime->GetData());
		}
		//update what difficulty has been unlocked
		if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyBaby)
			iMission->CompletedBaby = true;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyEasy)
			iMission->CompletedEasy = true;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyMedium)
			iMission->CompletedMedium = true;
		else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyHard)
			iMission->CompletedHard = true;
		//unlock next Mission
		CFighterPilotThePacificWar::FighterGame->iSaveGameObject->UnlockNextMission(iMission->MapId);
		//save score to disk
		CFighterPilotThePacificWar::FighterGame->iSaveGameObject->SaveToDisk();

#if AMAZON_BUILD
		//upload score to Amazon Gamecircle
		CAndroidAmazonGameCircleFunctions::UploadScore(iMission->MissionNameForHighscoreTable->GetData(), iMission->Score);
#endif

	}


	//maybe settings changed
	CGame::Game->iAudioEngine->CheckPlayerProfileIfAudioHasToBeMuted();
	//play Main Menu Song again
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iIsUserSong)
	{
		CGame::Game->iAudioEngine->GetMusicPlayer().PlayUserSong(CFighterPilotThePacificWar::FighterGame->iGameVariables->iFrontendSong->GetData(), true);
	}
	else
	{
		CGame::Game->iAudioEngine->GetMusicPlayer().PlayGameSong(CFighterPilotThePacificWar::FighterGame->iGameVariables->iFrontendSong->GetData(), true);
	}
	//enable audio again
	CGame::Game->iAudioEngine->GetSoundEngine().iSoundPlayer->Resume();
	CGame::Game->iAudioEngine->GetMusicPlayer().Resume();
}

//------------------ functions -------------------//
void CMissionVictoryView::ChildConsumeTouchEvent(const STouchEventLocationConverted& aTouchEvent)
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
				iButtonRestart->CheckIfHit(aTouchEvent);
				iButtonContinue->CheckIfHit(aTouchEvent);
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
				if (iButtonRestart->GetState() == CButton::EStateSelected && iButtonRestart->CheckIfHit(aTouchEvent))
				{
					//need to find out Mission and Campaign Index
					TInt lMissionIndex = 0;
					TInt lCampaignIndex = 0;
					for(TInt lIndex = 0; lIndex < CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->GetCount(); lIndex++)
					{
						if(iCampaign == CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex))
						{
							lCampaignIndex = lIndex;
							if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission1.MapId == iMission->MapId)
							{
								lMissionIndex = 0;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission2.MapId == iMission->MapId)
							{
								lMissionIndex = 1;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission3.MapId == iMission->MapId)
							{
								lMissionIndex = 2;
							}
							else if(CFighterPilotThePacificWar::FighterGame->iSaveGameObject->iCampaigns->Get(lIndex)->Mission4.MapId == iMission->MapId)
							{
								lMissionIndex = 3;
							}
							break; //all done
						}
					}
					//record user stat
					CUserStats::PlaysMission(lCampaignIndex, lMissionIndex, CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty, false);
					CUserStats::EndMissionInit(lCampaignIndex, lMissionIndex, CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty, false);

					CTransitionHorizontalFlippingTiles::New(&CGame::Game->iView, this, CLoadingScreen::New(iCampaign, iMission, ELoadingScreenModeStartMap), 5, 4, 4, 30);
				}
				//go back to GameSettings, keep playing the current Song
				if (iButtonContinue->GetState() == CButton::EStateSelected && iButtonContinue->CheckIfHit(aTouchEvent))
				{
					CTransitionHorizontalPeelOff::New(&CGame::Game->iView, this, CCampaignScreen::New(iCampaign), 0.5, false);
				}
				break;
			}
		}
	}
}

void CMissionVictoryView::ChildViewDrawMethod()
{
	if(!iIsDoingTransition) //otherwise doesn't work to create a popup during a transition
	{
		if(iMissionWon && CFighterPilotThePacificWar::FighterGame->iGameVariables->iFirstTimeGettingHighscore && iTotalScore > 0)
		{
			//check that this is a not a tutorial
			if(!iCampaign->TutorialCampaign)
			{
				//create New Highscore Message popup
				SPopupWarningArgs lNewHighscoreMessage;
				lNewHighscoreMessage.NumberOfSecondsBeforeClosing = 30;
				lNewHighscoreMessage.OwningView = this;
				lNewHighscoreMessage.LeftButtonStringId = "Frontend.ButtonLabels.Continue";
				lNewHighscoreMessage.TitleStringId = "Frontend.StatsScreen.NewHighscorePopupTitle";
				lNewHighscoreMessage.MessageLine1StringId = "Frontend.StatsScreen.NewHighscorePopupMessageLine1";
				lNewHighscoreMessage.MessageLine2StringId = "Frontend.StatsScreen.NewHighscorePopupMessageLine2";
				lNewHighscoreMessage.MessageLine3StringId = NULL;
				CPopupWarning::New(lNewHighscoreMessage);

				//don't display popup again
				CFighterPilotThePacificWar::FighterGame->iGameVariables->iFirstTimeGettingHighscore = false;
			}
		}
	}

	iGraphicsEngine->SetClearColour(KColourBlack);
	iGraphicsEngine->ClearScreen();

	//Background for Stats
	iDescriptionBackgroundTexture->DrawArrays();

	//draw Stats
	TPoint lStatsLocation(0, 370 + iStatsYOffset);
	//------------- KILLS --------------//
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	iGraphicsFont->SetFont(iFontForStats);
	iGraphicsFont->SetColour(KColourBlack);
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringKills);
	//Planes
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHeavyBomber, iStatsTracker->iNumberOfHeavyBombersKilled, BETTY_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringDiveBomber, iStatsTracker->iNumberOfDiveBombersKilled, VAL_KILL_BASE_SCORE);
	//Ships
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAircraftCarrier, iStatsTracker->iNumberOfAircraftCarriersKilled, CARRIER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringDestroyer, iStatsTracker->iNumberOfDestroyerKilled, DESTROYER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringBattleshipYamato, iStatsTracker->iNumberOfBattleshipYamatoKilled, JAPANESE_BATTLESHIP_YAMATO_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringLctLoaded, iStatsTracker->iNumberOfLCTLoadedKilled, JAPANESE_LCT_KILL_BASE_SCORE_LOADED);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringLctEmpty, iStatsTracker->iNumberOfLCTEmptyKilled, JAPANESE_LCT_KILL_BASE_SCORE_EMPTY);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringTroopTransporter, iStatsTracker->iNumberOfTroopTransporterKilled, TROOP_TRANSPORTER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringOilTanker, iStatsTracker->iNumberOfOilTankerKilled, OIL_TANKER_KILL_BASE_SCORE);
	//Buildings
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAirport, iStatsTracker->iNumberOfAirportsKilled, AIRPORT_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringComStation, iStatsTracker->iNumberOfComStationKilled, COMSTATION_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHangar, iStatsTracker->iNumberOfHangarKilled, HANGAR_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHQ, iStatsTracker->iNumberOfHqKilled, HQ_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAA20mmMachineGun, iStatsTracker->iNumberOfAA20mmMachineCannonKilled, JAPANESE_AA_20MM_MACHINE_CANNON_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAA75mmType88, iStatsTracker->iNumberOfAA75mmType88Killed, JAPANESE_AA_75MM_TYPE88_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAAConcrete, iStatsTracker->iNumberOfAAConcreteKilled, JAPANESE_AA_CONCRETE_KILL_BASE_SCORE);
	//Ground Units
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringMediumTank, iStatsTracker->iNumberOfMediumTanksKilled, JAPANESE_97_CHI_HA_TANK_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAATank, iStatsTracker->iNumberOfAATanksKilled, JAPANESE_AA_97_CHI_HA_TANK_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringRifleman, iStatsTracker->iNumberOfRiflemanKilled, JAPANESE_RIFLEMAN_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAntiTankman, iStatsTracker->iNumberOfAntiTankmanKilled, JAPANESE_ROCKETMAN_KILL_BASE_SCORE);
	//Add Empty Line
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();

	//------------- Loses --------------//
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	iGraphicsFont->SetFont(iFontForStats);
	iGraphicsFont->SetColour(KColourBlack);
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringLoses);
	//Planes
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHeavyBomber, iStatsTracker->iNumberOfHeavyBombersLost, -MITCHELL_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringDiveBomber, iStatsTracker->iNumberOfDiveBombersLost, -DAUNTLESS_KILL_BASE_SCORE);
	//Ships
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringUSSVictory, iStatsTracker->iNumberOfUSSVictoryLost, -USS_VICTORY_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAircraftCarrier, iStatsTracker->iNumberOfAircraftCarriersLost, -CARRIER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringDestroyer, iStatsTracker->iNumberOfDestroyerLost, -DESTROYER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringBattleshipUSSTennessee, iStatsTracker->iNumberOfBattleshipUSSTennesseeLost, -AMERICAN_BATTLESHIP_USS_TENNESSEE_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringLctLoaded, iStatsTracker->iNumberOfLCTLoadedLost, -AMERICAN_LCT_KILL_BASE_SCORE_LOADED);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringLctEmpty, iStatsTracker->iNumberOfLCTEmptyLost,-AMERICAN_LCT_KILL_BASE_SCORE_EMPTY);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringTroopTransporter, iStatsTracker->iNumberOfTroopTransporterLost, -TROOP_TRANSPORTER_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringOilTanker, iStatsTracker->iNumberOfOilTankerLost, -OIL_TANKER_KILL_BASE_SCORE);
	//Buildings
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAirport, iStatsTracker->iNumberOfAirportsLost, -AIRPORT_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringComStation, iStatsTracker->iNumberOfComStationLost, -COMSTATION_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHangar, iStatsTracker->iNumberOfHangarLost, -HANGAR_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringHQ, iStatsTracker->iNumberOfHqLost, -HQ_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAA20mmMachineGun, iStatsTracker->iNumberOfAA20mmMachineCannonLost, -JAPANESE_AA_20MM_MACHINE_CANNON_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAA75mmType88, iStatsTracker->iNumberOfAA75mmType88Lost, -JAPANESE_AA_75MM_TYPE88_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAAConcrete, iStatsTracker->iNumberOfAAConcreteLost, -JAPANESE_AA_CONCRETE_KILL_BASE_SCORE);
	//Ground Units
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringMediumTank, iStatsTracker->iNumberOfMediumTanksLost, -AMERICAN_SHERMAN_TANK_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAATank, iStatsTracker->iNumberOfAATanksLost, -AMERICAN_AA_SHERMAN_TANK_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringRifleman, iStatsTracker->iNumberOfRiflemanLost, -AMERICAN_RIFLEMAN_KILL_BASE_SCORE);
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringAntiTankman, iStatsTracker->iNumberOfAntiTankmanLost, -AMERICAN_ROCKETMAN_KILL_BASE_SCORE);
	//Add Empty Line
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();

	//------------- Bonus --------------//
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringBonus);
	//go through all Bonus Events
	for(TInt lIndex = 0; lIndex < iStatsTracker->iBonusObjectives->GetCount(); lIndex++)
	{
		lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
		//draw Name of Bonus Event
		lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
		iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStatsTracker->iBonusObjectives->Get(lIndex)->NameOfObjective);
		//draw Extra Score
		lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION;
		CString* lStringBonusScore = new CString();
		lStringBonusScore->AppendNum(iStatsTracker->iBonusObjectives->Get(lIndex)->ExtraScore);
		iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, lStringBonusScore);
		delete lStringBonusScore;
	}

	//------------- Lives Left --------------//
	DrawStatIfGreaterThanZero(lStatsLocation, iFontForStats, iStringLivesLeft, iLivesLeft, SUCCESSFUL_MISSION_SCREEN_STAT_BONUS_FOR_EXTRA_LIVES);

	//------------- MissionTime --------------//
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	//draw Mission Time lable
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringMissionTime);
	//draw Mission User Time
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_COUNTER_LOCATION;
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringUserMissionTime);
	//draw Bonus/Penalty Value for Mission
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION;
	CString* lStringMissionTimeBonus = new CString();
	lStringMissionTimeBonus->AppendNum(iTimePenaltyBonus);
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, lStringMissionTimeBonus);
	delete lStringMissionTimeBonus;

	//------------- SaveGame --------------//
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	//SaveGame Multiplier label
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringSaveGameMultiplier);
	//Multiplier
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION;
	CString* lStringSaveGameMultiplier = new CString(iStringMultiplierSymbol->GetData());
	if(iGameSaved)
		lStringSaveGameMultiplier->AppendNum(1);
	else
		lStringSaveGameMultiplier->AppendNum(2);
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, lStringSaveGameMultiplier);
	delete lStringSaveGameMultiplier;

	//------------- Difficulty --------------//
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	//Difficulty Multiplier label
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, iStringDifficultyMultiplier);
	//Multiplier
	CString* lStringDifficultyMultiplier = new CString(iStringMultiplierSymbol->GetData());
	if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyBaby)
		lStringDifficultyMultiplier->AppendNum(1);
	else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyEasy)
		lStringDifficultyMultiplier->AppendNum(3);
	else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyMedium)
		lStringDifficultyMultiplier->AppendNum(4);
	else if(CFighterPilotThePacificWar::FighterGame->iGameVariables->iDifficulty == EGameDifficultyHard)
		lStringDifficultyMultiplier->AppendNum(6);
	lStatsLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION;
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, lStringDifficultyMultiplier);
	delete lStringDifficultyMultiplier;

	//------------- Total Score --------------//
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
	lStatsLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt(); //Add one extra line
	//draw Total Score label together with Score
	CString* lStringTotalScoreLabelAndValue = new CString(iStringTotalScore->GetData());
	lStringTotalScoreLabelAndValue->AppendNum(iTotalScore);
	lStatsLocation.iX = 950 - iGraphicsFont->GetWidthOfString(iFontForStats, lStringTotalScoreLabelAndValue);
	iGraphicsFont->DrawStringToScreenHorizontally(lStatsLocation, lStringTotalScoreLabelAndValue);

	//draw Background Texture
	iBackgroundTexture->DrawArrays();

	//draw Mission Status
	iGraphicsFont->SetFont(iFontForMissionStatus);
	iGraphicsFont->SetColour(KColourSkyBlue);
	DrawCenteredString(TPoint(0,610), iFontForMissionStatus, iStringMissionStatus, 1024, 50);

	//draw Mission Name
	iGraphicsFont->SetFont(iFontForMissionName);
	iGraphicsFont->SetColour(KColourDarkRed);
	DrawCenteredString(TPoint(200,420), iFontForMissionName, iStringMissionName, 625, 40);

	//draw Controls
	iButtonRestart->Draw();
	iButtonContinue->Draw();
	iScrollBar->Draw();
	iTouchSurface->Draw();

}

void CMissionVictoryView::DrawStatIfGreaterThanZero(TPoint& aTopLocation, const SFont& aFont, CString* aString, TInt aNumberOfStat, TInt aMultiplierValue)
{
	if(aNumberOfStat > 0)
	{
		iGraphicsFont->SetFont(aFont);
		iGraphicsFont->SetColour(KColourBlack);
		//adjust height
		aTopLocation.iY -= iFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();

		//draw Stats Data Name
		aTopLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_START_LOCATION;
		iGraphicsFont->DrawStringToScreenHorizontally(aTopLocation, aString);
		//Draw Number of Events
		aTopLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_COUNTER_LOCATION;
		CString* lStringCounter = new CString();
		lStringCounter->AppendNum(aNumberOfStat);
		iGraphicsFont->DrawStringToScreenHorizontally(aTopLocation, lStringCounter);
		delete lStringCounter;
		//draw multiplier value
		aTopLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_MULTIPLIER_LOCATION;
		CString* lStringMultiplier = new CString(iStringMultiplierSymbol->GetData());
		lStringMultiplier->AppendNum(aMultiplierValue);
		iGraphicsFont->DrawStringToScreenHorizontally(aTopLocation, lStringMultiplier);
		delete lStringMultiplier;
		//draw Total Sum
		aTopLocation.iX = SUCCESSFUL_MISSION_SCREEN_STATS_X_SUM_LOCATION;
		CString* lStringSum = new CString();
		lStringSum->AppendNum(aMultiplierValue * aNumberOfStat);
		iGraphicsFont->DrawStringToScreenHorizontally(aTopLocation, lStringSum);
		delete lStringSum;
	}
}

TInt CMissionVictoryView::FindMaxY(const SFont& aFontForStats)
{
	TInt lMaxYScroll = 0;
	TInt lNumberOfLinesUsed = 12;
	lNumberOfLinesUsed += iStatsTracker->iBonusObjectives->GetCount();

	//planes
		if(iStatsTracker->iNumberOfHeavyBombersKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfDiveBombersKilled)
			lNumberOfLinesUsed++;
		//ships
		if(iStatsTracker->iNumberOfAircraftCarriersKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfDestroyerKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfBattleshipYamatoKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfLCTLoadedKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfLCTEmptyKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfTroopTransporterKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfOilTankerKilled)
			lNumberOfLinesUsed++;
		//buildings
		if(iStatsTracker->iNumberOfAirportsKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfComStationKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfHangarKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfHqKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAA20mmMachineCannonKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAA75mmType88Killed)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAAConcreteKilled)
			lNumberOfLinesUsed++;
		//troops
		if(iStatsTracker->iNumberOfMediumTanksKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAATanksKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfRiflemanKilled)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAntiTankmanKilled)
			lNumberOfLinesUsed++;

		//planes
		if(iStatsTracker->iNumberOfHeavyBombersLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfDiveBombersLost)
			lNumberOfLinesUsed++;
		//ships
		if(iStatsTracker->iNumberOfUSSVictoryLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAircraftCarriersLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfDestroyerLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfBattleshipUSSTennesseeLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfLCTLoadedLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfLCTEmptyLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfTroopTransporterLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfOilTankerLost)
			lNumberOfLinesUsed++;
		//buildings
		if(iStatsTracker->iNumberOfAirportsLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfComStationLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfHangarLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfHqLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAA20mmMachineCannonLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAA75mmType88Lost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAAConcreteLost)
			lNumberOfLinesUsed++;
		//troops
		if(iStatsTracker->iNumberOfMediumTanksLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAATanksLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfRiflemanLost)
			lNumberOfLinesUsed++;
		if(iStatsTracker->iNumberOfAntiTankmanLost)
			lNumberOfLinesUsed++;

		TInt lNumberOfLinesThatCanBeDisplayed = 280 / aFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
		//number of lines we can scroll down, plus 1 just to make sure we have enough space at the bottom
		lMaxYScroll = ((lNumberOfLinesUsed + 1) - lNumberOfLinesThatCanBeDisplayed) * aFontForStats.HeightAndDescentOfFont.GetIntInBaseInt();
		if(lMaxYScroll < 0)
			lMaxYScroll = 0;

		return lMaxYScroll;
}

