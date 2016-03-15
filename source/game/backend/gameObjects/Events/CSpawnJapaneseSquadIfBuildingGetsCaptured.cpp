/*
 ============================================================================
 Name		: SpawnJapaneseSquadIfBuildingGetsCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnJapaneseSquadIfBuildingGetsCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CSpawnJapaneseSquadIfBuildingGetsCaptured.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/core/utility/CRandomGenerator.h"
#include "includes/game/common/CSaveGameObject.h"

CSpawnJapaneseSquadIfBuildingGetsCaptured::CSpawnJapaneseSquadIfBuildingGetsCaptured(TJapaneseSquadTypes aSquadType, SGroundUnitOrder& aOrder, TInt aSecDelay) :
	CEvent(EEventIdentifierSpawnJapaneseSquadIfBuildingGetsCaptured)
{
	iSquadType = aSquadType;
	iOrder = aOrder;
	iFrameDelay = aSecDelay * GFRAMES_PER_SECOND_BACKEND;
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CSpawnJapaneseSquadIfBuildingGetsCaptured::~CSpawnJapaneseSquadIfBuildingGetsCaptured()
{
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
}

void CSpawnJapaneseSquadIfBuildingGetsCaptured::TriggerEvent()
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
				if(lReturnNumber < 50)
				{
					lSquadWidth = CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad();
					lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(lSquadWidth);
					CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(lXSpawningLocation, true)->GiveOrder(iOrder);
				}else
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

TBool CSpawnJapaneseSquadIfBuildingGetsCaptured::CheckIfEventConditionIsMet()
{
	for(TInt lIndex = 0; lIndex < iBuildingsThatNeedToBeCaptured->GetCount(); lIndex++)
	{
		if(!iBuildingsThatNeedToBeCaptured->Get(lIndex)->GetBuildingGotCaptured())
		{
			return false; //not possible anymore
		}
	}

	//will only be exectuted if all have been captured. Start decreasing the timer
	if(iFrameDelay < 0)
	{
		return true;
	}
	else
	{
		iFrameDelay--;
		return false;
	}

}

void CSpawnJapaneseSquadIfBuildingGetsCaptured::AddBuilding(CBuilding* aBuildingObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aBuildingObject);
}

void CSpawnJapaneseSquadIfBuildingGetsCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iSquadType);
	aOutputStream.WriteInt32(iFrameDelay);
	aOutputStream.WriteInt32(iOrder.ShouldProtectArea);
	aOutputStream.WriteInt32(iOrder.DefaultXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.LeftXPosition);
	aOutputStream.WriteIntFloat(iOrder.ProtectionArea.RightXPosition);
	aOutputStream.WriteInt32(iOrder.MoveToLocation);
	aOutputStream.WriteInt32(iOrder.XLocationDestination);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CSpawnJapaneseSquadIfBuildingGetsCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iSquadType = static_cast<TJapaneseSquadTypes> (aReadStream.ReadInt32());
	iFrameDelay = aReadStream.ReadInt32();
	iOrder.ShouldProtectArea = aReadStream.ReadInt32();
	iOrder.DefaultXPosition = aReadStream.ReadInt32();
	iOrder.ProtectionArea.LeftXPosition = aReadStream.ReadIntFloat();
	iOrder.ProtectionArea.RightXPosition = aReadStream.ReadIntFloat();
	iOrder.MoveToLocation = aReadStream.ReadInt32();
	iOrder.XLocationDestination = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}
