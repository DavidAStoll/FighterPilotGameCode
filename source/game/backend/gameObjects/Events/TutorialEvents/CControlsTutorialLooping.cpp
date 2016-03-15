/*
 ============================================================================
 Name		: ControlsTutorialLooping.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialLooping implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialLooping.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

CControlsTutorialLooping::CControlsTutorialLooping(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess) :
CBasicAircraftNeededTutorialClass(EEventIdentifierLoopTutorial, aAircraft, CONTROLS_TUTORIAL_LOOP_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
{
	iCurrentLoopPosition = EControlsTutorialLoopPositionUp90;
}

CControlsTutorialLooping::~CControlsTutorialLooping()
{
}

TBool CControlsTutorialLooping::TutorialEventLogic() //checks if the Event Condition has been met
{
	TInt lPlaneAngle = *iAircraft->GetFlyingAngel();
	switch(iCurrentLoopPosition)
	{
		case EControlsTutorialLoopPositionUp90:
		{
			if(lPlaneAngle == 0)
				iCurrentLoopPosition = EControlsTutorialLoopPositionUp0;//advance to next position
			else if(lPlaneAngle == 270)
				iCurrentLoopPosition = EControlsTutorialLoopPositionDown270;//go to previous position
			break;
		}
		case EControlsTutorialLoopPositionUp0:
		{
			if(lPlaneAngle == 270)
				iCurrentLoopPosition = EControlsTutorialLoopPositionUp270;//advance to next position
			else if(lPlaneAngle == 90)
				iCurrentLoopPosition = EControlsTutorialLoopPositionUp90;//go to previous position
			break;
		}
		case EControlsTutorialLoopPositionUp270:
		{
			if(lPlaneAngle == 180)
				return true; //done
			else if(lPlaneAngle == 0)
				iCurrentLoopPosition = EControlsTutorialLoopPositionUp0;//go to previous position
			break;
		}
			//down turn
		case EControlsTutorialLoopPositionDown270:
		{
			if(lPlaneAngle == 0)
				iCurrentLoopPosition = EControlsTutorialLoopPositionDown0;//advance to next position
			else if(lPlaneAngle == 90)
				iCurrentLoopPosition = EControlsTutorialLoopPositionUp90;//go to previous position
			break;
		}
		case EControlsTutorialLoopPositionDown0:
		{
			if(lPlaneAngle == 90)
				iCurrentLoopPosition = EControlsTutorialLoopPositionDown90;//advance to next position
			else if(lPlaneAngle == 270)
				iCurrentLoopPosition = EControlsTutorialLoopPositionDown270;//go to previous position
			break;
		}
		case EControlsTutorialLoopPositionDown90:
		{
			if(lPlaneAngle == 180)
				return true; //done 
			else if(lPlaneAngle == 0)
				iCurrentLoopPosition = EControlsTutorialLoopPositionDown0;//go to previous position
			break;
		}
	}
	
	return false;
}
