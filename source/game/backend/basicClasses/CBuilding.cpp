/*
 ============================================================================
 Name		: Building.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CBuilding implementation
 ============================================================================
 */

#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/basicClasses/CBuilding.h"

CBuilding::CBuilding(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer,//inheritance
		TPoint aLocation, SGameObjectAttributes& aAttributes) :
	CGameObject(aZCor, aTextureObject, aAnimationPlayer, aLocation, aAttributes)
{
	iGotCaptured = false;
}

CBuilding::~CBuilding()
{
}

// ---------------------- functions --------------------------//

void CBuilding::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once

	BuildingBasicDieRoutine();
	GameObjectMoveIntoBackground();
}

void CBuilding::BuildingBasicDieRoutine()
{
	if(!iAlive)
		return; //can only die once

	//by default set object to dead
	iAlive = false;
	if(iGotCaptured) //object got captured but destroyed. Thus it is like it just got destroyed withouth being capture. The User shouldn't lose points if a he caputeres a building and it gets destroyed
	{
		if(iConflictSide == EConflictSideAmerican)
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectKilled(iGameObjectIdentifier, this);
		else
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectLost(iGameObjectIdentifier, this);
	}
	else
	{
		if(iConflictSide == EConflictSideAmerican)
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectLost(iGameObjectIdentifier, this);
		else
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectKilled(iGameObjectIdentifier, this);
	}
}

void CBuilding::UpdateStatsIfBuildingWasSuccessfullyCaptured()
{
	if(iAlive && iGotCaptured) //building was captured and is still Alive, thus we need update the score to reflect that
	{
		if(iConflictSide == EConflictSideAmerican) //means it was previously Japanese
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectKilled(iGameObjectIdentifier, this);
		else //means it was previously American, but is now Japanese. Mark it lost
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->UpdateScoreObjectLost(iGameObjectIdentifier, this);
	}
}

TBool CBuilding::GetBuildingGotCaptured()
{
	return iGotCaptured;
}

void CBuilding::SetBuildingGotCaptured(TBool aValue)
{
	iGotCaptured = aValue;
}

CGameObject* CBuilding::CheckIfObjectGotCaptured(CInterval*& aInterval)
{
	CGameObject* lCurrentGameObject = NULL;
	CGameObject* lNewConflictSideGameObject = NULL;
	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with alive objects
		if(lCurrentGameObject->IsAlive())//don't compare to anything if already dead
		{
			//collide
			if(iConflictSide == lCurrentGameObject->GetConflictSide() && iHitBox->GetFastIntersectionRect().Intersects((lCurrentGameObject->GetHitBox()->GetFastIntersectionRect())))
			{
				//all done here, we have an object that is of the same conflict side and thus can't be captured by the enemy
				return NULL;
			}
			else if(iConflictSide != lCurrentGameObject->GetConflictSide() && iHitBox->GetFastIntersectionRect().Intersects((lCurrentGameObject->GetHitBox()->GetFastIntersectionRect())))
			{
				iGotCaptured = !iGotCaptured;
				lNewConflictSideGameObject = lCurrentGameObject;
			}
		}
	}

	//clean up
	delete lGameObjects;

	return lNewConflictSideGameObject;
}

void CBuilding::BuildingSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	GameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iGotCaptured);
}

void CBuilding::BuildingLoadContentFromDisk(CFileReadStream &aReadStream)
{
	GameObjectLoadContentFromDisk(aReadStream);
	iGotCaptured = aReadStream.ReadUint32();
}

void CBuilding::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BuildingSaveContentToDisk(aOutputStream);
}

void CBuilding::LoadFromDisk(CFileReadStream &aReadStream)
{
	BuildingLoadContentFromDisk(aReadStream);
}
