/*
 ============================================================================
 Name		: LoseIfObjectsAreDestroyedEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CLoseIfObjectsAreDestroyedEvent declaration
 ============================================================================
 */

#ifndef LOSEIFOBJECTSAREDESTROYEDEVENT_H
#define LOSEIFOBJECTSAREDESTROYEDEVENT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

class CGameObject;

// CLASS DECLARATION

/**
 *  CLoseIfObjectsAreDestroyedEvent
 * 
 */
class CLoseIfObjectsAreDestroyedEvent : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLoseIfObjectsAreDestroyedEvent();

	/**
	 * Two-phased constructor.
	 */
	static CLoseIfObjectsAreDestroyedEvent* New();
	
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLoseIfObjectsAreDestroyedEvent();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	CPointerArray<CGameObject>* iGameObjectsThatNeedToBeDestroyed;

	};

#endif // LOSEIFOBJECTSAREDESTROYEDEVENT_H
