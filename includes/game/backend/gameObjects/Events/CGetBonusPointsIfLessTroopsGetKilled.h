/*
 ============================================================================
 Name		: GetBonusPointsIfLessTroopsGetKilled.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGetBonusPointsIfLessTroopsGetKilled declaration
 ============================================================================
 */

#ifndef GETBONUSPOINTSIFLESSTROOPSGETKILLED_H
#define GETBONUSPOINTSIFLESSTROOPSGETKILLED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
// CLASS DECLARATION

/**
 *  CGetBonusPointsIfLessTroopsGetKilled
 * 
 */
class CGetBonusPointsIfLessTroopsGetKilled : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGetBonusPointsIfLessTroopsGetKilled();
	CGetBonusPointsIfLessTroopsGetKilled();//for loading
	CGetBonusPointsIfLessTroopsGetKilled(const char* aStringId, TInt aBonusScore, TInt aMaxSoldiersKilled, TInt aMaxTanksDestroyed);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TInt iBonusScore;
	CString* iStringId;
	TInt iMaxSoldierKilled;
	TInt iMaxTanksDestroyed;

	};

#endif // GETBONUSPOINTSIFLESSTROOPSGETKILLED_H
