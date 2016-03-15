/*
 * CSaveGameObject.h
 *
 *  Created on: Jul 25, 2012
 *      Author: dstoll
 */

#ifndef CSaveGameObject_H_
#define CSaveGameObject_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CImageStore.h"
#include "includes/core/utility/CFileStream.h"
#include "includes/game/common/CGameVariables.h"

#define SAVE_GAME_OBJECT_VERSION_NUMBER_1_0_0 100
#define SAVE_GAME_OBJECT_CURRENT_VERSION_NUMBER SAVE_GAME_OBJECT_VERSION_NUMBER_1_0_0

struct SMissionHighscoreTableEntry
{
	CString* PlayerName;
	CString* Score;
	CString* Date;
	CString* Time;
};

struct SMissionHighscoreTable
{
	SMissionHighscoreTableEntry Rank1;
	SMissionHighscoreTableEntry Rank2;
	SMissionHighscoreTableEntry Rank3;
	SMissionHighscoreTableEntry Rank4;
	SMissionHighscoreTableEntry Rank5;
	SMissionHighscoreTableEntry Rank6;
	SMissionHighscoreTableEntry Rank7;
	SMissionHighscoreTableEntry Rank8;
	SMissionHighscoreTableEntry Rank9;
	SMissionHighscoreTableEntry Rank10;
};

struct SBriefingScreen
{
	TBool Unlocked;
	TBool Enabled; //if not enabled, it will not be displayed
	TBool IsDebriefingScreen;
	CString* BriefingScreenNameId;
	TImageId SmallMapImage;
	TImageId BigMapImage;
	CString* BriefingDescriptionId;

};

//has all the data that we need to keep track off for a particular mission
struct SMission
{
	TBool Enabled; //if not enabled, it will not be displayed
	TGameMap MapId;
	TImageId MapImageId;
	TImageId MissionImage;
	CString* MissionNameId;
	CString* MissionDescriptionId;
	TBool Unlocked; //if locked the mission can't be started
	TBool CompletedBaby;
	TBool CompletedEasy;
	TBool CompletedMedium;
	TBool CompletedHard;
	TInt Score; //keeps tracks of the heightest score that this level was completed with
	CString* Time;
	CString* SaveGameFile; //should be unique string for each mission
	CString* MusicFile; //should be unique string for each mission
	//needed for highscores
	CString* MissionNameForHighscoreTable; //used to identify what mission to update with scores send from Server
	SMissionHighscoreTable HighscoreTable;
};

struct SCampaign
{
	TBool Unlocked; //if locked the campaign can't be started
	TBool TutorialCampaign;
	CString* CampaignNameId;
	TImageId CampaignImage;
	//list of potential missions and Briefing screens that this campaign could include
	SBriefingScreen CampaignBriefingScreen;
	SMission Mission1;
	SMission Mission2;
	SMission Mission3;
	SMission Mission4;
	SBriefingScreen CampaignDebriefingScreen;
};


class CSaveGameObject
{

public:
	static CSaveGameObject* New();
	virtual ~CSaveGameObject();

	//Unlocks the next campaign that follows
	void UnlockNextCampaign(SCampaign* aCurrentCampaign);
	//Unlocks the next mission that follows after the one passed as the argument
	void UnlockNextMission(TGameMap aMapId);

	void LoadFromDisk();
	void SaveToDisk();

	CPointerArray<SCampaign>* iCampaigns;
	SMission iBestOfAllMission; //needed to keep track who is best Overall in the Game

protected:
	CSaveGameObject();
	void Construct();
	//needed to initilize highscore table for mission with function since it contains a lot of new operations
	void InitializeMissionHighscoreTable(SMissionHighscoreTable& aTable);
	void InitializeMissionHighscoreTableEntry(SMissionHighscoreTableEntry& aEntry);
	//since Highscore tables has a lot of entries, I am going to write specific function it to keep it clean
	void WriteMissionHighscoreTableToDisk(CFileWriteStream* aWriteStream, SMissionHighscoreTable& aTable);
	void WriteMissionHighscoreTableEntryToDisk(CFileWriteStream* aWriteStream, SMissionHighscoreTableEntry& aTableEntry);
	void ReadMissionHighscoreTableFromDisk(CFileReadStream* aReadStream, SMissionHighscoreTable& aTable);
	void ReadMissionHighscoreTableEntryFromDisk(CFileReadStream* aReadStream, SMissionHighscoreTableEntry& aTableEntry);
};

#endif /* CSaveGameObject_H_ */
