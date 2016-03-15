/*
 ============================================================================
 Name		: GetBonusPointsIfObjectsAreDestroyedEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGetBonusPointsIfObjectsAreDestroyedEvent declaration
 ============================================================================
 */

#ifndef GETBONUSPOINTSIFOBJECTSAREDESTROYEDEVENT_H
#define GETBONUSPOINTSIFOBJECTSAREDESTROYEDEVENT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

// CLASS DECLARATION

/**
 *  CGetBonusPointsIfObjectsAreDestroyedEvent
 * 
 */
class CGetBonusPointsIfObjectsAreDestroyedEvent: public CEvent
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGetBonusPointsIfObjectsAreDestroyedEvent();
	CGetBonusPointsIfObjectsAreDestroyedEvent(); //used for loading only
	CGetBonusPointsIfObjectsAreDestroyedEvent(const char* aStrindId, TInt aBonusScore);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CGameObject>* iGameObjectsThatNeedToBeDestroyed;
	TInt iBonusScore;
	CString* iStringId;
	TBool iObjectLeftMap;
};

#endif // GETBONUSPOINTSIFOBJECTSAREDESTROYEDEVENT_H
