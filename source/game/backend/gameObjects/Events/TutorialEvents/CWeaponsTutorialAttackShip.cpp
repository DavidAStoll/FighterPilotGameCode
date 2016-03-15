/*
 ============================================================================
 Name		: WeaponsTutorialAttackShip.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackShip implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackShip.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialAttackShip::CWeaponsTutorialAttackShip(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierAttackShip,aAircraft, WEAPONS_TUTORIAL_ATTACK_SHIP_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
}

CWeaponsTutorialAttackShip::~CWeaponsTutorialAttackShip()
{
}

TBool CWeaponsTutorialAttackShip::TutorialEventLogic() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierJapaneseLCT && !lCurrentGameObject->IsAlive())
		{
			return true;
		}
	}
	
	return false;
}
