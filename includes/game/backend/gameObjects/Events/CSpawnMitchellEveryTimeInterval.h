/*
 ============================================================================
 Name		: SpawnMitchellEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnMitchellEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNMITCHELLEVERYTIMEINTERVAL_H
#define SPAWNMITCHELLEVERYTIMEINTERVAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CTimeEvent.h"

// CLASS DECLARATION

/**
 *  CSpawnMitchellEveryTimeInterval
 * 
 */
class CSpawnMitchellEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	~CSpawnMitchellEveryTimeInterval();
	CSpawnMitchellEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats, TUint aTargetPriority = GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	virtual void Run();

private:
	TUint iTargetPriority;
	};

#endif // SPAWNMITCHELLEVERYTIMEINTERVAL_H
