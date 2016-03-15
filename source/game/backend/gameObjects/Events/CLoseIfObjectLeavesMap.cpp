/*
 ============================================================================
 Name		: LoseIfObjectLeavesMap.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CLoseIfObjectLeavesMap implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CLoseIfObjectLeavesMap.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/common/CSaveGameObject.h"

CLoseIfObjectLeavesMap::CLoseIfObjectLeavesMap(TInt aObjectsNeedToSurvive) :
	CEvent(EEventIdentifierLoseIfObjectsLeaveMapAlive)
{
	iObjectsThatLeftMap = 0;
	iObjectsThatNeedToSurvive = aObjectsNeedToSurvive;
	iGameObjectsThatNeedToLeave = CPointerArray<CMoveableGameObject>::New();
}

CLoseIfObjectLeavesMap::~CLoseIfObjectLeavesMap()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iGameObjectsThatNeedToLeave != NULL)
	{
		delete iGameObjectsThatNeedToLeave;
		iGameObjectsThatNeedToLeave = NULL;
	}
}

void CLoseIfObjectLeavesMap::TriggerEvent()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusFailed);
	}
}

TBool CLoseIfObjectLeavesMap::CheckIfEventConditionIsMet()
{
	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < iGameObjectsThatNeedToLeave->GetCount(); lCurrentObjectIndex++)
	{
		CMoveableGameObject* lCurrentObject = iGameObjectsThatNeedToLeave->Get(lCurrentObjectIndex);
		if(lCurrentObject->IsAlive())
		{
			if(lCurrentObject->GetIsLeavingMap())
			{
				iObjectsThatLeftMap++;
				iGameObjectsThatNeedToLeave->Remove(lCurrentObjectIndex);
				lCurrentObjectIndex--;//count decreased by one
			}
		}else
		{
			iGameObjectsThatNeedToLeave->Remove(lCurrentObjectIndex);
			lCurrentObjectIndex--;//count decreased by one
		}
	}

	if(iObjectsThatLeftMap >= iObjectsThatNeedToSurvive)
		return true;
	else
		return false;
}

void CLoseIfObjectLeavesMap::AddGameObject(CMoveableGameObject* aGameObject)
{
	iGameObjectsThatNeedToLeave->Append(aGameObject);
}

void CLoseIfObjectLeavesMap::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iObjectsThatLeftMap);
	aOutputStream.WriteInt32(iObjectsThatNeedToSurvive);

	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToLeave, aOutputStream);
}

void CLoseIfObjectLeavesMap::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iObjectsThatLeftMap = aReadStream.ReadInt32();
	iObjectsThatNeedToSurvive = aReadStream.ReadInt32();

	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToLeave, aReadStream);
}
