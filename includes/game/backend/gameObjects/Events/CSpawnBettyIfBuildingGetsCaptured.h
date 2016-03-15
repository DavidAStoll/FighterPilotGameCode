/*
 ============================================================================
 Name		: SpawnBettyIfBuildingGetsCaptured.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnBettyIfBuildingGetsCaptured declaration
 ============================================================================
 */

#ifndef SPAWNBETTYIFBUILDINGGETSCAPTURED_H
#define SPAWNBETTYIFBUILDINGGETSCAPTURED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/gameObjects/Aircrafts/CBetty.h"
class CBuilding;

// CLASS DECLARATION

/**
 *  CSpawnBettyIfBuildingGetsCaptured
 * 
 */
class CSpawnBettyIfBuildingGetsCaptured : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnBettyIfBuildingGetsCaptured();
	CSpawnBettyIfBuildingGetsCaptured(TInt aSecDelay, TInt aExtraHeightOffset);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddBuilding(CBuilding* aBuilding);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TInt iExtraHeightOffset;
	TInt iFrameDelay;
	CPointerArray<CBuilding>* iBuildingsThatNeedToBeCaptured;

	};

#endif // SPAWNBETTYIFBUILDINGGETSCAPTURED_H
