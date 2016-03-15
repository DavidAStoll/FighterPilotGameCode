/*
 ============================================================================
 Name		: GetBonusPointsIfObjectsSurvive.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGetBonusPointsIfObjectsSurvive implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Events/CGetBonusPointsIfObjectsSurvive.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CGetBonusPointsIfObjectsSurvive::CGetBonusPointsIfObjectsSurvive(const char* aStringId, TInt aBonusScore) :
	CEvent(EEventIdentifierGetBonusPointsIfObjectsSurvive)
{
	iBonusScore = aBonusScore;
	iStringId = new CString(aStringId);
	iSuccess = true;
	iGameObjectsThatNeedToSurvive = CPointerArray<CGameObject>::New();
}

CGetBonusPointsIfObjectsSurvive::CGetBonusPointsIfObjectsSurvive() :
	CEvent(EEventIdentifierGetBonusPointsIfObjectsSurvive)
{
	iStringId = new CString("");
	iGameObjectsThatNeedToSurvive = CPointerArray<CGameObject>::New();
}

CGetBonusPointsIfObjectsSurvive::~CGetBonusPointsIfObjectsSurvive()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iGameObjectsThatNeedToSurvive != NULL)
	{
		delete iGameObjectsThatNeedToSurvive;
		iGameObjectsThatNeedToSurvive = NULL;
	}
	delete iStringId;
}

//------------------- functions -----------------------------//


void CGetBonusPointsIfObjectsSurvive::TriggerEvent()
{
	SBonusObjective lExtraObjective;
	CString* lStringBonusName = CGame::Game->iLocalizationEngine->GetString(iStringId->GetData());
	lExtraObjective.ExtraScore = iBonusScore;
	lExtraObjective.NameOfObjective = lStringBonusName;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
}

TBool CGetBonusPointsIfObjectsSurvive::CheckIfEventConditionIsMet()
{
	//go through all objects in the array and check if they have all been destroyed and if mission is not failed yet
	if(iSuccess) //only check if all objects are still around, otherwise game might crash if one died a long time ago and got recycled
	{
		for(TInt lIndex = 0; lIndex < iGameObjectsThatNeedToSurvive->GetCount(); lIndex++)
		{
			if(!iGameObjectsThatNeedToSurvive->Get(lIndex)->IsAlive()) //NOT alive anymore
			{
				iSuccess = false; //not possible anymore
			}
		}
	}

	//mission can still be won, check if all objects have been destroyed that need to be destroyed
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusSuccess)
	{
		return iSuccess;
	}
	else
		return false; //condition has not been met yet
}

void CGetBonusPointsIfObjectsSurvive::AddGameObject(CGameObject* aGameObject)
{
	iGameObjectsThatNeedToSurvive->Append(aGameObject);
}

void CGetBonusPointsIfObjectsSurvive::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);
	aOutputStream.WriteInt32(iSuccess);
	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToSurvive, aOutputStream);
}

void CGetBonusPointsIfObjectsSurvive::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);
	iSuccess = aReadStream.ReadInt32();
	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToSurvive, aReadStream);
}
