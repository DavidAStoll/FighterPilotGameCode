/*
 ============================================================================
 Name		: WinIfObjectsLeaveMapAlive.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWinIfObjectsLeaveMapAlive declaration
 ============================================================================
 */

#ifndef WINIFOBJECTSLEAVEMAPALIVE_H
#define WINIFOBJECTSLEAVEMAPALIVE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
// CLASS DECLARATION

/**
 *  CWinIfObjectsLeaveMapAlive
 * 
 */
class CWinIfObjectsLeaveMapAlive: public CEvent
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWinIfObjectsLeaveMapAlive();
	CWinIfObjectsLeaveMapAlive(); //used for loading only
	CWinIfObjectsLeaveMapAlive(const char* aStringId, TInt aBonusScore, TInt aObjectsNeedToSurvive);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CMoveableGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CMoveableGameObject>* iGameObjectsThatNeedToLeave;
	CString* iStringId;
	TInt iBonusScore;
	TInt iObjectsThatLeftMap;
	TInt iObjectsThatNeedToSurvive;
};

#endif // WINIFOBJECTSLEAVEMAPALIVE_H
