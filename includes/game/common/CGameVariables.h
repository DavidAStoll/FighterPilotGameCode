/*
 * CGameVariables.h
 *
 *  Created on: Jul 26, 2012
 *      Author: dstoll
 */

#ifndef CGAMEVARIABLES_H_
#define CGAMEVARIABLES_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CLocalizationEngine.h"
#include "includes/GlobalDefines.h"

#define GAME_VARIABLES_SETTINGS_FILE "GameSettings.ini"
#define GAME_VARIABLES_SETTINGS_VERSION 102 //should change everytime we add or remove new variables to avoid that an old data file is read
#define SONG_BATTLE_HYMN "Battle Hymn.mp3"
#define SONG_BIG_HITS "Big Hits.mp3"
#define SONG_DARK_MARCH "Dark March.mp3"
#define SONG_DEATH_FROM_ABOVE "Death From Above.mp3"
#define SONG_DISTO_MARCH "Disto March.mp3"
#define SONG_MENU_LOOP "Menu Loop.mp3"

enum TGameMap
{
   EGameMapTutorialMission1,EGameMapTutorialMission2,EGameMapTutorialMission3,
   EGameMapCampaign1Mission1,EGameMapCampaign1Mission2,EGameMapCampaign1Mission3,EGameMapCampaign1Mission4,
   EGameMapCampaign2Mission1,EGameMapCampaign2Mission2,EGameMapCampaign2Mission3,EGameMapCampaign2Mission4,
   EGameMapCampaign3Mission1,EGameMapCampaign3Mission2,EGameMapCampaign3Mission3,EGameMapCampaign3Mission4,
   EGameMapCampaign4Mission1,EGameMapCampaign4Mission2,EGameMapCampaign4Mission3,EGameMapCampaign4Mission4,
   EGameMapNone
};

enum TGameDifficulty
{
	EGameDifficultyBaby = 0,EGameDifficultyEasy,EGameDifficultyMedium,EGameDifficultyHard,EGameDifficultyNotDefined
};

//Sound
#define GDEFAULT_MUSIC_VOLUME 20
#define GDEFAULT_SOUND_EFFECT_VOLUME 80
//Other
#define GMAP_SAFETY_MARGIN 400 //400 X on the left and right ends of the map
#define GMAX_NUMBER_OF_POINTS 9999 //related hud, can't display more than that
#define GSECONDS_WHEN_INACTITY_TIMER_IS_RESET (GFRAMES_PER_SECOND_BACKEND * 5)
#define GMAX_HEIGHT_IN_GAME_LEVEL 2200
#define GMAX_HEIGHT_IN_GAME_LEVEL_FOR_PLANE 2000////leave a bit of a safety margin since plane might be rotated when it flies up
#define GPLAYER_WAIT_FRAMES_UNTIL_RESPAWN_POSSIBLE (GFRAMES_PER_SECOND_BACKEND * 3) //3 seconds the player needs to wait after the aircraft died
#define GMAX_WORK_LOAD_LIMIT_UNTIL_RECYCLING_STARTS 95 //if load above this dead moveable objects will get canned
#define GSECONDS_UNTIL_DEAD_MOVEABLE_GAME_OBJECT_GETS_RECYCLED (5) //5 secs for fadeout
#define GFRAMES_UNTIL_NEXT_RECYCLE_CIRCLE ((GSECONDS_UNTIL_DEAD_MOVEABLE_GAME_OBJECT_GETS_RECYCLED + 2) * GFRAMES_PER_SECOND_BACKEND) //+2 secs for CPU to recover
#define GNUMBER_OF_MOVEABLE_OBJECTS_GET_RECYCLED_AT_A_TIME 5
//Game Controls
#define GFRAMES_UNTIL_FIRST_REPEAT_INPUT_HAPPENS 6
#define GFRAMES_UNTIL_REPEAT_INPUT_HAPPENS 3
//For Highscore table
#define GMAX_CHARACTERS_FOR_PLAYER_NAME 20


class CGameVariables
{
public:

	virtual ~CGameVariables();
	static CGameVariables* New();

	//Load Previous Settings From Disk
	void LoadSettingsFromDisk();
	//Saves Settings to Disk
	void SaveSettingsToDisk();

	TInt iMusicPercentage;
	TInt iSoundEffectsPercentage;
	TBool iUseIconButton;
	TBool iAgreementAccepted;
	TBool iFirstTimeStartingUp;
	TLanguage iLanguage;
	TGameDifficulty iDifficulty;
	CString* iStringPlayerName;
	TGameMap iLastSaveGame;
	TBool iDisplayBackgroundImageInPauseMenu;
	TBool iFirstTimeGettingHighscore;

	//temp memory
	CString* iFrontendSong;
	TBool iIsUserSong;

protected:

	CGameVariables();
	void Construct();
};

#endif /* CGAMEVARIABLES_H_ */
