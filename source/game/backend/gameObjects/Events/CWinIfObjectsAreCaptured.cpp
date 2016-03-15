/*
 ============================================================================
 Name		: WinIfObjectsAreCaptured.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWinIfObjectsAreCaptured implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/CWinIfObjectsAreCaptured.h"
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/common/CSaveGameObject.h"

CWinIfObjectsAreCaptured::CWinIfObjectsAreCaptured(const char* aStringId, TInt aBonusScore) :
	CEvent(EEventIdentifierWinIfObjectsAreCaptured)
{
	iBonusScore = aBonusScore;
	iStringId = new CString(aStringId);
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CWinIfObjectsAreCaptured::CWinIfObjectsAreCaptured() :
	CEvent(EEventIdentifierWinIfObjectsAreCaptured)
{
	iStringId = new CString("");
	iBuildingsThatNeedToBeCaptured = CPointerArray<CBuilding>::New();
}

CWinIfObjectsAreCaptured::~CWinIfObjectsAreCaptured()
{
	//removes pointers but doesn't destroy actual objects, since those will be removed from somewhere else
	if(iBuildingsThatNeedToBeCaptured != NULL)
	{
		delete iBuildingsThatNeedToBeCaptured;
		iBuildingsThatNeedToBeCaptured = NULL;
	}
	delete iStringId;

}
//------------------- functions -----------------------------//

void CWinIfObjectsAreCaptured::TriggerEvent()
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

TBool CWinIfObjectsAreCaptured::CheckIfEventConditionIsMet()
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

void CWinIfObjectsAreCaptured::AddBuilding(CBuilding* aGameObject)
{
	iBuildingsThatNeedToBeCaptured->Append(aGameObject);
}

void CWinIfObjectsAreCaptured::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);
	CMap::SaveDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aOutputStream);
}

void CWinIfObjectsAreCaptured::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);
	CMap::LoadDynamicArray((CPointerArray<void>*) iBuildingsThatNeedToBeCaptured, aReadStream);
}

