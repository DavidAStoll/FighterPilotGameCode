/*
 ============================================================================
 Name		: WinIfAllJapaneseForcesAreDestroyed.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWinIfAllJapaneseForcesAreDestroyed implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CWinIfAllJapaneseForcesAreDestroyed.h"
#include "includes/game/common/CSaveGameObject.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWinIfAllJapaneseForcesAreDestroyed::CWinIfAllJapaneseForcesAreDestroyed(TInt aBonusScore)
:	CEvent(EEventIdentifierWinIfAllJapaneseForcesAreDestroyed)
	{
		iBonusScore = aBonusScore;
	}

CWinIfAllJapaneseForcesAreDestroyed::~CWinIfAllJapaneseForcesAreDestroyed()
	{
	}

void CWinIfAllJapaneseForcesAreDestroyed::TriggerEvent()
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

TBool CWinIfAllJapaneseForcesAreDestroyed::CheckIfEventConditionIsMet()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusUndecided)
	{
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>*& lAllGameObjects = lObjectManager->GetAllGameObjects();

		for(TInt lIndex = 0; lIndex < lAllGameObjects->GetCount(); lIndex++)
		{
			CGameObject* lCurrentGameObject =lAllGameObjects->Get(lIndex);
			if(lCurrentGameObject->GetConflictSide() == EConflictSideJapanese && lCurrentGameObject->GetGameObjectType() != EObjectTypesDecor)// since flag pole is decor
			{
				if(lCurrentGameObject->IsAlive())
					return false;
			}
		}
		return true;
	}
	return false;
}


void CWinIfAllJapaneseForcesAreDestroyed::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
}

void CWinIfAllJapaneseForcesAreDestroyed::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
}

