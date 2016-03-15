/*
 ============================================================================
 Name		: SpawnAmericanTroopTransporterIfObjectDestroyed.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanTroopTransporterIfObjectsDestroyed declaration
 ============================================================================
 */

#ifndef SPAWNAMERICANTROOPTRANSPORTERIFOBJECTSDESTROYED_H
#define SPAWNAMERICANTROOPTRANSPORTERIFOBJECTSDESTROYED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/gameObjects/Ships/CAmericanLCT.h"
// CLASS DECLARATION

/**
 *  CSpawnAmericanTroopTransporterIfObjectsDestroyed
 * 
 */
class CSpawnAmericanTroopTransporterIfObjectsDestroyed : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnAmericanTroopTransporterIfObjectsDestroyed();
	CSpawnAmericanTroopTransporterIfObjectsDestroyed(TInt aXMoveToLocation, TAmericanLCTType aLctType, SGroundUnitOrder aOrder);
	
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:
	
	CPointerArray<CGameObject>* iObjectsThatNeedToBeDestroyed;
	TAmericanLCTType iLctType;
	SGroundUnitOrder iOrder;
	TIntFloat iXMoveToLocation;

	};

#endif // SPAWNAMERICANTROOPTRANSPORTERIFOBJECTDESTROYED_H
