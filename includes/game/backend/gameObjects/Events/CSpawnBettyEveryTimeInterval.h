/*
 ============================================================================
 Name		: SpawnBettyEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnBettyEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNBETTYEVERYTIMEINTERVAL_H
#define SPAWNBETTYEVERYTIMEINTERVAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CTimeEvent.h"

// CLASS DECLARATION

/**
 *  CSpawnBettyEveryTimeInterval
 * 
 */
class CSpawnBettyEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnBettyEveryTimeInterval();
	CSpawnBettyEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TBool aSpawnOnLeftSide, TInt aNumberOfRepeats);
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	TInt iHeightOffset;
	TBool iSpawnOnLeftSide;

	};

#endif // SPAWNBETTYEVERYTIMEINTERVAL_H
