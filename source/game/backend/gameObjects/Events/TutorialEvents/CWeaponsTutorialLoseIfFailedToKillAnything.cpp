/*
 ============================================================================
 Name		: WeaponsTutorialLoseIfFailedToKillAnything.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialLoseIfFailedToKillAnything implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialLoseIfFailedToKillAnything.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialLoseIfFailedToKillAnything::CWeaponsTutorialLoseIfFailedToKillAnything(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierLoseIfFailedToKill,aAircraft, TUTORIAL_DIE_EVENT_SECS_UNTIL_CALL, aMethodCallIfSuccess)
{
}

CWeaponsTutorialLoseIfFailedToKillAnything::~CWeaponsTutorialLoseIfFailedToKillAnything()
{
}

TBool CWeaponsTutorialLoseIfFailedToKillAnything::TutorialEventLogic() //checks if the Event Condition has been met
{
	TBool lAllObjectsAreStillAlive = true;
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
		if(!lCurrentGameObject->IsAlive()) //one object died
		{
			lAllObjectsAreStillAlive = false;
		}
		if(lCurrentGameObject->GetGameObjectType() == EObjectTypesProjectileExplosive) // a bomb is still flying around
			return false;
	}
	
	if(iAircraft->GetSecondaryWeapon()->GetAmmunitionLeft() == 0 && lAllObjectsAreStillAlive)//no more bombs
	{
		return true;//player failed to kill anything, restart the tutorial section
	}
	
	return false;
}
