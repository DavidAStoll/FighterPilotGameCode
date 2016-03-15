/*
 * CUserStats.h
 *
 *  Created on: Oct 20, 2012
 *      Author: dstoll
 */

#ifndef CUSERSTATS_H_
#define CUSERSTATS_H_

#include "includes/core/utility/CUserStatsTrackerDaemon.h"

class CUserStats
{

public:

	static void GameStarted();

	static TUint64 GameStartTime; //keeps track when the game starts
	static TUint64 MissionStartTime; //keeps track when the game starts
	static TUint64 TutorialSectionStartTime;
	static TInt MissionNumberOfRestarts;
	static TInt MissionNumberOfSaves;
	static TInt MissionNumberOfLoads;
	static TInt MissionIndex;
	static TInt MissionCampaignIndex;
	static TInt MissionDifficulty;
	static TBool MissionIsLoading;

	static void GameStartedTime();
	static void GameFinishedTime();
	//to record what phone the user uses most often
	static void RecordPhoneModel();
	//frontend menus, might be a good idea so we know what a users clicks on during his lifetime
	static void MainMenu();
	static void CreditsMenuOpenedFrontend();
	static void SelectCampaignMenuOpenedFrontend();
	static void ContinueFromLastSaveMenuOpenedFrontend();
	static void WatchOnlineGameVideosMenuOpenedFrontend();
	static void BuyGameMenu();
	static void SelectLanguageMenuOpenedFrontend();
	static void GameSettingsMenuOpenedFrontend();
	static void GameSettingsMenuOpenedInGame();
	static void ChangeSongMenuOpenedFrontend();
	static void ChangeSongMenuOpenedInGame();
	//keeps track what language the user selected
	static void LanguageSelected(const char* aLanguage);
	//keeps track what song the user played
	static void ChangeSong(TBool aFrontend, const char* aSongName);
	//used to find out what volume the music and SFX were set and if icon buttons are used or not
	static void SoundEffectsVolumeChanged(TBool aFrontend, TInt aValue);
	static void MusicVolumeChanged(TBool aFrontend, TInt aValue);
	static void UseIconButtons(TBool aUseIcons);
	//what campaign the user selected
	static void SelectedCampaign(TInt aCampaignIndex);
	//check if player is using a silent profile
	static void UsesSilentProfile();
	//find out what mission the user plays
	static void PlaysMission(TInt aCampaignIndex, TInt aMissionIndex, TInt aDifficulty, TBool aLoading);
	//find out what campaign briefing screen was opened
	static void CampaignBriefingScreenOpenend(TInt aCampaignIndex, TBool aDebriefingScreen);
	//find out how mission was completed
	static void EndMissionInit(TInt aCampaignIndex, TInt aMissionIndex, TInt aDifficulty, TBool aIsLoading);
	static void EndMission(TInt aLivesLeft, TInt aCompletedSuccessfully, TInt aScore);
	static void EndMisssionIncreaseNumberOfRestarts();
	static void EndMisssionIncreaseNumberOfLoads();
	static void EndMisssionIncreaseNumberOfSaves();
	//to find out more about tutorials
	static void TutorialSectionInit();
	static void TutorialSection(const char* aTutorialSectionName);
	static void TutorialSectionSkipped(TInt aSectionIndex);
	//for tracking highscore activity
	static void GeneralCheckHighscoreScreen();
	static void UpdatesHighscores();
	static void MissionHighscoreScreen(TInt aMissionIndex);
};

#endif /* CUSERSTATS_H_ */
