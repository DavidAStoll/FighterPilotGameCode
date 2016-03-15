/*
 ============================================================================
 Name		: ControlsTutorialLandOnCarrier.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialLandOnCarrier implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialLandOnCarrier.h"

CControlsTutorialLandOnCarrier::CControlsTutorialLandOnCarrier(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess)
: CBasicAircraftNeededTutorialClass(EEventIdentifierLandOnCarrier, aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
	{
	// No implementation required
	}

CControlsTutorialLandOnCarrier::~CControlsTutorialLandOnCarrier()
{
	
}

TBool CControlsTutorialLandOnCarrier::TutorialEventLogic() //checks if the Event Condition has been met
{
 if(iAircraft->GetCurrentAbsoluteSpeed() == 0 && !iAircraft->IsFlying())
	 return true;
 else
	 return false;
}
