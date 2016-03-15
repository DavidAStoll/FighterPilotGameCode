/*
 * CUserStats.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */

#include "includes/game/common/CUserStats.h"
#include "includes/core/utility/CSystem.h"
#include "includes/core/CGame.h"

//For Start Info
#define KStartInfo "StartedInfo"
#define KStartInfoFile "StartedInfo.ini"
#define KNumberOfDaysPlayed "NumberOfDaysPlayed"
#define KNumberOfTimesStarted "NumberOfTimesStarted"
//Phone Model
#define KPhoneModel "PhoneModel"
#define KPhoneModelName "ModelName"
//for Game Time
#define KGameTime "GameTime"
#define KGameTimeInMintues "GameTimeInMinutes"
//for FrontEnd Interfaces
#define KMainMenu "MainMenu"
#define KGameSettingsMenuFrontend "GameSettingsMenuFrontend"
#define KGameSettingsMenuInGame "GameSettingsMenuInGame"
#define KChangeSongMenuFrontend "ChangeSongMenuFrontend"
#define KChangeSongMenuInGame "ChangeSongMenuInGame"
#define KCreditsMenu "CreditsMenu"
#define KSelectCampaignMenu "SelectCampaignMenu"
#define KContinueFromLastSaveMenu "ContinueFromLastSaveMenu"
#define KSelectLanguageMenu "SelectLanguageMenu"
#define KWatchOnlineGameVideos "WatchOnlineGameVideos"
#define KBuyGameMenu "BuyGameMenu"
//language selected
#define KLanguageSelected "LanguageSelected"
#define KLanguage "Language"
//change song
#define KChangeSongFunction "ChangeSongFunction"
#define KChangeSongFrontend "OpenedInFrontend"
#define KSongName "SongName"
//Change Sound Volume
#define KChangeSoundVolume "ChangeSoundVolume"
#define KChangeSoundVolumeFrontend "OpenedInFrontend"
#define KChangeSoundVolumeValue "Value"
//Change Music Volume
#define KChangeMusicVolume "ChangeMusicVolume"
#define KChangeMusicVolumeFrontend "OpenedInFrontend"
#define KChangeMusicVolumeValue "Value"
//Use Icon Buttons
#define KUseIconButtons "UseIconButtons"
//Selected Campaign
#define KSelectedCampaign "SelectedCampaign"
#define KCampaignIndex "CampaignIndex"
//Uses SilentProfile
#define KUsesSilentProfile "UsesSilentProfile"
//PlaysMission
#define KPlayMission "PlayMission"
#define KMissionCampaignIndex "CampaignIndex"
#define KMissionIndex "MissionIndex"
#define KMissionDifficulty "MissionDifficulty"
#define KMissionLoading "Loading"
//CampaignBriefingScreen
#define KCampaignBriefingScreen "CampaignBriefingScreen"
#define KCampaignBriefingScreenCampaignIndex "CampaignIndex"
#define KCampaignBriefingScreenDebriefingScreen "IsDebriefingScreen"
//End Mission
#define KEndMission "EndMission"
#define KEndMissionCampaignIndex "CampaignIndex"
#define KEndMissionMissionIndex "MissionIndex"
#define KEndMissionLoadedMission "LoadedMission"
#define KEndMissionMissionTimeInMinutes "MissionTimeInMinutes"
#define KEndMissionSuccessfulCompleted "SuccessfulCompleted"
#define KEndMissionDifficulty "Difficulty"
#define KEndMissionScore "Score"
#define KEndMissionLivesLeft "LivesLeft"
#define KEndMissionNumberOfSaves "NumberOfSaves"
#define KEndMissionNumberOfLoads "NumberOfLoads"
#define KEndMissionNumberOfRestarts "NumberOfRestarts"
//Tutorial Section
#define KTutorialSection "TutorialSection"
#define KTutorialSectionName "SectionName"
#define KTutorialSectionTime "PrevSectionTimeInMin"
//Check Highscore Screen
#define KGeneralCheckHighScoreScreen "GeneralCheckHighscoreScreen"
//Updates highscores
#define KUpdatesHighscores "UpdatesHighscores"
//Mission Highscore Screen
#define KMissionHighscoreScreen "MissionHighscoreScreen"
#define KMissionHighscoreScreenMissionIndex "MissionIndex"
//Tutorial Section Skipped
#define KTutorialSectionSkipped "TutorialSectionSkipped"
#define KTutorialSectionIndex "SectionIndex"

TUint64 CUserStats::GameStartTime = 0; //keeps track when the game starts
TUint64 CUserStats::MissionStartTime = 0; //keeps track when the game starts
TUint64 CUserStats::TutorialSectionStartTime = 0;
TInt CUserStats::MissionNumberOfSaves = 0;
TInt CUserStats::MissionNumberOfLoads = 0;
TInt CUserStats::MissionNumberOfRestarts = 0;
TInt CUserStats::MissionIndex = 0;
TInt CUserStats::MissionCampaignIndex = 0;
TInt CUserStats::MissionDifficulty = 0;
TBool CUserStats::MissionIsLoading = false;

void CUserStats::GameStarted()
{
	TInt lNumberOfDaysPlayed = 0;
	TInt lNumberOfTimesStarted = 0;
	CString* lStringLastDayPlayed = new CString("0");

	//read data from previous startup, if possible
	CNativeFile* lSaveFile = new CNativeFile(KStartInfoFile);
	CFileReadStream* lReadStream = lSaveFile->OpenForReading();

	if(lReadStream != NULL)
	{
		lNumberOfDaysPlayed = lReadStream->ReadInt32();
		lNumberOfTimesStarted = lReadStream->ReadInt32();
		lReadStream->ReadString(lStringLastDayPlayed);
	}
	delete lSaveFile;
	lSaveFile = NULL;

	//find out what day it is
	CString* lStringSystemTime = CSystem::GetDateNative();
	CString* lStringTodaysDay = lStringSystemTime->GetSubString(8, 2);

	//update update for how many consecutive days the game has been played
	if(!lStringLastDayPlayed->EqualTo(lStringTodaysDay))
	{
		lNumberOfDaysPlayed++;
		lStringLastDayPlayed->Replace(lStringTodaysDay);
	}
	//update how many times the game has been started
	lNumberOfTimesStarted++;

	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KStartInfo);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KNumberOfDaysPlayed, lNumberOfDaysPlayed);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KNumberOfTimesStarted, lNumberOfTimesStarted);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);

	//write new data to file
	lSaveFile = new CNativeFile(KStartInfoFile);
	CFileWriteStream* lWriteStream = lSaveFile->OpenForWriting(EFileWriteStreamModeReplace);

	if(lWriteStream != NULL)
	{
		lWriteStream->WriteInt32(lNumberOfDaysPlayed);
		lWriteStream->WriteInt32(lNumberOfTimesStarted);
		lWriteStream->WriteString(lStringLastDayPlayed);
	}
	//clean up
	delete lSaveFile;
	delete lStringSystemTime;
	delete lStringTodaysDay;
	delete lStringLastDayPlayed;
}

void CUserStats::GameStartedTime()
{
	CUserStats::GameStartTime = CSystem::GetCurrentTimeInMicroSecondsNative();
}

void CUserStats::GameFinishedTime()
{
	TUint64 lTimeBetweenStartAndFinish = CSystem::GetCurrentTimeInMicroSecondsNative() - CUserStats::GameStartTime;
	TInt lTimeInMinutes = (lTimeBetweenStartAndFinish / 1000000) / 60; //convert to Mins from MicroSecs

	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KGameTime);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KGameTimeInMintues, lTimeInMinutes);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::RecordPhoneModel()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KPhoneModel);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KPhoneModelName, CSystem::GetPhoneModelNative()->GetData()); //int value is not important
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::TutorialSectionInit()
{
	CUserStats::TutorialSectionStartTime = CSystem::GetCurrentTimeInMicroSecondsNative();
}

void CUserStats::TutorialSection(const char* aTutorialSectionName)
{
	TUint64 lTimeBetweenStartAndFinish = CSystem::GetCurrentTimeInMicroSecondsNative() - CUserStats::TutorialSectionStartTime;
	TInt lTimeInMinutes = (lTimeBetweenStartAndFinish / 1000000) / 60; //convert to Mins from MicroSecs

	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KTutorialSection);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KTutorialSectionName, aTutorialSectionName);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KTutorialSectionTime, lTimeInMinutes);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::TutorialSectionSkipped(TInt aSectionIndex)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KTutorialSectionSkipped);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KTutorialSectionIndex, aSectionIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::EndMissionInit(TInt aCampaignIndex, TInt aMissionIndex, TInt aDifficulty, TBool aIsLoading)
{
	CUserStats::MissionStartTime = CSystem::GetCurrentTimeInMicroSecondsNative();
	CUserStats::MissionNumberOfSaves = 0;
	CUserStats::MissionNumberOfLoads = 0;
	CUserStats::MissionNumberOfRestarts = 0;
	CUserStats::MissionCampaignIndex = aCampaignIndex;
	CUserStats::MissionIndex = aMissionIndex;
	CUserStats::MissionDifficulty = aDifficulty;
	CUserStats::MissionIsLoading = aIsLoading;
}

void CUserStats::EndMission(TInt aLivesLeft, TInt aCompletedSuccessfully, TInt aScore)
{
	TUint64 lTimeBetweenStartAndFinish = CSystem::GetCurrentTimeInMicroSecondsNative() - CUserStats::MissionStartTime;
	TInt lTimeInMinutes = (lTimeBetweenStartAndFinish / 1000000) / 60; //convert to Mins from MicroSecs

	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KEndMission);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionCampaignIndex, CUserStats::MissionCampaignIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionMissionIndex, CUserStats::MissionIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionLoadedMission, CUserStats::MissionIsLoading);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionMissionTimeInMinutes, lTimeInMinutes);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionSuccessfulCompleted, aCompletedSuccessfully);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionDifficulty, CUserStats::MissionDifficulty);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionScore, aScore);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionLivesLeft, aLivesLeft);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionNumberOfSaves, CUserStats::MissionNumberOfSaves);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionNumberOfLoads, CUserStats::MissionNumberOfLoads);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KEndMissionNumberOfRestarts, CUserStats::MissionNumberOfRestarts);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::EndMisssionIncreaseNumberOfRestarts()
{
	CUserStats::MissionNumberOfRestarts++;
}

void CUserStats::EndMisssionIncreaseNumberOfLoads()
{
	CUserStats::MissionNumberOfLoads++;
}

void CUserStats::EndMisssionIncreaseNumberOfSaves()
{
	CUserStats::MissionNumberOfSaves++;
}

void CUserStats::CampaignBriefingScreenOpenend(TInt aCampaignIndex, TBool aDebriefingScreen)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KCampaignBriefingScreen);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KCampaignBriefingScreenCampaignIndex, aCampaignIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KCampaignBriefingScreenDebriefingScreen, aDebriefingScreen);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::PlaysMission(TInt aCampaignIndex, TInt aMissionIndex, TInt aDifficulty, TBool aLoading)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KPlayMission);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KCampaignIndex, aCampaignIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KMissionIndex, aMissionIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KMissionDifficulty, aDifficulty);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KMissionLoading, aLoading);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::UsesSilentProfile()
{
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KUsesSilentProfile);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KUsesSilentProfile, CSystem::GetIsSilentModeActivated());
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::SelectedCampaign(TInt aCampaignIndex)
{
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KSelectedCampaign);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KCampaignIndex, aCampaignIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::UseIconButtons(TBool aUseIcons)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KUseIconButtons);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KUseIconButtons, aUseIcons); //int value is not important
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::SoundEffectsVolumeChanged(TBool aFrontend, TInt aValue)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KChangeSoundVolume);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KChangeSoundVolumeFrontend, aFrontend);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KChangeSoundVolumeValue, aValue);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::MusicVolumeChanged(TBool aFrontend, TInt aValue)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KChangeMusicVolume);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KChangeMusicVolumeFrontend, aFrontend);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KChangeMusicVolumeValue, aValue);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::ChangeSong(TBool aFrontend, const char* aSongName)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KChangeSongFunction);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KChangeSongFrontend, aFrontend);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KSongName, aSongName);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::LanguageSelected(const char* aLanguage)
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KLanguageSelected);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KLanguage, aLanguage); //int value is not important
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

//frontend menus, might be a good idea so we know what a users clicks on during his lifetime
void CUserStats::GameSettingsMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KGameSettingsMenuFrontend);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

//frontend menus, might be a good idea so we know what a users clicks on during his lifetime
void CUserStats::GameSettingsMenuOpenedInGame()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KGameSettingsMenuInGame);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}


//frontend menus, might be a good idea so we know what a users clicks on during his lifetime
void CUserStats::ChangeSongMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KChangeSongMenuFrontend);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

//frontend menus, might be a good idea so we know what a users clicks on during his lifetime
void CUserStats::ChangeSongMenuOpenedInGame()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KChangeSongMenuInGame);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::CreditsMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KCreditsMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::SelectCampaignMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KSelectCampaignMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::ContinueFromLastSaveMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KContinueFromLastSaveMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::SelectLanguageMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KSelectLanguageMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::WatchOnlineGameVideosMenuOpenedFrontend()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KWatchOnlineGameVideos);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::BuyGameMenu()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KBuyGameMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::MainMenu()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KMainMenu);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::GeneralCheckHighscoreScreen()
{
	//add Stat
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KGeneralCheckHighScoreScreen);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::UpdatesHighscores()
{
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KUpdatesHighscores);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

void CUserStats::MissionHighscoreScreen(TInt aMissionIndex)
{
	SUserStatsEntry* lUserEntry = CGame::Game->iUserStatsTrackerDaemon->CreateUserStatsEntry(KMissionHighscoreScreen);
	CGame::Game->iUserStatsTrackerDaemon->AddValueToUserStatsEntry(lUserEntry, KMissionHighscoreScreenMissionIndex, aMissionIndex);
	CGame::Game->iUserStatsTrackerDaemon->AddUserStatsEntry(lUserEntry);
}

