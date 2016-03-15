/*
 ============================================================================
 Name		: WeaponsTutorialFireButtons.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CWeaponsTutorialFireButtons implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CWeaponsTutorialFireButtons.h"
#include "includes/game/CFighterPilotThePacificWar.h"

CWeaponsTutorialFireButtons::CWeaponsTutorialFireButtons(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierFireButtons,aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
{
	iFire1ButtonPressed = false;
	iFire2ButtonPressed = false;
}

CWeaponsTutorialFireButtons::~CWeaponsTutorialFireButtons()
{
}

TBool CWeaponsTutorialFireButtons::TutorialEventLogic() //checks if the Event Condition has been met
{
	if(!iFire1ButtonPressed)
	{
		CGameObjectManager* lObjectManager = CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetObjectManager();
		CPointerArray<CGameObject>*& lObjects = lObjectManager->GetAllGameObjects();

		for(TInt lIndex = 0; lIndex < lObjects->GetCount(); lIndex++)
		{
			CGameObject* lCurrentGameObject = lObjects->Get(lIndex);
			if(lCurrentGameObject->GetGameObjectIdentifier() == EGameObjectIdentifier13mmBullet)
			{
				iFire1ButtonPressed = true;
			}
		}
	}
	if(!iFire2ButtonPressed)
	{
		if(iAircraft->GetSecondaryWeapon()->GetAmmunitionLeft()!= iAircraft->GetSecondaryWeapon()->GetMaxAmmunitionLeft())
			iFire2ButtonPressed = true;
	}
	
	if(iFire1ButtonPressed && iFire2ButtonPressed)
		return true;
	else
		return false;
}

void CWeaponsTutorialFireButtons::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicAircraftNeededTutorialSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iFire1ButtonPressed); //used from the outside to know what kind of event to create
  aOutputStream.WriteInt32(iFire2ButtonPressed); //used from the outside to know what kind of event to create
}

void CWeaponsTutorialFireButtons::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicAircraftNeededTutorialLoadFromDisk(aReadStream);
	iFire1ButtonPressed = aReadStream.ReadInt32();
	iFire2ButtonPressed = aReadStream.ReadInt32();
}

