/*
 ============================================================================
 Name		: SpawnJapaneseSquadEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnJapaneseSquadEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/core/utility/CRandomGenerator.h"

CSpawnJapaneseSquadEveryTimeInterval::CSpawnJapaneseSquadEveryTimeInterval(TJapaneseSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats) :
	CTimeEvent(EEventIdentifierSpawnJapaneseSquadEveryTimerInterval, aPeriodInSeconds,aInitialExtraDelayInSec,aNumberOfRepeats)
{
	iSquadType = aSquadType;
	iOrder = aOrder;
}

CSpawnJapaneseSquadEveryTimeInterval::~CSpawnJapaneseSquadEveryTimeInterval()
{
}

void CSpawnJapaneseSquadEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = 0;
		TInt lSquadWidth = 0;
		switch(iSquadType)
		{
			case EJapaneseSquadTypeSoldierSquad:
			{
				lSquadWidth = CJapaneseSquad::GetWidthOfJapaneseSoldierSquad();
				lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(lSquadWidth);
				CJapaneseSquad::CreateJapaneseSoldierSquad(lXSpawningLocation, true)->GiveOrder(iOrder);
				break;
			}
			case EJapaneseSquadTypeSoldierAndTankSquad:
			case EJapaneseSquadTypeSoldierAATankSquad:
			{
				TInt lReturnNumber = CGame::Game->iRandomGenerator->GetRandomNumberTInt(1,101);
				if(lReturnNumber < 50 )
				{
					lSquadWidth = CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
					lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(lSquadWidth);
					CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(lXSpawningLocation, true)->GiveOrder(iOrder);
				}
				else
				{
					lSquadWidth = CJapaneseSquad::GetWidthOfJapaneseSoldierAATankSquad();
					lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(lSquadWidth);
					CJapaneseSquad::CreateJapaneseSoldierAATankSquad(lXSpawningLocation, true)->GiveOrder(iOrder);
				}
				break;
			}
			case EJapaneseSquadTypeTankSquad:
			{
				lSquadWidth = CJapaneseSquad::GetWidthOfJapaneseTankSquad();
				lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(lSquadWidth);
				CJapaneseSquad::CreateJapaneseTankSquad(lXSpawningLocation, true)->GiveOrder(iOrder);
				break;
			}
		}
	}
}

void CSpawnJapaneseSquadEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
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

void CSpawnJapaneseSquadEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iSquadType = static_cast<TJapaneseSquadTypes> (aReadStream.ReadInt32());
	iOrder.ShouldProtectArea = aReadStream.ReadInt32();
	iOrder.DefaultXPosition = aReadStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat();
	iOrder.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrder.MoveToLocation = aReadStream.ReadInt32();
	iOrder.XLocationDestination = aReadStream.ReadInt32();
}
