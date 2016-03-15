/*
 * CStatsTracker.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CSTATSTRACKER_H_
#define CSTATSTRACKER_H_

#include "includes/game/backend/basicClasses/CGameObject.h"

struct SBonusObjective
{
	CString* NameOfObjective;
	TInt ExtraScore;
};

// CLASS DECLARATION

/**
 *  CStatsTracker
 *
 */
class CStatsTracker
{
public:

	~CStatsTracker();
	static CStatsTracker* New();

	void UpdateScoreObjectKilled(TGameObjectIdentifier aId, CGameObject* aDestroyedGameObject);//called by an object when it has been destroyed by the player
	void UpdateScoreObjectLost(TGameObjectIdentifier aId, CGameObject* aDestroyedGameObject);//called by an object when it has been destroyed and was part of the player's side
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);

	//to keep tracks of stats, need to be public otherwise it will be a pain to access them

	TGameObjectIdentifier lTest;

	//planes
	TInt iNumberOfDiveBombersKilled;
	TInt iNumberOfDiveBombersLost;
	TInt iNumberOfFightersKilled;
	TInt iNumberOfFightersLost;
	TInt iNumberOfHeavyBombersKilled;
	TInt iNumberOfHeavyBombersLost;

	//ships
	TInt iNumberOfUSSVictoryLost;
	TInt iNumberOfAircraftCarriersKilled;
	TInt iNumberOfAircraftCarriersLost;
	TInt iNumberOfDestroyerKilled;
	TInt iNumberOfDestroyerLost;
	TInt iNumberOfBattleshipUSSTennesseeLost;
	TInt iNumberOfBattleshipYamatoKilled;
	TInt iNumberOfLCTEmptyKilled;
	TInt iNumberOfLCTEmptyLost;
	TInt iNumberOfLCTLoadedKilled;
	TInt iNumberOfLCTLoadedLost;
	TInt iNumberOfTroopTransporterKilled;
	TInt iNumberOfTroopTransporterLost;
	TInt iNumberOfOilTankerKilled;
	TInt iNumberOfOilTankerLost;

	//buildings
	TInt iNumberOfAirportsKilled;
	TInt iNumberOfAirportsLost;
	TInt iNumberOfComStationKilled;
	TInt iNumberOfComStationLost;
	TInt iNumberOfHangarKilled;
	TInt iNumberOfHangarLost;
	TInt iNumberOfHqKilled;
	TInt iNumberOfHqLost;
	TInt iNumberOfAA20mmMachineCannonKilled;
	TInt iNumberOfAA20mmMachineCannonLost;
	TInt iNumberOfAA75mmType88Killed;
	TInt iNumberOfAA75mmType88Lost;
	TInt iNumberOfAAConcreteKilled;
	TInt iNumberOfAAConcreteLost;

	//Ground Units
	TInt iNumberOfMediumTanksKilled;
	TInt iNumberOfMediumTanksLost;
	TInt iNumberOfAATanksKilled;
	TInt iNumberOfAATanksLost;
	TInt iNumberOfRiflemanKilled;
	TInt iNumberOfRiflemanLost;
	TInt iNumberOfAntiTankmanKilled;
	TInt iNumberOfAntiTankmanLost;

	//Time, don't need to save to disk since the Player object will reset them before finishing the mission
	TInt iGameTimeHours;
	TInt iGameTimeMins;
	TInt iGameTimeSecs;

	CPointerArray<SBonusObjective>* iBonusObjectives;

private:

	CStatsTracker();
	void ConstructL();

};

#endif /* CSTATSTRACKER_H_ */
