/*
 ============================================================================
 Name		: SpawnAmericanSquadEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnAmericanSquadEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CSpawnAmericanSquadEveryTimeInterval::CSpawnAmericanSquadEveryTimeInterval(TAmericanSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats) :
	CTimeEvent(EEventIdentifierSpawnAmericanSquadEveryTimerInterval, aPeriodInSeconds,aInitialExtraDelayInSec,aNumberOfRepeats)
{
	iSquadType = aSquadType;
	iOrder = aOrder;
}

CSpawnAmericanSquadEveryTimeInterval::~CSpawnAmericanSquadEveryTimeInterval()
{
}

void CSpawnAmericanSquadEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = 0;
		TInt lSquadWidth = 0;
		switch(iSquadType)
		{
			case EAmericanSquadTypeSoldierSquad:
			{
				lSquadWidth = CAmericanSquad::GetWidthOfAmericanSoldierSquad();
				lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(lSquadWidth);
				CAmericanSquad::CreateAmericanSoldierSquad(lXSpawningLocation, false)->GiveOrder(iOrder);
				break;
			}
			case EAmericanSquadTypeSoldierAndTankSquad:
			case EAmericanSquadTypeSoldierAATankSquad:
			{
				TInt lReturnNumber = CGame::Game->iRandomGenerator->GetRandomNumberTInt(1,101);
				if(lReturnNumber < 50 )
				{
					lSquadWidth = CAmericanSquad::GetWidthOfAmericanSoldierAndTankSquad();
					lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(lSquadWidth);
					CAmericanSquad::CreateAmericanSoldierAndTankSquad(lXSpawningLocation, false)->GiveOrder(iOrder);
				}
				else
				{
					lSquadWidth = CAmericanSquad::GetWidthOfAmericanSoldierAATankSquad();
					lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(lSquadWidth);
					CAmericanSquad::CreateAmericanSoldierAATankSquad(lXSpawningLocation, false)->GiveOrder(iOrder);
				}
				break;
			}
			case EAmericanSquadTypeTankSquad:
			{
				lSquadWidth = CAmericanSquad::GetWidthOfAmericanTankSquad();
				lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(lSquadWidth);
				CAmericanSquad::CreateAmericanTankSquad(lXSpawningLocation, false)->GiveOrder(iOrder);
				break;
			}
		}
	}
}

void CSpawnAmericanSquadEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iSquadType);
	aOutputStream.WriteInt32(iOrder.ShouldProtectArea);
	aOutputStream.WriteInt32(iOrder.DefaultXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.LeftXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrder.MoveToLocation);
	aOutputStream.WriteInt32(iOrder.XLocationDestination);
}

void CSpawnAmericanSquadEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iSquadType = static_cast<TAmericanSquadTypes> (aReadStream.ReadInt32());
	iOrder.ShouldProtectArea = aReadStream.ReadInt32();
	iOrder.DefaultXPosition = aReadStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat();
	iOrder.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrder.MoveToLocation = aReadStream.ReadInt32();
	iOrder.XLocationDestination = aReadStream.ReadInt32();
}

