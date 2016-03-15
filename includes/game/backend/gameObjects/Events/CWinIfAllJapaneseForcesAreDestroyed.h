/*
 ============================================================================
 Name		: WinIfAllJapaneseForcesAreDestroyed.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWinIfAllJapaneseForcesAreDestroyed declaration
 ============================================================================
 */

#ifndef WINIFALLJAPANESEFORCESAREDESTROYED_H
#define WINIFALLJAPANESEFORCESAREDESTROYED_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

// CLASS DECLARATION

/**
 *  CWinIfAllJapaneseForcesAreDestroyed
 * 
 */
class CWinIfAllJapaneseForcesAreDestroyed : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CWinIfAllJapaneseForcesAreDestroyed();
	CWinIfAllJapaneseForcesAreDestroyed(TInt aBonusScore);

	
	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:

	TInt iBonusScore;
	};

#endif // WINIFALLJAPANESEFORCESAREDESTROYED_H
