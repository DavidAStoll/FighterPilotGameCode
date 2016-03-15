/*
 ============================================================================
 Name		: ControlsTutorialMoveToEndOfMap.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialMoveToEndOfMap implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialMoveToEndOfMap.h"

CControlsTutorialMoveToEndOfMap::CControlsTutorialMoveToEndOfMap(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess)
: CBasicAircraftNeededTutorialClass(EEventIdentifierMovetoEndOfMapAndAvoidAA,aAircraft, TUTORIAL_DEFAULT_SECS_UNTIL_METHOD_CALL, aMethodCallIfSuccess)
	{
	// No implementation required
	}

CControlsTutorialMoveToEndOfMap::~CControlsTutorialMoveToEndOfMap()
	{
	}

TBool CControlsTutorialMoveToEndOfMap::TutorialEventLogic() //checks if the Event Condition has been met
{
 if(iAircraft->IsAlive() && iAircraft->GetCurrentPositionNormilized().iX < -3500)
		return true;
	else
		return false;
}

