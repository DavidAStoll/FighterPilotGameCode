/*
 * CWinIfAllJapaneseMoveableObjectsAreDestroyed.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: dstoll
 */

#include "includes/game/backend/gameObjects/Events/CWinIfAllJapaneseMoveableObjectsAreDestroyed.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWinIfAllJapaneseMoveableObjectsAreDestroyed::CWinIfAllJapaneseMoveableObjectsAreDestroyed(TInt aBonusScore)
:	CEvent(EEventIdentifierWinIfAllJapaneseMoveableObjectsAreDestroyed)
	{
		iBonusScore = aBonusScore;
	}

CWinIfAllJapaneseMoveableObjectsAreDestroyed::~CWinIfAllJapaneseMoveableObjectsAreDestroyed()
	{
	}

void CWinIfAllJapaneseMoveableObjectsAreDestroyed::TriggerEvent()
{
	//set game state to won
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusSuccess);
	//get points for completing the goal
	SBonusObjective lExtraObjective;
	lExtraObjective.ExtraScore = iBonusScore;
	CString* lStringDestroyAllJapaneseForces = CGame::Game->iLocalizationEngine->GetString("Frontend.BonusEvents.DestroyAllJapaneseForces");
	lExtraObjective.NameOfObjective = lStringDestroyAllJapaneseForces;

	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
}

TBool CWinIfAllJapaneseMoveableObjectsAreDestroyed::CheckIfEventConditionIsMet()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CMoveableGameObject>*& lAllMoveableGameObjects = lObjectManager->GetAllMoveableGameObjects();

		for(TInt lIndex = 0; lIndex < lAllMoveableGameObjects->GetCount(); lIndex++)
		{
			CGameObject* lCurrentGameObject = lAllMoveableGameObjects->Get(lIndex);
			if(lCurrentGameObject->GetConflictSide() == EConflictSideJapanese && lCurrentGameObject->GetIsMoveableGameObject()
					&& lCurrentGameObject->GetGameObjectType() != EObjectTypesDecor)// since flag pole is decor
			{
				if(lCurrentGameObject->IsAlive())
					return false;
			}
		}
		return true;
	}
	return false;
}


void CWinIfAllJapaneseMoveableObjectsAreDestroyed::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
}

void CWinIfAllJapaneseMoveableObjectsAreDestroyed::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
}
