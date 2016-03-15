/*
 ============================================================================
 Name		: WinIfObjectsAreDestroyedEvent.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWinIfObjectsAreDestroyedEvent declaration
 ============================================================================
 */

#ifndef WINIFOBJECTSAREDESTROYEDEVENT_H
#define WINIFOBJECTSAREDESTROYEDEVENT_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"



// CLASS DECLARATION

/**
 *  CWinIfObjectsAreDestroyedEvent
 * 
 */
class CWinIfObjectsAreDestroyedEvent : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWinIfObjectsAreDestroyedEvent();

	/**
	 * Two-phased constructor.
	 */
	CWinIfObjectsAreDestroyedEvent();//just used for loading purposes
	CWinIfObjectsAreDestroyedEvent(const char* aStringId, TInt aBonusScore);
	
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

#endif // WINIFOBJECTSAREDESTROYEDEVENT_H
