/*
 ============================================================================
 Name		: SpawnDauntlessEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnDauntlessEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSpawnDauntlessEveryTimeInterval::CSpawnDauntlessEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aSpawnOfRightSide)
		: CTimeEvent(EEventIdentifierSpawnDauntlessEveryTimerInterval, aPeriodInSeconds, aInitialExtraDelayInSec, aNumberOfRepeats)
	{
		iTargetPriority = aTargetPriority;
		iHeightOffset = aHeightOffset;
		iSpawnOnRightSide = aSpawnOfRightSide;
	}

CSpawnDauntlessEveryTimeInterval::~CSpawnDauntlessEveryTimeInterval()
	{
	}

void CSpawnDauntlessEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation;
		if(iSpawnOnRightSide)
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(DAUNTLESS_WIDTH);
		else
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(DAUNTLESS_WIDTH);
		CDauntless* lDauntless = CDauntless::New(false, TIntFloat::Convert(FIGHTER_AI_SLOW_DOWN_SPEED_FOR_LANDING), true, TPoint(lXSpawningLocation, DAUNTLESS_SPAWNING_HEIGHT + iHeightOffset));
		if(!lDauntless->GetFighterAI())
			lDauntless->SetAIControlled();
		lDauntless->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo); //can attack anything with it, so it it is save to use
		lDauntless->GetFighterAI()->SetTargetAttackType(iTargetPriority);
		lDauntless->GetFighterAI()->SetFighterAIToRetreatToBase(); //either plane flies to the closest base or it attacks the closest target
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lDauntless);
	}
}

void CSpawnDauntlessEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iTargetPriority);
	aOutputStream.WriteInt32(iHeightOffset);
	aOutputStream.WriteInt32(iSpawnOnRightSide);
}

void CSpawnDauntlessEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iTargetPriority = aReadStream.ReadInt32();
	iHeightOffset = aReadStream.ReadInt32();
	iSpawnOnRightSide = aReadStream.ReadInt32();
}

