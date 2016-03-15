/*
 * CEvent.h
 *
 *  Created on: Aug 25, 2012
 *      Author: dstoll
 */

#ifndef CEVENT_H_
#define CEVENT_H_

// INCLUDES
#include "includes/game/backend/CStatsTracker.h"
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

class CMoveableGameObject;

// CLASS DECLARATION

enum TEventIdentifier
{
	/*Just used in Tutorials*/
	EEventIdentifierRestartLastTutorialIfDied = 0,
	//Tutorial 1 (Navigation)
	EEventIdentifierLoopTutorial,
	EEventIdentifierTurnTutorial,
	EEventIdentifierSpeedUpTutorial,
	EEventIdentifierSlowDownTutorial,
	EEventIdentifierZoomInAndOutTutorial,
	EEventIdentifierMovetoEndOfMapAndAvoidAA,
	EEventIdentifierStartFromCarrier,
	EEventIdentifierLandOnCarrier,
	//Tutorial 2 (Weapons)
	EEventIdentifierFireButtons,
	EEventIdentifierBomb1000Kg,
	EEventIdentifierLoseIfFailedToKill,
	EEventIdentifierKillSoldiersUsingMachineGun,
	EEventIdentifierLoseIfAllAmericanSoldiersDied,
	EEventIdentifierDiveBombingKill,
	EEventIdentifierAttackBomber,
	EEventIdentifierAttackFighter,
	EEventIdentifierAttackShip,

	/*END*/
	//Win
	EEventIdentifierWinIfObjectsLeaveMapAlive,
	EEventIdentifierWinIfObjectsAreDestroyedEvent,
	EEventIdentifierWinIfAllJapaneseForcesAreDestroyed,
	EEventIdentifierWinIfAllJapaneseMoveableObjectsAreDestroyed,
	EEventIdentifierSpawnMitchellToAttackTargetClockTimer,
	EEventIdentifierWinIfObjectsAreCaptured,
	EEventIdentifierWinIfJapaneseDestroyedAfterCertainTime,
	//Lose
	EEventIdentifierLoseIfObjectsAreDestroyedEvent,
	EEventIdentifierLoseIfObjectsAreCaptured,
	EEventIdentifierLoseIfObjectsLeaveMapAlive,
	//Bonus
	EEventIdentifierGetBonusPointsIfObjectsAreDestroyed,
	EEventIdentifierGetBonusPointsIfObjectsSurvive,
	EEventIdentifierGetBonusPointsIfLessTroopsGetKilled,
	//Spawn
	EEventIdentifierSpawnAmericanLCTEveryTimerInterval,
	EEventIdentifierSpawnAmericanTroopTransporterIfObjectsAreDestroyed,
	EEventIdentifierSpawnAmericanSquadEveryTimerInterval,
	EEventIdentifierSpawnMitchellEveryTimerInterval,
	EEventIdentifierSpawnBettyEveryTimerInterval,
	EEventIdentifierSpawnJapaneseSquadEveryTimerInterval,
	EEventIdentifierSpawnJapaneseSquadIfBuildingGetsCaptured,
	EEventIdentifierSpawnBettyIfBuildingGetsCaptured,
	EEventIdentifierSpawnValIfBuildingGetsCaptured,
	EEventIdentifierSpawnValEveryTimerInterval,
	EEventIdentifierSpawnDauntlessEveryTimerInterval,
	EEventIdentifierSpawnDauntlessIfBuildingGetsCaptured,
	//Extra
	EEventIdentifierClockTimerEvent,

	EEventIdentifierStopReading
};

/**
 *  CEvent
 * This class triggers certain changes to a map and its objects if certain conditions have been met
 * This class will be inherited with more specific event types
 */
class CEvent
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CEvent();
	CEvent(TEventIdentifier aEventId);

	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	TBool EventHasAlreadyHappened();// just wrapper function that returns HasHappened
	TEventIdentifier GetEventIdentifier();
	void BasicEvenSaveOnDisk(CFileWriteStream &aOutputStream);
	void BasicEventLoadFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	TBool iHasHappened;
	TEventIdentifier iEventIdentifier;

	/**
	 * Constructor for performing 1st stage construction
	 */

};


#endif /* CEVENT_H_ */
