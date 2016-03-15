/*
 ============================================================================
 Name		: ControlsTutorialSpeedUp.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialSpeedUp implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialSpeedUp.h"

CControlsTutorialSpeedUp::CControlsTutorialSpeedUp(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierSpeedUpTutorial,aAircraft, CONTROLS_TUTORIAL_SPEED_UP_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
}

CControlsTutorialSpeedUp::~CControlsTutorialSpeedUp()
{
}

TBool CControlsTutorialSpeedUp::TutorialEventLogic() //checks if the Event Condition has been met
{
 if(iAircraft->GetCurrentEngineAcceleration() == iAircraft->GetMaxEngineAccelerationWithCurrentHealth())
		return true;
	else
		return false;
}


