/*
 ============================================================================
 Name		: WeaponsTutorialLoseIfAllAmericanSoldiersDied.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialLoseIfAllAmericanSoldiersDied implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialLoseIfAllAmericanSoldiersDied.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialLoseIfAllAmericanSoldiersDied::CWeaponsTutorialLoseIfAllAmericanSoldiersDied(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierLoseIfAllAmericanSoldiersDied,aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
{
}

CWeaponsTutorialLoseIfAllAmericanSoldiersDied::~CWeaponsTutorialLoseIfAllAmericanSoldiersDied()
{
}

TBool CWeaponsTutorialLoseIfAllAmericanSoldiersDied::TutorialEventLogic() //checks if the Event Condition has been met
{
	CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
	CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

	for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
	{
		CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
		if(lCurrentGameObject->GetConflictSide() == EConflictSideAmerican && lCurrentGameObject->GetGameObjectType() == EObjectTypesGroundUnit)
		{
			if(lCurrentGameObject->IsAlive())
				return false;
		}
	}
	
	return true; //all American soldiers on the map died
}
