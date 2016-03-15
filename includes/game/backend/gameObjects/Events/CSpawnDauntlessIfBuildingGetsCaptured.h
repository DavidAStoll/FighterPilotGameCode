/*
 ============================================================================
 Name		: SpawnDauntlessIfBuildingGetsCaptured.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnDauntlessIfBuildingGetsCaptured declaration
 ============================================================================
 */

#ifndef SPAWNDAUNTLESSIFBUILDINGGETSCAPTURED_H
#define SPAWNDAUNTLESSIFBUILDINGGETSCAPTURED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/gameObjects/Aircrafts/CDauntless.h"
#include "includes/game/backend/gameObjects/AI/CFighterAI.h"
class CBuilding;

#define BUILDING_GETS_CAPTURED_DAUNTLESS_SPAWNING_HEIGHT 250

// CLASS DECLARATION

/**
 *  CSpawnDauntlessIfBuildingGetsCaptured
 * 
 */
class CSpawnDauntlessIfBuildingGetsCaptured : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnDauntlessIfBuildingGetsCaptured();
	CSpawnDauntlessIfBuildingGetsCaptured(TInt aSecDelay, TInt aExtraHeightOffset, TWeaponIdentifier aWeaponIdentifier, TUint aTargetPriority = FIGHTER_AI_PREFERRED_TARGET_TYPES);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddBuilding(CBuilding* aBuilding);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:

	TInt iExtraHeightOffset;
	TInt iFrameDelay;
	TUint iTargetPriority;
	TWeaponIdentifier iWeaponType;
	CPointerArray<CBuilding>* iBuildingsThatNeedToBeCaptured;

	};

#endif // SPAWNDAUNTLESSIFBUILDINGGETSCAPTURED_H
