/*
 ============================================================================
 Name		: SpawnAmericanLCTEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanLCTEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSpawnAmericanLCTEveryTimeInterval::CSpawnAmericanLCTEveryTimeInterval(TAmericanLCTType aLCTType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats) :
	CTimeEvent(EEventIdentifierSpawnAmericanLCTEveryTimerInterval,aPeriodInSeconds,aInitialExtraDelayInSec,aNumberOfRepeats)
{
	iLCTType = aLCTType;
	iOrder = aOrder;
}

CSpawnAmericanLCTEveryTimeInterval::~CSpawnAmericanLCTEveryTimeInterval()
{
}

void CSpawnAmericanLCTEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(AMERICAN_LCT_WIDTH);
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(CAmericanLCT::New(lXSpawningLocation, false, iLCTType, iOrder));
	}
}

void CSpawnAmericanLCTEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iLCTType);
	aOutputStream.WriteInt32(iOrder.ShouldProtectArea);
	aOutputStream.WriteInt32(iOrder.DefaultXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.LeftXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrder.MoveToLocation);
	aOutputStream.WriteInt32(iOrder.XLocationDestination);
}

void CSpawnAmericanLCTEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iLCTType = static_cast<TAmericanLCTType> (aReadStream.ReadInt32());
	iOrder.ShouldProtectArea = aReadStream.ReadInt32();
	iOrder.DefaultXPosition = aReadStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat();
	iOrder.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrder.MoveToLocation = aReadStream.ReadInt32();
	iOrder.XLocationDestination = aReadStream.ReadInt32();
}

