/*
 ============================================================================
 Name		: GetBonusPointsIfObjectsSurvive.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGetBonusPointsIfObjectsSurvive declaration
 ============================================================================
 */

#ifndef GETBONUSPOINTSIFOBJECTSSURVIVE_H
#define GETBONUSPOINTSIFOBJECTSSURVIVE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

// CLASS DECLARATION

/**
 *  CGetBonusPointsIfObjectsSurvive
 * 
 */
class CGetBonusPointsIfObjectsSurvive : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGetBonusPointsIfObjectsSurvive();
	CGetBonusPointsIfObjectsSurvive(); //used for loading only
	CGetBonusPointsIfObjectsSurvive(const char* aStringId, TInt aBonusScore);
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	void AddGameObject(CGameObject* aGameObject);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	CPointerArray<CGameObject>* iGameObjectsThatNeedToSurvive;
	TInt iBonusScore;
	CString* iStringId;
	TBool iSuccess;

	};

#endif // GETBONUSPOINTSIFOBJECTSSURVIVE_H
