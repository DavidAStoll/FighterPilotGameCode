/*
 ============================================================================
 Name		: SpawnBettyIfBuildingGetsCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnBettyIfBuildingGetsCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CSpawnBettyIfBuildingGetsCaptured.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"

CSpawnBettyIfBuildingGetsCaptured::CSpawnBettyIfBuildingGetsCaptured(TInt aSecDelay, TInt aExtraHeightOffset) :
	CEvent(EEventIdentifierSpawnBettyIfBuildingGetsCaptured)
{
	iExtraHeightOffset = aExtraHeightOffset;
	iFrameDelay = aSecDelay * GFRAMES_PER_SECOND_BACKEND;
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CSpawnBettyIfBuildingGetsCaptured::~CSpawnBettyIfBuildingGetsCaptured()
{
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
}

void CSpawnBettyIfBuildingGetsCaptured::TriggerEvent()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(BETTY_WIDTH);
		CBetty* lBetty = CBetty::New(true, TIntFloat::Convert(BASIC_AIRCRAFT_MINIMUM_SPEED_TO_AVOID_FALLING), true, TPoint(lXSpawningLocation, COMSTATION_BOMBER_SPAWNING_HEIGHT + iExtraHeightOffset));
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lBetty);
	}
}

TBool CSpawnBettyIfBuildingGetsCaptured::CheckIfEventConditionIsMet()
{
	for(TInt lIndex = 0; lIndex < iBuildingsThatNeedToBeCaptured->GetCount(); lIndex++)
	{
		if(!iBuildingsThatNeedToBeCaptured->Get(lIndex)->GetBuildingGotCaptured())
		{
			return false; //not possible anymore,
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

void CSpawnBettyIfBuildingGetsCaptured::AddBuilding(CBuilding* aBuildingObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aBuildingObject);
}

void CSpawnBettyIfBuildingGetsCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iFrameDelay);
	aOutputStream.WriteInt32(iExtraHeightOffset);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CSpawnBettyIfBuildingGetsCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iFrameDelay = aReadStream.ReadInt32();
	iExtraHeightOffset = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}
