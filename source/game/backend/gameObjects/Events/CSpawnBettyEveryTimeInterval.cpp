/*
 ============================================================================
 Name		: SpawnBettyEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnBettyEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"


CSpawnBettyEveryTimeInterval::CSpawnBettyEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TBool aSpawnOnLeftSide, TInt aNumberOfRepeats) :
CTimeEvent(EEventIdentifierSpawnBettyEveryTimerInterval, aPeriodInSeconds, aInitialExtraDelayInSec, aNumberOfRepeats)
	{
	iHeightOffset = aHeightOffset;
	iSpawnOnLeftSide = aSpawnOnLeftSide;
	}

CSpawnBettyEveryTimeInterval::~CSpawnBettyEveryTimeInterval()
	{
	}

void CSpawnBettyEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation;
		if(iSpawnOnLeftSide)
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(BETTY_WIDTH);
		else
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(BETTY_WIDTH);
		CBetty* lBetty = CBetty::New(iSpawnOnLeftSide, TIntFloat::Convert(BETTY_MAX_ACCELERATION), true, TPoint(lXSpawningLocation, COMSTATION_BOMBER_SPAWNING_HEIGHT+ iHeightOffset));
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lBetty);
	}
}

void CSpawnBettyEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iHeightOffset);
	aOutputStream.WriteInt32(iSpawnOnLeftSide);
}

void CSpawnBettyEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iHeightOffset = aReadStream.ReadInt32();
	iSpawnOnLeftSide = aReadStream.ReadInt32();
}
