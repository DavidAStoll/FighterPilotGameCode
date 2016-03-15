/*
 ============================================================================
 Name		: WinIfObjectsAreCaptured.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWinIfObjectsAreCaptured declaration
 ============================================================================
 */

#ifndef WINIFOBJECTSARECAPTURED_H
#define WINIFOBJECTSARECAPTURED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
class CBuilding;

// CLASS DECLARATION

/**
 *  CWinIfObjectsAreCaptured
 * 
 */
class CWinIfObjectsAreCaptured : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWinIfObjectsAreCaptured();
	CWinIfObjectsAreCaptured();//just used for loading purposes
	CWinIfObjectsAreCaptured(const char* aStringId, TInt aBonusScore);
	
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddBuilding(CBuilding* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CBuilding>* iBuildingsThatNeedToBeCaptured;
	TInt iBonusScore;
	CString* iStringId;
	};

#endif // WINIFOBJECTSARECAPTURED_H
