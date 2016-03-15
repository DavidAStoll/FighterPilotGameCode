/*
 ============================================================================
 Name		: LoseIfObjectsAreDestroyedEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CLoseIfObjectsAreDestroyedEvent implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CLoseIfObjectsAreDestroyedEvent.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/common/CSaveGameObject.h"

CLoseIfObjectsAreDestroyedEvent::CLoseIfObjectsAreDestroyedEvent() :
	CEvent(EEventIdentifierLoseIfObjectsAreDestroyedEvent)
{
	iGameObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}

CLoseIfObjectsAreDestroyedEvent::~CLoseIfObjectsAreDestroyedEvent()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iGameObjectsThatNeedToBeDestroyed != NULL)
	{
		delete iGameObjectsThatNeedToBeDestroyed;
		iGameObjectsThatNeedToBeDestroyed = NULL;
	}
}

CLoseIfObjectsAreDestroyedEvent* CLoseIfObjectsAreDestroyedEvent::New()
{
	CLoseIfObjectsAreDestroyedEvent* self = new CLoseIfObjectsAreDestroyedEvent();
	return self;
}

void CLoseIfObjectsAreDestroyedEvent::TriggerEvent()
{
	//set game state to lost
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusFailed);
}

TBool CLoseIfObjectsAreDestroyedEvent::CheckIfEventConditionIsMet()
{
	//go through all objects in the array and check if they have all been destroyed and if mission is still undecided
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		//mission can still be won, check if all objects have been destroyed that need to be destroyed
		for(TInt lCurrentObject = 0; lCurrentObject < iGameObjectsThatNeedToBeDestroyed->GetCount(); lCurrentObject++)
		{
			if(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->IsAlive())
				return false; //if any object is still alive return that the condition is not met
		}
		//check all objets, all of them are destroyed, so we can return true
		return true;
	}
	return false; //Mission status has already been set, can't change it now
}

void CLoseIfObjectsAreDestroyedEvent::AddGameObject(CGameObject* aGameObject)
{
	iGameObjectsThatNeedToBeDestroyed->Append(aGameObject);
}

void CLoseIfObjectsAreDestroyedEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aOutputStream);
}

void CLoseIfObjectsAreDestroyedEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aReadStream);
}
