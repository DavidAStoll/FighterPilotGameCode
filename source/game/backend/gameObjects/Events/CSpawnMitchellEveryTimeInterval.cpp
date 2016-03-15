/*
 ============================================================================
 Name		: SpawnMitchellEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnMitchellEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSpawnMitchellEveryTimeInterval::CSpawnMitchellEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats, TUint aTargetPriority) :
	CTimeEvent(EEventIdentifierSpawnMitchellEveryTimerInterval, aPeriodInSeconds, aInitialExtraDelayInSec, aNumberOfRepeats)
{
	iTargetPriority = aTargetPriority;
}

CSpawnMitchellEveryTimeInterval::~CSpawnMitchellEveryTimeInterval()
{
}

void CSpawnMitchellEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(MITCHELL_WIDTH);
		CMitchell* lMitchell = CMitchell::New(false, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocation, COMSTATION_BOMBER_SPAWNING_HEIGHT));
		lMitchell->GetHeavyBomberAI()->SetTargetPriority(iTargetPriority);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lMitchell);
	}
}

void CSpawnMitchellEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iTargetPriority);
}

void CSpawnMitchellEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iTargetPriority = aReadStream.ReadInt32();
}
