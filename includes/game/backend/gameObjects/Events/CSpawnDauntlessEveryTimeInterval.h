/*
 ============================================================================
 Name		: SpawnDauntlessEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnDauntlessEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNDAUNTLESSEVERYTIMEINTERVAL_H
#define SPAWNDAUNTLESSEVERYTIMEINTERVAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CTimeEvent.h"

#define DAUNTLESS_SPAWNING_HEIGHT 250

// CLASS DECLARATION

/**
 *  CSpawnDauntlessEveryTimeInterval
 * 
 */
class CSpawnDauntlessEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnDauntlessEveryTimeInterval();
	CSpawnDauntlessEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aSpawnOfRightSide = true);
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:
	TUint iTargetPriority;
	TInt iHeightOffset;
	TBool iSpawnOnRightSide;
	};

#endif // SPAWNDAUNTLESSEVERYTIMEINTERVAL_H
