/*
 * CGameData.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: dstoll
 */

#include "includes/core/CGame.h"
#include "includes/game/backend/CGameData.h"
#include "includes/game/backend/gameObjects/Maps/GameMaps.h"

CGameData::CGameData()
{
	iGamePaused = false;
	iTutorialModeActivated = false;
	iGameMap = NULL;
	iCampaignData = NULL;
	iMissionData = NULL;
	iSelectedMap = EGameMapNone;
}

CGameData::~CGameData()
{
	DeleteMap();
}

CGameData* CGameData::New()
{
	CGameData* lSelf = new CGameData();
	return lSelf;
}

//------------------ functions ------------------//

CMap* CGameData::GetMap()
{
	return iGameMap;
}

void CGameData::DeleteMap()
{
	if(iGameMap != NULL)
	{
		iGameMap->DeleteMap();
		iGameMap = NULL;
	}
}

void CGameData::LoadGame(SCampaign* aCampaignData, SMission* aMissionData)
{
	//update Mission
	iCampaignData = aCampaignData;
	iMissionData = aMissionData;

	//find out what Map we need to start
	TGameMap lMapId = CMap::GetMapIdFromSaveGameFile(aMissionData->SaveGameFile->GetData());

	//need to delete pervious map if the map is different
	if(iGameMap != NULL)
	{
		if(lMapId == iSelectedMap)
		{
			//same map, so we don't need to reupload textures and such
			iGameMap->LoadGame(aMissionData->SaveGameFile->GetData());
			return; //all done
		}
		else
		{
			DeleteMap();
		}
	}

	//start map from scratch, also loades textures that we need for this map
	StartMap(iCampaignData, iMissionData);
	//load specific save game data
	iGameMap->LoadGame(aMissionData->SaveGameFile->GetData());

	//pause game just to be sure
	SetPaused(true);
}

void CGameData::StartMap(SCampaign* aCampaignData, SMission* aMissionData)
{
	//update Mission
	iCampaignData = aCampaignData;
	iMissionData = aMissionData;

	//make sure game starts right away
	SetPaused(false);

	//need to delete pervious map if the map is different
	if(iGameMap != NULL)
	{
		if(aMissionData->MapId == iSelectedMap)
		{
			//same map, so we don't need to reupload textures and such
			iGameMap->RestartMap();
			return; //all done
		}
		else
		{
			DeleteMap();
		}
	}

	iSelectedMap = aMissionData->MapId;

	switch(iSelectedMap)
	{
		case EGameMapTutorialMission1:
		{
			iGameMap = CTutorial1Map::New();
			break;
		}
		case EGameMapTutorialMission2:
		{
			iGameMap = CTutorial2Map::New();
			break;
		}
		case EGameMapTutorialMission3:
		{
			iGameMap = CTutorial3Map::New();
			break;
		}
		case EGameMapCampaign1Mission1:
		{
			iGameMap = CCampaign1Map1::New();
			break;
		}
		case EGameMapCampaign1Mission2:
		{
			iGameMap = CCampaign1Map2::New();
			break;
		}
		case EGameMapCampaign1Mission3:
		{
			iGameMap = CCampaign1Map3::New();
			break;
		}
		case EGameMapCampaign1Mission4:
		{
			iGameMap = CCampaign1Map4::New();
			break;
		}
		case EGameMapCampaign2Mission1:
		{
			iGameMap = CCampaign2Map1::New();
			break;
		}
		case EGameMapCampaign2Mission2:
		{
			iGameMap = CCampaign2Map2::New();
			break;
		}
		case EGameMapCampaign2Mission3:
		{
			iGameMap = CCampaign2Map3::New();
			break;
		}
		case EGameMapCampaign2Mission4:
		{
			iGameMap = CCampaign2Map4::New();
			break;
		}
		case EGameMapCampaign3Mission1:
		{
			iGameMap = CCampaign3Map1::New();
			break;
		}
		case EGameMapCampaign3Mission2:
		{
			iGameMap = CCampaign3Map2::New();
			break;
		}
		case EGameMapCampaign3Mission3:
		{
			iGameMap = CCampaign3Map3::New();
			break;
		}
		case EGameMapCampaign3Mission4:
		{
			iGameMap = CCampaign3Map4::New();
			break;
		}
		case EGameMapCampaign4Mission1:
		{
			iGameMap = CCampaign4Map1::New();
			break;
		}
		case EGameMapCampaign4Mission2:
		{
			iGameMap = CCampaign4Map2::New();
			break;
		}
		case EGameMapCampaign4Mission3:
		{
			iGameMap = CCampaign4Map3::New();
			break;
		}
		case EGameMapCampaign4Mission4:
		{
			iGameMap = CCampaign4Map4::New();
			break;
		}

		default:
			break;//do nothing
	}

	//play Mission Song
	CGame::Game->iAudioEngine->GetMusicPlayer().PlayGameSong(iMissionData->MusicFile->GetData(), true);
}

void CGameData::SetPaused(TBool aValue)
{
	iGamePaused = aValue;
}

void CGameData::SetTutorialMode(TBool aActivated)
{
	iTutorialModeActivated = true;
}

