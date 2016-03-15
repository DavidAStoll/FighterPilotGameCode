/*
 ============================================================================
 Name		: SpawnValEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnValEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNVALEVERYTIMEINTERVAL_H
#define SPAWNVALEVERYTIMEINTERVAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CTimeEvent.h"

#define VAL_SPAWNING_HEIGHT 250

// CLASS DECLARATION

/**
 *  CSpawnValEveryTimeInterval
 * 
 */
class CSpawnValEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnValEveryTimeInterval();
	CSpawnValEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aHasNoAmmunitionLeft = false);
	CSpawnValEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TBool aSpawnOnLeftSide, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aHasNoAmmunitionLeft = false);
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
protected:
	TUint iTargetPriority;
	TInt iHeightOffset;
	TBool iSpawnOnLeftSide;
	TBool iHasNoAmmunitionLeft;
	};

#endif // SPAWNVALEVERYTIMEINTERVAL_H
