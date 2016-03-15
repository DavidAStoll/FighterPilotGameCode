/*
 ============================================================================
 Name		: LoseIfObjectsAreCaptured.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CLoseIfObjectsAreCaptured declaration
 ============================================================================
 */

#ifndef LOSEIFOBJECTSARECAPTURED_H
#define LOSEIFOBJECTSARECAPTURED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
class CBuilding;

// CLASS DECLARATION

/**
 *  CLoseIfObjectsAreCaptured
 * 
 */
class CLoseIfObjectsAreCaptured : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLoseIfObjectsAreCaptured();
	CLoseIfObjectsAreCaptured();//just used for loading purposes
	
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddBuilding(CBuilding* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CBuilding>* iBuildingsThatNeedToBeCaptured;
	};

#endif // LOSEIFOBJECTSARECAPTURED_H
