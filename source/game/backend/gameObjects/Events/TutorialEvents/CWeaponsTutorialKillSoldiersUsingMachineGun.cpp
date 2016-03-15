/*
 ============================================================================
 Name		: WeaponsTutorialKillSoldiersUsingMachineGun.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialKillSoldiersUsingMachineGun implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialKillSoldiersUsingMachineGun.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialKillSoldiersUsingMachineGun::CWeaponsTutorialKillSoldiersUsingMachineGun(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierKillSoldiersUsingMachineGun,aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
{
}

CWeaponsTutorialKillSoldiersUsingMachineGun::~CWeaponsTutorialKillSoldiersUsingMachineGun()
{
}

TBool CWeaponsTutorialKillSoldiersUsingMachineGun::TutorialEventLogic() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
		if(lCurrentGameObject->GetConflictSide() == EConflictSideJapanese && lCurrentGameObject->GetGameObjectType() == EObjectTypesGroundUnit)
		{
			if(lCurrentGameObject->IsAlive())
				return false;
		}
	}
	
	return true; //all Japanese soldiers on the map died
}

