/*
 ============================================================================
 Name		: LoseIfObjectLeavesMap.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CLoseIfObjectLeavesMap declaration
 ============================================================================
 */

#ifndef LOSEIFOBJECTLEAVESMAP_H
#define LOSEIFOBJECTLEAVESMAP_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

// CLASS DECLARATION

/**
 *  CLoseIfObjectLeavesMap
 * 
 */
class CLoseIfObjectLeavesMap : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLoseIfObjectLeavesMap();
	CLoseIfObjectLeavesMap( TInt aObjectsNeedToSurvive);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CMoveableGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CMoveableGameObject>* iGameObjectsThatNeedToLeave;
	TInt iObjectsThatLeftMap;
	TInt iObjectsThatNeedToSurvive;
	};

#endif // LOSEIFOBJECTLEAVESMAP_H
