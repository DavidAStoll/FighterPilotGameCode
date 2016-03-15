/*
 ============================================================================
 Name		: WinIfObjectsAreDestroyedEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWinIfObjectsAreDestroyedEvent implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Events/CWinIfObjectsAreDestroyedEvent.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CWinIfObjectsAreDestroyedEvent::CWinIfObjectsAreDestroyedEvent(const char* aStringId, TInt aBonusScore) :
	CEvent(EEventIdentifierWinIfObjectsAreDestroyedEvent)
{
	iBonusScore = aBonusScore;
	iStringId = new CString(aStringId);
	iObjectLeftMap = false;
	iGameObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}

CWinIfObjectsAreDestroyedEvent::CWinIfObjectsAreDestroyedEvent() :
	CEvent(EEventIdentifierWinIfObjectsAreDestroyedEvent)
{
	iStringId = new CString("");
	iGameObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}

CWinIfObjectsAreDestroyedEvent::~CWinIfObjectsAreDestroyedEvent()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iGameObjectsThatNeedToBeDestroyed != NULL)
	{
		delete iGameObjectsThatNeedToBeDestroyed;
		iGameObjectsThatNeedToBeDestroyed = NULL;
	}
	delete iStringId;
}
//------------------- functions -----------------------------//


void CWinIfObjectsAreDestroyedEvent::TriggerEvent()
{
	//set game state to won
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
	//get points for completing the goal
	SBonusObjective lExtraObjective;
	CString* lStringBonusName = CGame::Game->iLocalizationEngine->GetString(iStringId->GetData());
	lExtraObjective.ExtraScore = iBonusScore;
	lExtraObjective.NameOfObjective = lStringBonusName;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
}

TBool CWinIfObjectsAreDestroyedEvent::CheckIfEventConditionIsMet()
{
	//go through all objects in the array and check if they have all been destroyed and if mission is not failed yet
	if(!iObjectLeftMap && CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		TBool lAllObjectsDied = true;
		//mission can still be won, check if all objects have been destroyed that need to be destroyed
		for(TInt lCurrentObject = 0; lCurrentObject < iGameObjectsThatNeedToBeDestroyed->GetCount(); lCurrentObject++)
		{
			if(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->GetIsMoveableGameObject() && static_cast<CMoveableGameObject*>(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject))->GetIsLeavingMap())
			{
				iObjectLeftMap = true; //can't come true anymore
				lAllObjectsDied = false;
			}
			else if(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->IsAlive())
			{
				lAllObjectsDied = false;
			}
			else
			{
				//all good, object has died and we can remove it
				iGameObjectsThatNeedToBeDestroyed->Remove(lCurrentObject);
				lCurrentObject--;//count decreased by one
			}
		}
		return lAllObjectsDied;
	}
	return false; //Mission status has already been set, can't change it now
}

void CWinIfObjectsAreDestroyedEvent::AddGameObject(CGameObject* aGameObject)
{
	iGameObjectsThatNeedToBeDestroyed->Append(aGameObject);
}

void CWinIfObjectsAreDestroyedEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);
	aOutputStream.WriteInt32(iObjectLeftMap);

	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aOutputStream);
}

void CWinIfObjectsAreDestroyedEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);
	iObjectLeftMap = aReadStream.ReadInt32();

	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aReadStream);
}
