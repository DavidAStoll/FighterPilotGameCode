/*
 ============================================================================
 Name		: SpawnAmericanSquadEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanSquadEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNAMERICANSQUADEVERYTIMEINTERVAL_H
#define SPAWNAMERICANSQUADEVERYTIMEINTERVAL_H

#include "includes/game/backend/basicClasses/CTimeEvent.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"
#include "includes/game/backend/gameObjects/GroundUnits/CAmericanSquad.h"


// CLASS DECLARATION

/**
 *  CSpawnAmericanSquadEveryTimeInterval
 * 
 */
class CSpawnAmericanSquadEveryTimeInterval : public CTimeEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnAmericanSquadEveryTimeInterval();
	CSpawnAmericanSquadEveryTimeInterval(TAmericanSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats);
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
private:

	TAmericanSquadTypes iSquadType;
	SGroundUnitOrder iOrder;
	};

#endif // SPAWNAMERICANSQUADEVERYTIMEINTERVAL_H
