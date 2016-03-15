/*
 ============================================================================
 Name		: SpawnDauntlessIfBuildingGetsCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnDauntlessIfBuildingGetsCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CSpawnDauntlessIfBuildingGetsCaptured.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/common/CGameVariables.h"
#include "includes/game/common/CSaveGameObject.h"

CSpawnDauntlessIfBuildingGetsCaptured::CSpawnDauntlessIfBuildingGetsCaptured(TInt aSecDelay, TInt aExtraHeightOffset, TWeaponIdentifier aWeaponIdentifier, TUint aTargetPriority) :
	CEvent(EEventIdentifierSpawnDauntlessIfBuildingGetsCaptured)
{
	iExtraHeightOffset = aExtraHeightOffset;
	iWeaponType = aWeaponIdentifier;
	iFrameDelay = aSecDelay * GFRAMES_PER_SECOND_BACKEND;
	iTargetPriority = aTargetPriority;
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CSpawnDauntlessIfBuildingGetsCaptured::~CSpawnDauntlessIfBuildingGetsCaptured()
{
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
}

void CSpawnDauntlessIfBuildingGetsCaptured::TriggerEvent()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(DAUNTLESS_WIDTH);
		CDauntless* lDauntless = CDauntless::New(true, TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING), true, TPoint(lXSpawningLocation, BUILDING_GETS_CAPTURED_DAUNTLESS_SPAWNING_HEIGHT + iExtraHeightOffset));
		if(!lDauntless->GetFighterAI())
			lDauntless->SetAIControlled();
		lDauntless->EquipWithNewSecondaryWeapon(iWeaponType); //can attack anything with it, so it it is save to use
		lDauntless->GetFighterAI()->SetTargetAttackType(iTargetPriority);
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase(); //either plane flies to the closest base or it attacks the closest target
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lDauntless);
	}
}

TBool CSpawnDauntlessIfBuildingGetsCaptured::CheckIfEventConditionIsMet()
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

void CSpawnDauntlessIfBuildingGetsCaptured::AddBuilding(CBuilding* aBuildingObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aBuildingObject);
}

void CSpawnDauntlessIfBuildingGetsCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iFrameDelay);
	aOutputStream.WriteInt32(iExtraHeightOffset);
	aOutputStream.WriteInt32(iWeaponType);
	aOutputStream.WriteInt32(iTargetPriority);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CSpawnDauntlessIfBuildingGetsCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iFrameDelay = aReadStream.ReadInt32();
	iExtraHeightOffset = aReadStream.ReadInt32();
	iWeaponType = static_cast<TWeaponIdentifier> (aReadStream.ReadInt32());
	iTargetPriority = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}
