/*
 ============================================================================
 Name		: WeaponsTutorialAttackFighter.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackFighter implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackFighter.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialAttackFighter::CWeaponsTutorialAttackFighter(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierAttackFighter, aAircraft, WEAPONS_TUTORIAL_ATTACK_BOMBER_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
}

CWeaponsTutorialAttackFighter::~CWeaponsTutorialAttackFighter()
{
}

TBool CWeaponsTutorialAttackFighter::TutorialEventLogic() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierVal) //quick fix, uses Japanese dauntless
		{
			if(static_cast<CAircraft*> (lCurrentGameObject)->AirplaneCrashed())
				return true;
		}
	}
	
	return false;
}


