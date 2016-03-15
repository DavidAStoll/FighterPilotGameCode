/*
 ============================================================================
 Name		: SpawnValIfBuildingGetsCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnValIfBuildingGetsCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CSpawnValIfBuildingGetsCaptured.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"

CSpawnValIfBuildingGetsCaptured::CSpawnValIfBuildingGetsCaptured(TInt aSecDelay, TInt aExtraHeightOffset, TWeaponIdentifier aWeaponIdentifier, TUint aTargetPriority) :
	CEvent(EEventIdentifierSpawnValIfBuildingGetsCaptured)
{
	iExtraHeightOffset = aExtraHeightOffset;
	iWeaponType = aWeaponIdentifier;
	iFrameDelay = aSecDelay * GFRAMES_PER_SECOND_BACKEND;
	iTargetPriority = aTargetPriority;
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CSpawnValIfBuildingGetsCaptured::~CSpawnValIfBuildingGetsCaptured()
{
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
}

void CSpawnValIfBuildingGetsCaptured::TriggerEvent()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(VAL_WIDTH);
		CVal* lVal = CVal::New(true, TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(lXSpawningLocation, BUILDING_GETS_CAPTURED_VAL_SPAWNING_HEIGHT + iExtraHeightOffset));
		if(!lVal->GetFighterAI())
			lVal->SetAIControlled();
		lVal->EquipWithNewSecondaryWeapon(iWeaponType); //can attack anything with it, so it it is save to use
		lVal->GetFighterAI()->SetTargetAttackType(iTargetPriority);
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase(); //either plane flies to the closest base or it attacks the closest target
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lVal);
	}
}

TBool CSpawnValIfBuildingGetsCaptured::CheckIfEventConditionIsMet()
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

void CSpawnValIfBuildingGetsCaptured::AddBuilding(CBuilding* aBuildingObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aBuildingObject);
}

void CSpawnValIfBuildingGetsCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iFrameDelay);
	aOutputStream.WriteInt32(iExtraHeightOffset);
	aOutputStream.WriteInt32(iWeaponType);
	aOutputStream.WriteInt32(iTargetPriority);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CSpawnValIfBuildingGetsCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iFrameDelay = aReadStream.ReadInt32();
	iExtraHeightOffset = aReadStream.ReadInt32();
	iWeaponType = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());
	iTargetPriority = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}
