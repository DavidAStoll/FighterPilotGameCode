/*
 ============================================================================
 Name		: RestartLastTutorialScreenIfDiedEvent.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CRestartLastTutorialScreenIfDiedEvent implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CRestartLastTutorialScreenIfDiedEvent.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

CRestartLastTutorialScreenIfDiedEvent::CRestartLastTutorialScreenIfDiedEvent(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfDies)
:		CBasicAircraftNeededTutorialClass(EEventIdentifierRestartLastTutorialIfDied, aAircraft, TUTORIAL_DIE_EVENT_SECS_UNTIL_CALL, aMethodCallIfDies)
{
}

CRestartLastTutorialScreenIfDiedEvent::~CRestartLastTutorialScreenIfDiedEvent()
{
}

TBool CRestartLastTutorialScreenIfDiedEvent::TutorialEventLogic() //checks if the Event Condition has been met
{
	if(!iAircraft->IsAlive())
		return true;
	else
		return false;	//can't be used otherwise we have a double deletion problem
}
