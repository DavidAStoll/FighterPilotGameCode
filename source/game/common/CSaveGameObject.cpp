/*
 * CSaveGameObject.cpp
 *
 *  Created on: Jul 25, 2012
 *      Author: dstoll
 */

#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/core/utility/CFileStream.h"


#define SAVE_GAME_INI_FILE "GameProgress.ini"
#define SAVE_GAME_INI_FILE_VERSION 102

//mission names
#define TUTORIAL1_NAME "TutorialMission1"
#define TUTORIAL2_NAME "TutorialMission2"
#define TUTORIAL3_NAME "TutorialMission3"
//campaign 1
#define CAMPAIGN1_MISSION1_NAME "Campaign1Mission1"
#define CAMPAIGN1_MISSION2_NAME "Campaign1Mission2"
#define CAMPAIGN1_MISSION3_NAME "Campaign1Mission3"
#define CAMPAIGN1_MISSION4_NAME "Campaign1Mission4"
//campaign 2
#define CAMPAIGN2_MISSION1_NAME "Campaign2Mission1"
#define CAMPAIGN2_MISSION2_NAME "Campaign2Mission2"
#define CAMPAIGN2_MISSION3_NAME "Campaign2Mission3"
#define CAMPAIGN2_MISSION4_NAME "Campaign2Mission4"
//campaign 3
#define CAMPAIGN3_MISSION1_NAME "Campaign3Mission1"
#define CAMPAIGN3_MISSION2_NAME "Campaign3Mission2"
#define CAMPAIGN3_MISSION3_NAME "Campaign3Mission3"
#define CAMPAIGN3_MISSION4_NAME "Campaign3Mission4"
//campaign 4
#define CAMPAIGN4_MISSION1_NAME "Campaign4Mission1"
#define CAMPAIGN4_MISSION2_NAME "Campaign4Mission2"
#define CAMPAIGN4_MISSION3_NAME "Campaign4Mission3"
#define CAMPAIGN4_MISSION4_NAME "Campaign4Mission4"
//BestOfAll
#define BEST_OF_ALL_NAME "BestOfAll"



CSaveGameObject::CSaveGameObject()
{
	// TODO Auto-generated constructor stub

}

CSaveGameObject::~CSaveGameObject()
{
	// TODO Auto-generated destructor stub
}

CSaveGameObject* CSaveGameObject::New()
{
	CSaveGameObject* lSelf = new CSaveGameObject();
	lSelf->Construct();
	return lSelf;
}

void CSaveGameObject::Construct()
{
	iCampaigns = CPointerArray<SCampaign>::New();

	SMission lTemplateMission;
	lTemplateMission.Enabled = true;
	lTemplateMission.CompletedBaby = false;
	lTemplateMission.CompletedEasy = false;
	lTemplateMission.CompletedMedium = false;
	lTemplateMission.CompletedHard = false;
	lTemplateMission.Unlocked = false;
	lTemplateMission.Score = 0;

	SCampaign lCampaign;
	//------------------- Tutorial Campaign -----------------//
	lCampaign.Unlocked = true;
	lCampaign.TutorialCampaign = true;
	lCampaign.CampaignNameId = new CString("Frontend.CampaignNamesAndLevels.TutorialName");
	lCampaign.CampaignImage = EImageIdFrontendChooseCampaignImageSingleDauntless;
	//Briefing Screen
	lCampaign.CampaignBriefingScreen.Unlocked = true;
	lCampaign.CampaignBriefingScreen.Enabled = true;
	lCampaign.CampaignBriefingScreen.IsDebriefingScreen = false;
	lCampaign.CampaignBriefingScreen.BigMapImage = EImageIdFrontendCampaignBriefingScreenMap1941;
	lCampaign.CampaignBriefingScreen.SmallMapImage =  EImageIdFrontendCampaignBriefingScreenMap1941Small;
	lCampaign.CampaignBriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.TutorialName");
	lCampaign.CampaignBriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.TutorialBriefing");
	//Missions
	lCampaign.Mission1 = lTemplateMission;
	lCampaign.Mission2 = lTemplateMission;
	lCampaign.Mission3 = lTemplateMission;
	lCampaign.Mission4 = lTemplateMission;
	lCampaign.Mission1.Time = new CString("??:??:??");
	lCampaign.Mission2.Time = new CString("??:??:??");
	lCampaign.Mission3.Time = new CString("??:??:??");
	lCampaign.Mission4.Time = new CString("??:??:??");
	InitializeMissionHighscoreTable(lCampaign.Mission1.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission2.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission3.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission4.HighscoreTable);
	lCampaign.Mission1.Unlocked = true;
	lCampaign.Mission2.Unlocked = true;
	lCampaign.Mission3.Unlocked = true;
	lCampaign.Mission1.MapId = EGameMapTutorialMission1;
	lCampaign.Mission2.MapId = EGameMapTutorialMission2;
	lCampaign.Mission3.MapId = EGameMapTutorialMission3;
	lCampaign.Mission1.MissionImage = EImageIdFrontendMissionImageDauntlessOnAirport;
	lCampaign.Mission2.MissionImage = EImageIdFrontendMissionImageDauntlessDropsBomb;
	lCampaign.Mission3.MissionImage = EImageIdFrontendMissionImageDauntlessFlyInFormation;
	lCampaign.Mission1.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.TutorialLevel1");
	lCampaign.Mission2.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.TutorialLevel2");
	lCampaign.Mission3.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.TutorialLevel3");
	lCampaign.Mission1.MapImageId = EImageIdMapTutorial1;
	lCampaign.Mission2.MapImageId = EImageIdMapTutorial2;
	lCampaign.Mission3.MapImageId = EImageIdMapTutorial3;
	lCampaign.Mission1.MissionDescriptionId = new CString("Frontend.Tutorials.MissionBriefingTutorial1");
	lCampaign.Mission2.MissionDescriptionId = new CString("Frontend.Tutorials.MissionBriefingTutorial2");
	lCampaign.Mission3.MissionDescriptionId = new CString("Frontend.Tutorials.MissionBriefingTutorial3");
	lCampaign.Mission1.SaveGameFile = new CString(TUTORIAL1_NAME);
	lCampaign.Mission2.SaveGameFile = new CString(TUTORIAL2_NAME);
	lCampaign.Mission3.SaveGameFile = new CString(TUTORIAL3_NAME);
	lCampaign.Mission1.MissionNameForHighscoreTable = new CString(TUTORIAL1_NAME);
	lCampaign.Mission2.MissionNameForHighscoreTable = new CString(TUTORIAL2_NAME);
	lCampaign.Mission3.MissionNameForHighscoreTable = new CString(TUTORIAL3_NAME);
	lCampaign.Mission1.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission2.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission3.MusicFile = new CString(SONG_DARK_MARCH);
	//there is no mission 4
	lCampaign.Mission4.Enabled = false;
	//Debriefing
	lCampaign.CampaignDebriefingScreen.Enabled = false;
	//Add to List
	iCampaigns->Append(new SCampaign(lCampaign));

	//------------------- Campaign 1 -----------------//
	lCampaign.Unlocked = true;
	lCampaign.TutorialCampaign = false;
	lCampaign.CampaignNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Name");
	lCampaign.CampaignImage = EImageIdFrontendChooseCampaignImageDauntlessTakeOfFromCarrier;
	//Briefing Screen
	lCampaign.CampaignBriefingScreen.Unlocked = true;
	lCampaign.CampaignBriefingScreen.Enabled = true;
	lCampaign.CampaignBriefingScreen.IsDebriefingScreen = false;
	lCampaign.CampaignBriefingScreen.BigMapImage = EImageIdFrontendCampaignBriefingScreenMap1942;
	lCampaign.CampaignBriefingScreen.SmallMapImage =  EImageIdFrontendCampaignBriefingScreenMap1942Small;
	lCampaign.CampaignBriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Intro");
	lCampaign.CampaignBriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign1Briefing");
	//Missions
	lCampaign.Mission1 = lTemplateMission;
	lCampaign.Mission2 = lTemplateMission;
	lCampaign.Mission3 = lTemplateMission;
	lCampaign.Mission4 = lTemplateMission;
	lCampaign.Mission1.Time = new CString("??:??:??");
	lCampaign.Mission2.Time = new CString("??:??:??");
	lCampaign.Mission3.Time = new CString("??:??:??");
	lCampaign.Mission4.Time = new CString("??:??:??");
	InitializeMissionHighscoreTable(lCampaign.Mission1.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission2.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission3.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission4.HighscoreTable);
	lCampaign.Mission1.Unlocked = true;
	lCampaign.Mission2.Unlocked = true;
	lCampaign.Mission3.Unlocked = true;
	lCampaign.Mission4.Unlocked = false;
	lCampaign.Mission1.MapId = EGameMapCampaign1Mission3;
	lCampaign.Mission2.MapId = EGameMapCampaign3Mission2;
	lCampaign.Mission3.MapId = EGameMapCampaign4Mission2;
	lCampaign.Mission4.MapId = EGameMapCampaign1Mission4;
	lCampaign.Mission1.MissionImage = EImageIdFrontendMissionImageOkinawaBeach;
	lCampaign.Mission2.MissionImage = EImageIdFrontendMissionImageShootDownBurningJapanesePlane;
	lCampaign.Mission3.MissionImage = EImageIdFrontendMissionImageIwoJimaFlagRising;
	lCampaign.Mission4.MissionImage = EImageIdFrontendMissionImageOkinawaCaveDemolition;
	lCampaign.Mission1.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Level3");
	lCampaign.Mission2.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Level2");
	lCampaign.Mission3.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Level2");
	lCampaign.Mission4.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Level4");
	lCampaign.Mission1.MapImageId = EImageIdMapCampaign1Mission3;
	lCampaign.Mission2.MapImageId = EImageIdMapCampaign3Mission2;
	lCampaign.Mission3.MapImageId = EImageIdMapCampaign4Mission2;
	lCampaign.Mission4.MapImageId = EImageIdMapCampaign1Mission4;
	lCampaign.Mission1.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign1Mission3");
	lCampaign.Mission2.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign3Mission2");
	lCampaign.Mission3.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign4Mission2");
	lCampaign.Mission4.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign1Mission4");
	lCampaign.Mission1.SaveGameFile = new CString(CAMPAIGN1_MISSION3_NAME);
	lCampaign.Mission2.SaveGameFile = new CString(CAMPAIGN3_MISSION2_NAME);
	lCampaign.Mission3.SaveGameFile = new CString(CAMPAIGN4_MISSION2_NAME);
	lCampaign.Mission4.SaveGameFile = new CString(CAMPAIGN1_MISSION4_NAME);
	lCampaign.Mission1.MissionNameForHighscoreTable = new CString(CAMPAIGN1_MISSION3_NAME);
	lCampaign.Mission2.MissionNameForHighscoreTable = new CString(CAMPAIGN3_MISSION2_NAME);
	lCampaign.Mission3.MissionNameForHighscoreTable = new CString(CAMPAIGN4_MISSION2_NAME);
	lCampaign.Mission4.MissionNameForHighscoreTable = new CString(CAMPAIGN1_MISSION4_NAME);
	lCampaign.Mission1.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission2.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission3.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission4.MusicFile = new CString(SONG_DARK_MARCH);
	//Debriefing
	lCampaign.CampaignDebriefingScreen.Unlocked = false;
	lCampaign.CampaignDebriefingScreen.Enabled = true;
	lCampaign.CampaignDebriefingScreen.IsDebriefingScreen = true;
	lCampaign.CampaignDebriefingScreen.BigMapImage = EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlag;
	lCampaign.CampaignDebriefingScreen.SmallMapImage =  EImageIdFrontendCampaignDebriefingImageAmericanDisplayFlagSmall;
	lCampaign.CampaignDebriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Conclusion");
	lCampaign.CampaignDebriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign1Debriefing");
	//Add to List
	iCampaigns->Append(new SCampaign(lCampaign));

	//------------------- Campaign 2 -----------------//
	lCampaign.Unlocked = false;
	lCampaign.TutorialCampaign = false;
	lCampaign.CampaignNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Name");
	lCampaign.CampaignImage = EImageIdFrontendChooseCampaignImageMitchellAttackShip;
	//Briefing Screen
	lCampaign.CampaignBriefingScreen.Unlocked = false;
	lCampaign.CampaignBriefingScreen.Enabled = true;
	lCampaign.CampaignBriefingScreen.IsDebriefingScreen = false;
	lCampaign.CampaignBriefingScreen.BigMapImage = EImageIdFrontendCampaignBriefingScreenMap1943;
	lCampaign.CampaignBriefingScreen.SmallMapImage =  EImageIdFrontendCampaignBriefingScreenMap1943Small;
	lCampaign.CampaignBriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Intro");
	lCampaign.CampaignBriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign2Briefing");
	//Missions
	lCampaign.Mission1 = lTemplateMission;
	lCampaign.Mission2 = lTemplateMission;
	lCampaign.Mission3 = lTemplateMission;
	lCampaign.Mission4 = lTemplateMission;
	lCampaign.Mission1.Time = new CString("??:??:??");
	lCampaign.Mission2.Time = new CString("??:??:??");
	lCampaign.Mission3.Time = new CString("??:??:??");
	lCampaign.Mission4.Time = new CString("??:??:??");
	InitializeMissionHighscoreTable(lCampaign.Mission1.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission2.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission3.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission4.HighscoreTable);
	lCampaign.Mission1.Unlocked = false;
	lCampaign.Mission2.Unlocked = false;
	lCampaign.Mission3.Unlocked = false;
	lCampaign.Mission4.Unlocked = false;
	lCampaign.Mission1.MapId = EGameMapCampaign2Mission1;
	lCampaign.Mission2.MapId = EGameMapCampaign2Mission2;
	lCampaign.Mission3.MapId = EGameMapCampaign1Mission1;
	lCampaign.Mission4.MapId = EGameMapCampaign2Mission4;
	lCampaign.Mission1.MissionImage = EImageIdFrontendMissionImageTransporterAttack;
	lCampaign.Mission2.MissionImage = EImageIdFrontendMissionImageWaterBuffaloLoaded;
	lCampaign.Mission3.MissionImage = EImageIdFrontendMissionImageTarawaOnHill;
	lCampaign.Mission4.MissionImage = EImageIdFrontendMissionImageJapaneseBattleshipAttacked;
	lCampaign.Mission1.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Level1");
	lCampaign.Mission2.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Level2");
	lCampaign.Mission3.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign1Level1");
	lCampaign.Mission4.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Level4");
	lCampaign.Mission1.MapImageId = EImageIdMapCampaign2Mission1;
	lCampaign.Mission2.MapImageId = EImageIdMapCampaign2Mission2;
	lCampaign.Mission3.MapImageId = EImageIdMapCampaign1Mission1;
	lCampaign.Mission4.MapImageId = EImageIdMapCampaign2Mission4;
	lCampaign.Mission1.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign2Mission1");
	lCampaign.Mission2.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign2Mission2");
	lCampaign.Mission3.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign1Mission1");
	lCampaign.Mission4.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign2Mission4");
	lCampaign.Mission1.SaveGameFile = new CString(CAMPAIGN2_MISSION1_NAME);
	lCampaign.Mission2.SaveGameFile = new CString(CAMPAIGN2_MISSION2_NAME);
	lCampaign.Mission3.SaveGameFile = new CString(CAMPAIGN1_MISSION1_NAME);
	lCampaign.Mission4.SaveGameFile = new CString(CAMPAIGN2_MISSION4_NAME);
	lCampaign.Mission1.MissionNameForHighscoreTable = new CString(CAMPAIGN2_MISSION1_NAME);
	lCampaign.Mission2.MissionNameForHighscoreTable = new CString(CAMPAIGN2_MISSION2_NAME);
	lCampaign.Mission3.MissionNameForHighscoreTable = new CString(CAMPAIGN1_MISSION1_NAME);
	lCampaign.Mission4.MissionNameForHighscoreTable = new CString(CAMPAIGN2_MISSION4_NAME);
	lCampaign.Mission1.MusicFile = new CString(SONG_DISTO_MARCH);
	lCampaign.Mission2.MusicFile = new CString(SONG_BIG_HITS);
	lCampaign.Mission3.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission4.MusicFile = new CString(SONG_DEATH_FROM_ABOVE);
	//Debriefing
	lCampaign.CampaignDebriefingScreen.Unlocked = false;
	lCampaign.CampaignDebriefingScreen.Enabled = true;
	lCampaign.CampaignDebriefingScreen.IsDebriefingScreen = true;
	lCampaign.CampaignDebriefingScreen.BigMapImage = EImageIdFrontendCampaignDebriefingImageGilbertArtillery;
	lCampaign.CampaignDebriefingScreen.SmallMapImage =  EImageIdFrontendCampaignDebriefingImageGilbertArtillerySmall;
	lCampaign.CampaignDebriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign2Conclusion");
	lCampaign.CampaignDebriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign2Debriefing");
	//Add to List
	iCampaigns->Append(new SCampaign(lCampaign));

	//------------------- Campaign 3 -----------------//
	lCampaign.Unlocked = false;
	lCampaign.TutorialCampaign = false;
	lCampaign.CampaignNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Name");
	lCampaign.CampaignImage = EImageIdFrontendChooseCampaignImageIwoJimaLanding;
	//Briefing Screen
	lCampaign.CampaignBriefingScreen.Unlocked = false;
	lCampaign.CampaignBriefingScreen.Enabled = true;
	lCampaign.CampaignBriefingScreen.IsDebriefingScreen = false;
	lCampaign.CampaignBriefingScreen.BigMapImage = EImageIdFrontendCampaignBriefingScreenMap1944;
	lCampaign.CampaignBriefingScreen.SmallMapImage =  EImageIdFrontendCampaignBriefingScreenMap1944Small;
	lCampaign.CampaignBriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Intro");
	lCampaign.CampaignBriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign3Briefing");
	//Missions
	lCampaign.Mission1 = lTemplateMission;
	lCampaign.Mission2 = lTemplateMission;
	lCampaign.Mission3 = lTemplateMission;
	lCampaign.Mission4 = lTemplateMission;
	lCampaign.Mission1.Time = new CString("??:??:??");
	lCampaign.Mission2.Time = new CString("??:??:??");
	lCampaign.Mission3.Time = new CString("??:??:??");
	lCampaign.Mission4.Time = new CString("??:??:??");
	InitializeMissionHighscoreTable(lCampaign.Mission1.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission2.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission3.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission4.HighscoreTable);
	lCampaign.Mission1.Unlocked = false;
	lCampaign.Mission2.Unlocked = false;
	lCampaign.Mission3.Unlocked = false;
	lCampaign.Mission4.Unlocked = false;
	lCampaign.Mission1.MapId = EGameMapCampaign3Mission1;
	lCampaign.Mission2.MapId = EGameMapCampaign3Mission2;
	lCampaign.Mission3.MapId = EGameMapCampaign3Mission3;
	lCampaign.Mission4.MapId = EGameMapCampaign3Mission4;
	lCampaign.Mission1.MissionImage = EImageIdFrontendMissionImageAmericansLandeAtLosNegros;
	lCampaign.Mission2.MissionImage = EImageIdFrontendMissionImageShootDownBurningJapanesePlane;
	lCampaign.Mission3.MissionImage = EImageIdFrontendMissionImageBunkerHitTwoKamikazes;
	lCampaign.Mission4.MissionImage = EImageIdFrontendMissionImageShermanTankFlamethrower;
	lCampaign.Mission1.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Level1");
	lCampaign.Mission2.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Level2");
	lCampaign.Mission3.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Level3");
	lCampaign.Mission4.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Level4");
	lCampaign.Mission1.MapImageId = EImageIdMapCampaign3Mission1;
	lCampaign.Mission2.MapImageId = EImageIdMapCampaign3Mission2;
	lCampaign.Mission3.MapImageId = EImageIdMapCampaign3Mission3;
	lCampaign.Mission4.MapImageId = EImageIdMapCampaign3Mission4;
	lCampaign.Mission1.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign3Mission1");
	lCampaign.Mission2.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign3Mission2");
	lCampaign.Mission3.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign3Mission3");
	lCampaign.Mission4.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign3Mission4");
	lCampaign.Mission1.SaveGameFile = new CString(CAMPAIGN3_MISSION1_NAME);
	lCampaign.Mission2.SaveGameFile = new CString(CAMPAIGN3_MISSION2_NAME);
	lCampaign.Mission3.SaveGameFile = new CString(CAMPAIGN3_MISSION3_NAME);
	lCampaign.Mission4.SaveGameFile = new CString(CAMPAIGN3_MISSION4_NAME);
	lCampaign.Mission1.MissionNameForHighscoreTable = new CString(CAMPAIGN3_MISSION1_NAME);
	lCampaign.Mission2.MissionNameForHighscoreTable = new CString(CAMPAIGN3_MISSION2_NAME);
	lCampaign.Mission3.MissionNameForHighscoreTable = new CString(CAMPAIGN3_MISSION3_NAME);
	lCampaign.Mission4.MissionNameForHighscoreTable = new CString(CAMPAIGN3_MISSION4_NAME);
	lCampaign.Mission1.MusicFile = new CString(SONG_DISTO_MARCH);
	lCampaign.Mission2.MusicFile = new CString(SONG_DEATH_FROM_ABOVE);
	lCampaign.Mission3.MusicFile = new CString(SONG_BIG_HITS);
	lCampaign.Mission4.MusicFile = new CString(SONG_DARK_MARCH);
	//Debriefing
	lCampaign.CampaignDebriefingScreen.Unlocked = false;
	lCampaign.CampaignDebriefingScreen.Enabled = true;
	lCampaign.CampaignDebriefingScreen.IsDebriefingScreen = true;
	lCampaign.CampaignDebriefingScreen.BigMapImage = EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikaze;
	lCampaign.CampaignDebriefingScreen.SmallMapImage =  EImageIdFrontendCampaignDebriefingImageHighscoolWaveKamikazeSmall;
	lCampaign.CampaignDebriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign3Conclusion");
	lCampaign.CampaignDebriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign3Debriefing");
	//Add to List
	iCampaigns->Append(new SCampaign(lCampaign));

	//------------------- Campaign 4 -----------------//
	lCampaign.Unlocked = false;
	lCampaign.TutorialCampaign = false;
	lCampaign.CampaignNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Name");
	lCampaign.CampaignImage = EImageIdFrontendChooseCampaignImageJapaneseSurrender;
	//Briefing Screen
	lCampaign.CampaignBriefingScreen.Unlocked = false;
	lCampaign.CampaignBriefingScreen.Enabled = true;
	lCampaign.CampaignBriefingScreen.IsDebriefingScreen = false;
	lCampaign.CampaignBriefingScreen.BigMapImage = EImageIdFrontendCampaignBriefingScreenMap1945;
	lCampaign.CampaignBriefingScreen.SmallMapImage =  EImageIdFrontendCampaignBriefingScreenMap1945Small;
	lCampaign.CampaignBriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Intro");
	lCampaign.CampaignBriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign4Briefing");
	//Missions
	lCampaign.Mission1 = lTemplateMission;
	lCampaign.Mission2 = lTemplateMission;
	lCampaign.Mission3 = lTemplateMission;
	lCampaign.Mission4 = lTemplateMission;
	lCampaign.Mission1.Time = new CString("??:??:??");
	lCampaign.Mission2.Time = new CString("??:??:??");
	lCampaign.Mission3.Time = new CString("??:??:??");
	lCampaign.Mission4.Time = new CString("??:??:??");
	InitializeMissionHighscoreTable(lCampaign.Mission1.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission2.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission3.HighscoreTable);
	InitializeMissionHighscoreTable(lCampaign.Mission4.HighscoreTable);
	lCampaign.Mission1.Unlocked = false;
	lCampaign.Mission2.Unlocked = false;
	lCampaign.Mission3.Unlocked = false;
	lCampaign.Mission4.Unlocked = false;
	lCampaign.Mission1.MapId = EGameMapCampaign4Mission1;
	lCampaign.Mission2.MapId = EGameMapCampaign4Mission2;
	lCampaign.Mission3.MapId = EGameMapCampaign4Mission3;
	lCampaign.Mission4.MapId = EGameMapCampaign4Mission4;
	lCampaign.Mission1.MissionImage = EImageIdFrontendMissionImageOkinawaBeach;
	lCampaign.Mission2.MissionImage = EImageIdFrontendMissionImageIwoJimaFlagRising;
	lCampaign.Mission3.MissionImage = EImageIdFrontendMissionImageBattleMarianas;
	lCampaign.Mission4.MissionImage = EImageIdFrontendMissionImageYamato;
	lCampaign.Mission1.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Level1");
	lCampaign.Mission2.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Level2");
	lCampaign.Mission3.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Level3");
	lCampaign.Mission4.MissionNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Level4");
	lCampaign.Mission1.MapImageId = EImageIdMapCampaign4Mission1;
	lCampaign.Mission2.MapImageId = EImageIdMapCampaign4Mission2;
	lCampaign.Mission3.MapImageId = EImageIdMapCampaign4Mission3;
	lCampaign.Mission4.MapImageId = EImageIdMapCampaign4Mission4;
	lCampaign.Mission1.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign4Mission1");
	lCampaign.Mission2.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign4Mission2");
	lCampaign.Mission3.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign4Mission3");
	lCampaign.Mission4.MissionDescriptionId = new CString("Frontend.MissionDescriptions.Campaign4Mission4");
	lCampaign.Mission1.SaveGameFile = new CString(CAMPAIGN4_MISSION1_NAME);
	lCampaign.Mission2.SaveGameFile = new CString(CAMPAIGN4_MISSION2_NAME);
	lCampaign.Mission3.SaveGameFile = new CString(CAMPAIGN4_MISSION3_NAME);
	lCampaign.Mission4.SaveGameFile = new CString(CAMPAIGN4_MISSION4_NAME);
	lCampaign.Mission1.MissionNameForHighscoreTable = new CString(CAMPAIGN4_MISSION1_NAME);
	lCampaign.Mission2.MissionNameForHighscoreTable = new CString(CAMPAIGN4_MISSION2_NAME);
	lCampaign.Mission3.MissionNameForHighscoreTable = new CString(CAMPAIGN4_MISSION3_NAME);
	lCampaign.Mission4.MissionNameForHighscoreTable = new CString(CAMPAIGN4_MISSION4_NAME);
	lCampaign.Mission1.MusicFile = new CString(SONG_DARK_MARCH);
	lCampaign.Mission2.MusicFile = new CString(SONG_BIG_HITS);
	lCampaign.Mission3.MusicFile = new CString(SONG_DISTO_MARCH);
	lCampaign.Mission4.MusicFile = new CString(SONG_DEATH_FROM_ABOVE);
	//Debriefing
	lCampaign.CampaignDebriefingScreen.Unlocked = false;
	lCampaign.CampaignDebriefingScreen.Enabled = true;
	lCampaign.CampaignDebriefingScreen.IsDebriefingScreen = true;
	lCampaign.CampaignDebriefingScreen.BigMapImage = EImageIdFrontendCampaignDebriefingImageJapanSurrender;
	lCampaign.CampaignDebriefingScreen.SmallMapImage =  EImageIdFrontendCampaignDebriefingImageJapanSurrenderSmall;
	lCampaign.CampaignDebriefingScreen.BriefingScreenNameId = new CString("Frontend.CampaignNamesAndLevels.Campaign4Conclusion");
	lCampaign.CampaignDebriefingScreen.BriefingDescriptionId = new CString("Frontend.CampaignBriefingAndDebriefing.Campaign4Debriefing");
	//Add to List
	iCampaigns->Append(new SCampaign(lCampaign));

	//Best oF All Mission
	iBestOfAllMission.Score = 0;
	iBestOfAllMission.Time = new CString("??:??:??");
	iBestOfAllMission.MissionImage = EImageIdFrontendMissionImageCrewBeforeWildCat;
	iBestOfAllMission.MissionNameId = new CString("Frontend.HighscoresView.AcesOfThePacificWar");
	iBestOfAllMission.MissionNameForHighscoreTable = new CString(BEST_OF_ALL_NAME);
	InitializeMissionHighscoreTable(iBestOfAllMission.HighscoreTable);
}

//------------------------ functions --------------------//

void CSaveGameObject::UnlockNextCampaign(SCampaign* aCurrentCampaign)
{

}

void CSaveGameObject::UnlockNextMission(TGameMap aMapId)
{

}

void CSaveGameObject::LoadFromDisk()
{
	CNativeFile* lSaveFile = new CNativeFile(SAVE_GAME_INI_FILE);
	CFileReadStream* lReadStream = lSaveFile->OpenForReading();

	if(lReadStream != NULL)
	{
		if(lReadStream->ReadInt32() == SAVE_GAME_INI_FILE_VERSION)
		{
			for(TInt lCampaignIndex = 0; lCampaignIndex < iCampaigns->GetCount(); lCampaignIndex++)
			{
				//Campaign
				iCampaigns->Get(lCampaignIndex)->Unlocked = lReadStream->ReadInt32();
				//Briefing Screens
				iCampaigns->Get(lCampaignIndex)->CampaignBriefingScreen.Unlocked = lReadStream->ReadInt32();
				iCampaigns->Get(lCampaignIndex)->CampaignDebriefingScreen.Unlocked = lReadStream->ReadInt32();
				//Mission 1
				SMission& lMission1 = iCampaigns->Get(lCampaignIndex)->Mission1;
				lMission1.CompletedBaby = lReadStream->ReadInt32();
				lMission1.CompletedEasy = lReadStream->ReadInt32();
				lMission1.CompletedMedium = lReadStream->ReadInt32();
				lMission1.CompletedHard = lReadStream->ReadInt32();
				lMission1.Score = lReadStream->ReadInt32Encrypted(0);
				lReadStream->ReadStringEncrypted(lMission1.Time, "??:??:??"); //read time
				lMission1.Unlocked = lReadStream->ReadInt32();
				ReadMissionHighscoreTableFromDisk(lReadStream, iCampaigns->Get(lCampaignIndex)->Mission1.HighscoreTable);
				//Mission 2
				SMission& lMission2 = iCampaigns->Get(lCampaignIndex)->Mission2;
				lMission2.CompletedBaby = lReadStream->ReadInt32();
				lMission2.CompletedEasy = lReadStream->ReadInt32();
				lMission2.CompletedMedium = lReadStream->ReadInt32();
				lMission2.CompletedHard = lReadStream->ReadInt32();
				lMission2.Score = lReadStream->ReadInt32Encrypted(0);
				lReadStream->ReadStringEncrypted(lMission2.Time, "??:??:??"); //read time
				lMission2.Unlocked = lReadStream->ReadInt32();
				ReadMissionHighscoreTableFromDisk(lReadStream, iCampaigns->Get(lCampaignIndex)->Mission2.HighscoreTable);
				//Mission 3
				SMission& lMission3 = iCampaigns->Get(lCampaignIndex)->Mission3;
				lMission3.CompletedBaby = lReadStream->ReadInt32();
				lMission3.CompletedEasy = lReadStream->ReadInt32();
				lMission3.CompletedMedium = lReadStream->ReadInt32();
				lMission3.CompletedHard = lReadStream->ReadInt32();
				lMission3.Score = lReadStream->ReadInt32Encrypted(0);
				lReadStream->ReadStringEncrypted(lMission3.Time, "??:??:??"); //read time
				lMission3.Unlocked = lReadStream->ReadInt32();
				ReadMissionHighscoreTableFromDisk(lReadStream, iCampaigns->Get(lCampaignIndex)->Mission3.HighscoreTable);
				//Mission 4
				SMission& lMission4 = iCampaigns->Get(lCampaignIndex)->Mission4;
				lMission4.CompletedBaby = lReadStream->ReadInt32();
				lMission4.CompletedEasy = lReadStream->ReadInt32();
				lMission4.CompletedMedium = lReadStream->ReadInt32();
				lMission4.CompletedHard = lReadStream->ReadInt32();
				lMission4.Score = lReadStream->ReadInt32Encrypted(0);
				lReadStream->ReadStringEncrypted(lMission4.Time, "??:??:??"); //read time
				lMission4.Unlocked = lReadStream->ReadInt32();
				ReadMissionHighscoreTableFromDisk(lReadStream, iCampaigns->Get(lCampaignIndex)->Mission4.HighscoreTable);
			}

			//Load Best Of All Mission
			iBestOfAllMission.Score = lReadStream->ReadInt32Encrypted(0);
			lReadStream->ReadStringEncrypted(iBestOfAllMission.Time, "??:??:??"); //read time
			ReadMissionHighscoreTableFromDisk(lReadStream, iBestOfAllMission.HighscoreTable);
		}

		lReadStream->CloseReadStream();
		delete lReadStream;
	}

	delete lSaveFile;
}

void CSaveGameObject::SaveToDisk()
{
	CNativeFile* lSaveFile = new CNativeFile(SAVE_GAME_INI_FILE);
	CFileWriteStream* lWriteStream = lSaveFile->OpenForWriting(EFileWriteStreamModeReplace);

	//write version
	lWriteStream->WriteInt32(SAVE_GAME_INI_FILE_VERSION);
	//write Settings
	for(TInt lCampaignIndex = 0; lCampaignIndex < iCampaigns->GetCount(); lCampaignIndex++)
	{
		//Campaign
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Unlocked);
		//Briefing Screens
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->CampaignBriefingScreen.Unlocked);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->CampaignDebriefingScreen.Unlocked);
		//Mission 1
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission1.CompletedBaby);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission1.CompletedEasy);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission1.CompletedMedium);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission1.CompletedHard);
		lWriteStream->WriteInt32Encrypted(iCampaigns->Get(lCampaignIndex)->Mission1.Score);
		lWriteStream->WriteStringEncrypted(iCampaigns->Get(lCampaignIndex)->Mission1.Time);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission1.Unlocked);
		WriteMissionHighscoreTableToDisk(lWriteStream, iCampaigns->Get(lCampaignIndex)->Mission1.HighscoreTable);
		//Mission 2
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission2.CompletedBaby);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission2.CompletedEasy);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission2.CompletedMedium);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission2.CompletedHard);
		lWriteStream->WriteInt32Encrypted(iCampaigns->Get(lCampaignIndex)->Mission2.Score);
		lWriteStream->WriteStringEncrypted(iCampaigns->Get(lCampaignIndex)->Mission2.Time);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission2.Unlocked);
		WriteMissionHighscoreTableToDisk(lWriteStream, iCampaigns->Get(lCampaignIndex)->Mission2.HighscoreTable);
		//Mission 3
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission3.CompletedBaby);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission3.CompletedEasy);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission3.CompletedMedium);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission3.CompletedHard);
		lWriteStream->WriteInt32Encrypted(iCampaigns->Get(lCampaignIndex)->Mission3.Score);
		lWriteStream->WriteStringEncrypted(iCampaigns->Get(lCampaignIndex)->Mission3.Time);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission3.Unlocked);
		WriteMissionHighscoreTableToDisk(lWriteStream, iCampaigns->Get(lCampaignIndex)->Mission3.HighscoreTable);
		//Mission 4
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission4.CompletedBaby);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission4.CompletedEasy);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission4.CompletedMedium);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission4.CompletedHard);
		lWriteStream->WriteInt32Encrypted(iCampaigns->Get(lCampaignIndex)->Mission4.Score);
		lWriteStream->WriteStringEncrypted(iCampaigns->Get(lCampaignIndex)->Mission4.Time);
		lWriteStream->WriteInt32(iCampaigns->Get(lCampaignIndex)->Mission4.Unlocked);
		WriteMissionHighscoreTableToDisk(lWriteStream, iCampaigns->Get(lCampaignIndex)->Mission4.HighscoreTable);
	}

	//Save Also Best Of All Mission
	lWriteStream->WriteInt32Encrypted(iBestOfAllMission.Score);
	lWriteStream->WriteStringEncrypted(iBestOfAllMission.Time);
	WriteMissionHighscoreTableToDisk(lWriteStream, iBestOfAllMission.HighscoreTable);

	lWriteStream->CloseWriteStream();
	delete lWriteStream;
	delete lSaveFile;
}

void CSaveGameObject::InitializeMissionHighscoreTable(SMissionHighscoreTable& aTable)
{
	InitializeMissionHighscoreTableEntry(aTable.Rank1);
	InitializeMissionHighscoreTableEntry(aTable.Rank2);
	InitializeMissionHighscoreTableEntry(aTable.Rank3);
	InitializeMissionHighscoreTableEntry(aTable.Rank4);
	InitializeMissionHighscoreTableEntry(aTable.Rank5);
	InitializeMissionHighscoreTableEntry(aTable.Rank6);
	InitializeMissionHighscoreTableEntry(aTable.Rank7);
	InitializeMissionHighscoreTableEntry(aTable.Rank8);
	InitializeMissionHighscoreTableEntry(aTable.Rank9);
	InitializeMissionHighscoreTableEntry(aTable.Rank10);
}

void CSaveGameObject::InitializeMissionHighscoreTableEntry(SMissionHighscoreTableEntry& aEntry)
{
	//initilize highscore table
	aEntry.PlayerName = new CString("Open");
	aEntry.Score = new CString("0");
	aEntry.Time = new CString("??:??:??");
	aEntry.Date = new CString("????:??:??");
}

void CSaveGameObject::WriteMissionHighscoreTableToDisk(CFileWriteStream* aWriteStream, SMissionHighscoreTable& aTable)
{
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank1);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank2);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank3);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank4);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank5);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank6);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank7);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank8);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank9);
	WriteMissionHighscoreTableEntryToDisk(aWriteStream, aTable.Rank10);
}

void CSaveGameObject::WriteMissionHighscoreTableEntryToDisk(CFileWriteStream* aWriteStream, SMissionHighscoreTableEntry& aTableEntry)
{
	aWriteStream->WriteString(aTableEntry.PlayerName);
	aWriteStream->WriteString(aTableEntry.Score);
	aWriteStream->WriteString(aTableEntry.Time);
	aWriteStream->WriteString(aTableEntry.Date);
}

void CSaveGameObject::ReadMissionHighscoreTableFromDisk(CFileReadStream* aReadStream, SMissionHighscoreTable& aTable)
{
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank1);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank2);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank3);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank4);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank5);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank6);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank7);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank8);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank9);
	ReadMissionHighscoreTableEntryFromDisk(aReadStream, aTable.Rank10);
}

void CSaveGameObject::ReadMissionHighscoreTableEntryFromDisk(CFileReadStream* aReadStream, SMissionHighscoreTableEntry& aTableEntry)
{
	aReadStream->ReadString(aTableEntry.PlayerName);
	aReadStream->ReadString(aTableEntry.Score);
	aReadStream->ReadString(aTableEntry.Time);
	aReadStream->ReadString(aTableEntry.Date);
}


