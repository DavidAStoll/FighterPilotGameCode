/*
 ============================================================================
 Name		: SpawnJapaneseSquadEveryTimeInterval.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnJapaneseSquadEveryTimeInterval declaration
 ============================================================================
 */

#ifndef SPAWNJAPANESESQUADEVERYTIMEINTERVAL_H
#define SPAWNJAPANESESQUADEVERYTIMEINTERVAL_H

#include "includes/game/backend/basicClasses/CTimeEvent.h"
#include "includes/game/backend/gameObjects/AI/CGroundUnitOverlordAI.h"
#include "includes/game/backend/gameObjects/GroundUnits/CJapaneseSquad.h"

// CLASS DECLARATION

/**
 *  CSpawnJapaneseSquadEveryTimeInterval
 * 
 */
class CSpawnJapaneseSquadEveryTimeInterval: public CTimeEvent
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpawnJapaneseSquadEveryTimeInterval();
	CSpawnJapaneseSquadEveryTimeInterval(TJapaneseSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats);
	virtual void Run();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TJapaneseSquadTypes iSquadType;
	SGroundUnitOrder iOrder;

};

#endif // SPAWNJAPANESESQUADEVERYTIMEINTERVAL_H
