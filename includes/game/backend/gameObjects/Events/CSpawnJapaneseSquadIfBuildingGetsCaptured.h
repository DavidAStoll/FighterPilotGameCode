/*
 ============================================================================
 Name		: SpawnJapaneseSquadIfBuildingGetsCaptured.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnJapaneseSquadIfBuildingGetsCaptured declaration
 ============================================================================
 */

#ifndef SPAWNJAPANESESQUADIFBUILDINGGETSCAPTURED_H
#define SPAWNJAPANESESQUADIFBUILDINGGETSCAPTURED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"
#include "includes/game/backend/gameObjects/GroundUnits/CJapaneseSquad.h"
class CBuilding;

// CLASS DECLARATION

/**
 *  CSpawnJapaneseSquadIfBuildingGetsCaptured
 * 
 */
class CSpawnJapaneseSquadIfBuildingGetsCaptured : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnJapaneseSquadIfBuildingGetsCaptured();
	CSpawnJapaneseSquadIfBuildingGetsCaptured(TJapaneseSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aSecDelay);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddBuilding(CBuilding* aBuilding);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TJapaneseSquadTypes iSquadType;
	SGroundUnitOrder iOrder;
	TInt iFrameDelay;
	CPointerArray<CBuilding>* iBuildingsThatNeedToBeCaptured;

	};

#endif // SPAWNJAPANESESQUADIFBUILDINGGETSCAPTURED_H
