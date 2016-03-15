/*
 ============================================================================
 Name		: SpawnAmericanLCTEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanLCTEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNAMERICANLCTEVERYTIMEINTERVAL_H
#define SPAWNAMERICANLCTEVERYTIMEINTERVAL_H

// INCLUDES
#include "includes/game/backend/basicClasses/CTimeEvent.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"
#include "includes/game/backend/gameObjects/Ships/CAmericanLCT.h"

// CLASS DECLARATION

/**
 *  CSpawnAmericanLCTEveryTimeInterval
 * 
 */
class CSpawnAmericanLCTEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	CSpawnAmericanLCTEveryTimeInterval(TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats);
	virtual ~CSpawnAmericanLCTEveryTimeInterval();
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */

	TAmericanLCTType iLCTType;
	SGroundUnitOrder iOrder;
	};

#endif // SPAWNAMERICANLCTEVERYTIMEINTERVAL_H
