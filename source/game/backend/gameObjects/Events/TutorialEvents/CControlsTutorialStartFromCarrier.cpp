/*
 ============================================================================
 Name		: ControlsTutorialStartFromCarrier.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialStartFromCarrier implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialStartFromCarrier.h"

CControlsTutorialStartFromCarrier::CControlsTutorialStartFromCarrier(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
	CBasicAircraftNeededTutorialClass(EEventIdentifierStartFromCarrier, aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
{
}

CControlsTutorialStartFromCarrier::~CControlsTutorialStartFromCarrier()
{
}

TBool CControlsTutorialStartFromCarrier::TutorialEventLogic() //checks if the Event Condition has been met
{
 if(iAircraft->GetSecondaryWeapon()->GetWeaponIdentifier() == EWeaponStandardRocket && iAircraft->GetCurrentPositionNormilized().iY >= 700)
	 return true;
 else
	 return false;
}

