/*
 * CGameData.h
 *
 *  Created on: Aug 26, 2012
 *      Author: dstoll
 */

#ifndef CGAMEDATA_H_
#define CGAMEDATA_H_

#include "includes/game/backend/basicClasses/CMap.h"
#include "includes/game/common/CSaveGameObject.h"

class CGameData
{
public:

	virtual ~CGameData();
	static CGameData* New();

	//returns the currentMap Object
	CMap* GetMap();
	//deletes the current Map if one is being played
	void DeleteMap();
	//changes the map, to the one set in the savegame file and loads it
	void LoadGame(SCampaign* aCampaignData, SMission* aMissionData);
	// will start Map from scratch
	void StartMap(SCampaign* aCampaignData, SMission* aMissionData);
	//sets the game to Paused
	void SetPaused(TBool aValue);
	//sets the game mode to tutorial
	void SetTutorialMode(TBool aActivated);

	TBool iGamePaused;
	TBool iTutorialModeActivated;
	CMap* iGameMap;
	SCampaign* iCampaignData;
	SMission* iMissionData;

protected:
	CGameData();

	TGameMap iSelectedMap;

};

#endif /* CGAMEDATA_H_ */
