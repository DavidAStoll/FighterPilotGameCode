/*
 ============================================================================
 Name		: GetBonusPointsIfLessTroopsGetKilled.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGetBonusPointsIfLessTroopsGetKilled implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CGetBonusPointsIfLessTroopsGetKilled.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CGetBonusPointsIfLessTroopsGetKilled::CGetBonusPointsIfLessTroopsGetKilled(const char* aStringId, TInt aBonusScore, TInt aMaxSoldiersKilled, TInt aMaxTanksDestroyed)
: CEvent(EEventIdentifierGetBonusPointsIfLessTroopsGetKilled)
	{
	iStringId = new CString(aStringId);
	iBonusScore = aBonusScore;
	iMaxSoldierKilled = aMaxSoldiersKilled;
	iMaxTanksDestroyed = aMaxTanksDestroyed;
	}

CGetBonusPointsIfLessTroopsGetKilled::CGetBonusPointsIfLessTroopsGetKilled()
: CEvent(EEventIdentifierGetBonusPointsIfLessTroopsGetKilled)
	{
	iStringId = new CString("");
	}

CGetBonusPointsIfLessTroopsGetKilled::~CGetBonusPointsIfLessTroopsGetKilled()
	{
		if(iStringId != NULL)
		{
			delete iStringId;
			iStringId = NULL;
		}
	}


//------------------- functions -----------------------------//


void CGetBonusPointsIfLessTroopsGetKilled::TriggerEvent()
{
	//get points for completing the bonus objective
	SBonusObjective lExtraObjective;
	CString* lStringBonusName = CGame::Game->iLocalizationEngine->GetString(iStringId->GetData());
	lExtraObjective.ExtraScore = iBonusScore;
	lExtraObjective.NameOfObjective = lStringBonusName;
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
}

TBool CGetBonusPointsIfLessTroopsGetKilled::CheckIfEventConditionIsMet()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() == EMissionStatusSuccess)
	{
		CStatsTracker* lStats = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker();
		TInt lSoldiersLost = lStats->iNumberOfRiflemanLost + lStats->iNumberOfAntiTankmanLost;
		TInt lTanksLost = lStats->iNumberOfAATanksLost + lStats->iNumberOfMediumTanksLost;
		
		if(lSoldiersLost <= iMaxSoldierKilled && lTanksLost <= iMaxTanksDestroyed)
			return true;
		else
			return false;
	}
	return false; //condition has not been met yet
}

void CGetBonusPointsIfLessTroopsGetKilled::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);
	aOutputStream.WriteInt32(iMaxSoldierKilled);
	aOutputStream.WriteInt32(iMaxTanksDestroyed);
}

void CGetBonusPointsIfLessTroopsGetKilled::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);
	iMaxSoldierKilled = aReadStream.ReadInt32();
	iMaxTanksDestroyed = aReadStream.ReadInt32();
}
