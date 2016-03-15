/*
 ============================================================================
 Name		: LoseIfObjectsAreCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CLoseIfObjectsAreCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CLoseIfObjectsAreCaptured.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CLoseIfObjectsAreCaptured::CLoseIfObjectsAreCaptured() :
	CEvent(EEventIdentifierLoseIfObjectsAreCaptured)
{
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CLoseIfObjectsAreCaptured::~CLoseIfObjectsAreCaptured()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
}
//------------------- functions -----------------------------//

void CLoseIfObjectsAreCaptured::TriggerEvent()
{
	//set game state to lost
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusFailed);
}

TBool CLoseIfObjectsAreCaptured::CheckIfEventConditionIsMet()
{
	//go through all objects in the array and check if they have all been destroyed and if mission is not failed yet
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		//mission can still be won, check if all objects have been destroyed that need to be destroyed
		for(TInt lCurrentObject = 0; lCurrentObject < iBuildingsThatNeedToBeCaptured->GetCount(); lCurrentObject++)
		{
			if(!iBuildingsThatNeedToBeCaptured->Get(lCurrentObject)->GetBuildingGotCaptured())
				return false; //if any object has not been captured yet
		}
		//check all objets, all of them are captured, so we can return true
		return true;
	}
	return false; //Mission status has already been set, can't change it now
}

void CLoseIfObjectsAreCaptured::AddBuilding(CBuilding* aGameObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aGameObject);
}

void CLoseIfObjectsAreCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CLoseIfObjectsAreCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}
