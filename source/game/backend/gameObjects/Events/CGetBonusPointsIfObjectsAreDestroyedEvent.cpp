/*
 ============================================================================
 Name		: GetBonusPointsIfObjectsAreDestroyedEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGetBonusPointsIfObjectsAreDestroyedEvent implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Events/CGetBonusPointsIfObjectsAreDestroyedEvent.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CGetBonusPointsIfObjectsAreDestroyedEvent::CGetBonusPointsIfObjectsAreDestroyedEvent(const char* aStringId, TInt aBonusScore) :
	CEvent(EEventIdentifierGetBonusPointsIfObjectsAreDestroyed)
{
	iBonusScore = aBonusScore;
	iStringId = new CString(aStringId);
	iObjectLeftMap = false;
	iGameObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}

CGetBonusPointsIfObjectsAreDestroyedEvent::CGetBonusPointsIfObjectsAreDestroyedEvent() :
	CEvent(EEventIdentifierGetBonusPointsIfObjectsAreDestroyed)
{
	iStringId = new CString("");
	iGameObjectsThatNeedToBeDestroyed = CPointerArray<CGameObject>::New();
}


CGetBonusPointsIfObjectsAreDestroyedEvent::~CGetBonusPointsIfObjectsAreDestroyedEvent()
{
	if(iGameObjectsThatNeedToBeDestroyed != NULL)
	{
		delete iGameObjectsThatNeedToBeDestroyed;
		iGameObjectsThatNeedToBeDestroyed = NULL;
	}
	delete iStringId;
}

//------------------- functions -----------------------------//


void CGetBonusPointsIfObjectsAreDestroyedEvent::TriggerEvent()
{
	SBonusObjective lExtraObjective;
	CString* lStringBonusName = CGame::Game->iLocalizationEngine->GetString(iStringId->GetData());
	lExtraObjective.ExtraScore = iBonusScore;
	lExtraObjective.NameOfObjective = lStringBonusName;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
}

TBool CGetBonusPointsIfObjectsAreDestroyedEvent::CheckIfEventConditionIsMet()
{
	if(!iObjectLeftMap)
	{
		TBool lAllObjectsDied = true;
		for(TInt lCurrentObject = 0; lCurrentObject < iGameObjectsThatNeedToBeDestroyed->GetCount(); lCurrentObject++)
		{
			if(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->GetIsMoveableGameObject() && static_cast<CMoveableGameObject*> (iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject))->GetIsLeavingMap())
			{
				iObjectLeftMap = true; //can't come true anymore
				lAllObjectsDied = false;
			}else if(iGameObjectsThatNeedToBeDestroyed->Get(lCurrentObject)->IsAlive())
			{
				lAllObjectsDied = false;
			}else
			{
				//all good, object has died and we can remove it
				iGameObjectsThatNeedToBeDestroyed->Remove(lCurrentObject);
				lCurrentObject--;//count decreased by one
			}
		}
		return lAllObjectsDied;
	}
	//at least one object got away
	return false;
}

void CGetBonusPointsIfObjectsAreDestroyedEvent::AddGameObject(CGameObject* aGameObject)
{
	iGameObjectsThatNeedToBeDestroyed->Append(aGameObject);
}

void CGetBonusPointsIfObjectsAreDestroyedEvent::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);
	aOutputStream.WriteInt32(iObjectLeftMap);

	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aOutputStream);
}

void CGetBonusPointsIfObjectsAreDestroyedEvent::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);
	iObjectLeftMap = aReadStream.ReadInt32();

	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToBeDestroyed, aReadStream);
}
