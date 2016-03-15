/*
 ============================================================================
 Name		: WeaponsTutorialAttackBomber.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialAttackBomber implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialAttackBomber.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialAttackBomber::CWeaponsTutorialAttackBomber(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierAttackBomber,aAircraft, WEAPONS_TUTORIAL_ATTACK_BOMBER_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
}

CWeaponsTutorialAttackBomber::~CWeaponsTutorialAttackBomber()
{
}

TBool CWeaponsTutorialAttackBomber::TutorialEventLogic() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>* lBigPlanes = lObjectManager->GetGameObjectsByType(EObjectTypesBigPlane);

	for(TInt lIndex = 0; lIndex < lBigPlanes->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lBigPlanes->Get(lIndex);
		if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifierBetty) //betty got killed
		{
			if(static_cast<CAircraft*> (lCurrentGameObject)->AirplaneCrashed())
			{
				//clean up
				delete lBigPlanes;

				return true;
			}
		}
	}
	
	//clean up
	delete lBigPlanes;

	return false;
}
