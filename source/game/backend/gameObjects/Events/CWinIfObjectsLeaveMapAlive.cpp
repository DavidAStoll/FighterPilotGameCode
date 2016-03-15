/*
 ============================================================================
 Name		: WinIfObjectsLeaveMapAlive.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWinIfObjectsLeaveMapAlive implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Events/CWinIfObjectsLeaveMapAlive.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/game/common/CSaveGameObject.h"

CWinIfObjectsLeaveMapAlive::CWinIfObjectsLeaveMapAlive(const char* aStringId, TInt aBonusScore, TInt aObjectsNeedToSurvive) :
	CEvent(EEventIdentifierWinIfObjectsLeaveMapAlive)
{
	iBonusScore = aBonusScore;
	iObjectsThatLeftMap = 0;
	iObjectsThatNeedToSurvive = aObjectsNeedToSurvive;
	iStringId = new CString(aStringId);
	iGameObjectsThatNeedToLeave = CPointerArray<CMoveableGameObject>::New();
}

CWinIfObjectsLeaveMapAlive::CWinIfObjectsLeaveMapAlive() :
	CEvent(EEventIdentifierWinIfObjectsLeaveMapAlive)
{
	iStringId = new CString("");
	iGameObjectsThatNeedToLeave = CPointerArray<CMoveableGameObject>::New();
}

CWinIfObjectsLeaveMapAlive::~CWinIfObjectsLeaveMapAlive()
{
	if(iStringId != NULL)
	{
		delete iStringId;
		iStringId = NULL;
	}
	if(iGameObjectsThatNeedToLeave != NULL)
	{
		delete iGameObjectsThatNeedToLeave;
		iGameObjectsThatNeedToLeave = NULL;
	}
}

//------------------- functions -----------------------------//


void CWinIfObjectsLeaveMapAlive::TriggerEvent()
{
	if(CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetMissionStatus() != EMissionStatusFailed)
	{
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->SetMissionStatus(EMissionStatusSuccess);

		//get points for completing the bonus objective
		SBonusObjective lExtraObjective;
		CString* lStringBonusName = CGame::Game->iLocalizationEngine->GetString(iStringId->GetData());
		lExtraObjective.ExtraScore = iBonusScore;
		lExtraObjective.NameOfObjective = lStringBonusName;
		CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->GetStatsTracker()->iBonusObjectives->Append(new SBonusObjective(lExtraObjective));
	}
}

TBool CWinIfObjectsLeaveMapAlive::CheckIfEventConditionIsMet()
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

void CWinIfObjectsLeaveMapAlive::AddGameObject(CMoveableGameObject* aGameObject)
{
	iGameObjectsThatNeedToLeave->Append(aGameObject);
}

void CWinIfObjectsLeaveMapAlive::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicEvenSaveOnDisk(aOutputStream);
	aOutputStream.WriteInt32(iObjectsThatLeftMap);
	aOutputStream.WriteInt32(iObjectsThatNeedToSurvive);
	aOutputStream.WriteInt32(iBonusScore);
	aOutputStream.WriteString(iStringId);

	CMap::SaveDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToLeave, aOutputStream);
}

void CWinIfObjectsLeaveMapAlive::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicEventLoadFromDisk(aReadStream);
	iObjectsThatLeftMap = aReadStream.ReadInt32();
	iObjectsThatNeedToSurvive = aReadStream.ReadInt32();
	iBonusScore = aReadStream.ReadInt32();
	aReadStream.ReadString(iStringId);

	CMap::LoadDynamicArray((CPointerArray<void>*) iGameObjectsThatNeedToLeave, aReadStream);
}
