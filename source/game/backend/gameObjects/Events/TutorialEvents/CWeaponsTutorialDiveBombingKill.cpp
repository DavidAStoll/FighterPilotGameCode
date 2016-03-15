/*
 ============================================================================
 Name		: WeaponsTutorialDiveBombingKill.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialDiveBombingKill implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialDiveBombingKill.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialDiveBombingKill::CWeaponsTutorialDiveBombingKill(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess, TTutorialMethodCallIds aMethodCallIfFailed) :
CBasicAircraftNeededTutorialClass(EEventIdentifierDiveBombingKill,aAircraft, TUTORIAL_DIE_EVENT_SECS_UNTIL_CALL, aMethodCallIfSuccess)
{
	iBombReleasedDuringDive = false;
	iSuccess = false;
	iIfFailed = aMethodCallIfFailed;
}

CWeaponsTutorialDiveBombingKill::~CWeaponsTutorialDiveBombingKill()
{
}

TBool CWeaponsTutorialDiveBombingKill::TutorialEventLogic()
{
	//not needed
	return false;
}

TBool CWeaponsTutorialDiveBombingKill::CheckIfEventConditionIsMet() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);

		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifier1000kgBomb) // a bomb is still flying around
		{
			CMoveableGameObject* lBomb = static_cast<CMoveableGameObject*> (lCurrentGameObject);
			if(lBomb->GetAngle() == 270)
				iBombReleasedDuringDive = true;
		}
		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierJapanese97ChiHaTank)
		{
			if(!lCurrentGameObject->IsAlive())
			{
				iReadyToMakeMethodCall = true; //tank has been destroyed nothing more the user can do
				if(iBombReleasedDuringDive)
					iSuccess = true; // must have been be hit by a dive bomb
			}
		}
	}
	
	if(iReadyToMakeMethodCall)
	{
		iCurrentFrame++;
		if(iCurrentFrame >= iFramesUntilMethodCall)
		{
			if(iSuccess)
				((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iMethodCall);
			else
				((CBasicTutorialMap*) CFighterPilotThePacificWar::FighterGame->iGameData->GetMap())->TutorialMethodCall(iIfFailed); //better ending, if player speed up again and saved the plane
		}
	}
		
	return false;
}

void CWeaponsTutorialDiveBombingKill::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicAircraftNeededTutorialSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iBombReleasedDuringDive);
  aOutputStream.WriteInt32(iSuccess);
  aOutputStream.WriteInt32(iIfFailed);
}

void CWeaponsTutorialDiveBombingKill::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicAircraftNeededTutorialLoadFromDisk(aReadStream);
	iBombReleasedDuringDive = aReadStream.ReadInt32();
	iSuccess = aReadStream.ReadInt32();
	iIfFailed =  static_cast<TTutorialMethodCallIds>(aReadStream.ReadInt32());
}
