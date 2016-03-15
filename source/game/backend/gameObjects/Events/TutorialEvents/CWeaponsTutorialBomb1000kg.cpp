/*
 ============================================================================
 Name		: WeaponsTutorialBomb1000kg.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialBomb1000kg implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialBomb1000kg.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialBomb1000kg::CWeaponsTutorialBomb1000kg(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierBomb1000Kg,aAircraft, WEAPONS_TUTORIAL_BOMBS_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
}

CWeaponsTutorialBomb1000kg::~CWeaponsTutorialBomb1000kg()
{
}

TBool CWeaponsTutorialBomb1000kg::TutorialEventLogic() //checks if the Event Condition has been met
{
	if(iAircraft->GetSecondaryWeapon()->GetAmmunitionLeft() == 0)
		return true;
	else
		return false;
}
