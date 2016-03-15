/*
 ============================================================================
 Name		: SpawnValEveryTimeInterval.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CSpawnValEveryTimeInterval implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CSpawnValEveryTimeInterval::CSpawnValEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aHasNoAmmunitionLeft) :
CTimeEvent(EEventIdentifierSpawnValEveryTimerInterval, aPeriodInSeconds, aInitialExtraDelayInSec, aNumberOfRepeats)
	{
	iTargetPriority = aTargetPriority;
	iHeightOffset = 0;
	iSpawnOnLeftSide = true;
	iHasNoAmmunitionLeft = aHasNoAmmunitionLeft;

	}

CSpawnValEveryTimeInterval::CSpawnValEveryTimeInterval(TInt aPeriodInSeconds, TInt aInitialExtraDelayInSec, TInt aHeightOffset, TBool aSpawnOnLeftSide, TInt aNumberOfRepeats, TUint aTargetPriority, TBool aHasNoAmmunitionLeft) :
CTimeEvent(EEventIdentifierSpawnValEveryTimerInterval, aPeriodInSeconds, aInitialExtraDelayInSec, aNumberOfRepeats)
	{
	iTargetPriority = aTargetPriority;
	iHeightOffset = aHeightOffset;
	iSpawnOnLeftSide = aSpawnOnLeftSide;
	iHasNoAmmunitionLeft = aHasNoAmmunitionLeft;
	}

CSpawnValEveryTimeInterval::~CSpawnValEveryTimeInterval()
	{
	}

void CSpawnValEveryTimeInterval::Run()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TInt lXSpawningLocation;
		if(iSpawnOnLeftSide)
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetLeftSideSpawningAreaXCoordinate(VAL_WIDTH);
		else
			lXSpawningLocation= CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetRightSideSpawningAreaXCoordinate(VAL_WIDTH);
		CVal* lVal = CVal::New(iSpawnOnLeftSide, TIntFloat::Convert(FIGHTER_AI_MAX_ENGINE_SPEED), true, TPoint(lXSpawningLocation, VAL_SPAWNING_HEIGHT + iHeightOffset));
		if(!lVal->GetFighterAI())
			lVal->SetAIControlled();
		lVal->EquipWithNewSecondaryWeapon(EWeaponIdentifierTorpedo); //can attack anything with it, so it it is save to use
		if(iHasNoAmmunitionLeft)
			lVal->GetSecondaryWeapon()->SetAmmunitionToValue(0);
		lVal->GetFighterAI()->SetTargetAttackType(iTargetPriority);
		lVal->GetFighterAI()->SetFighterAIToRetreatToBase(); //either plane flies to the closest base or it attacks the closest target
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lVal);
	}
}

void CSpawnValEveryTimeInterval::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	TimeEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iTargetPriority);
	aOutputStream.WriteInt32(iHeightOffset);
	aOutputStream.WriteInt32(iSpawnOnLeftSide);
	aOutputStream.WriteInt32(iHasNoAmmunitionLeft);
}

void CSpawnValEveryTimeInterval::LoadFromDisk(CFileReadStream &aReadStream)
{
	TimeEventLoadFromDisk(aReadStream);
	iTargetPriority = aReadStream.ReadInt32();
	iHeightOffset = aReadStream.ReadInt32();
	iSpawnOnLeftSide = aReadStream.ReadInt32();
	iHasNoAmmunitionLeft = aReadStream.ReadInt32();
}
