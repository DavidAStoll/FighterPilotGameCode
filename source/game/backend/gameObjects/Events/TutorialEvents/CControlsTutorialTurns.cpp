/*
 ============================================================================
 Name		: ControlsTutorialTurns.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CControlsTutorialTurns implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/Events/TutorialEvents/CControlsTutorialTurns.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

CControlsTutorialTurns::CControlsTutorialTurns(CAircraft* aAircraft, TTutorialMethodCallIds aMethodCallIfSuccess)
: CBasicAircraftNeededTutorialClass(EEventIdentifierLoopTutorial, aAircraft,CONTROLS_TUTORIAL_LOOP_SECS_UNTIL_SCUCCESS_MESSAGE, aMethodCallIfSuccess)
	{
	iHasDoneRightTurn = false;
	}

CControlsTutorialTurns::~CControlsTutorialTurns()
	{
	}

TBool CControlsTutorialTurns::TutorialEventLogic() //checks if the Event Condition has been met
{
	TBool lIsReflected = iAircraft->IsReflected();//if reflected that means it it is turned to the right
	if(lIsReflected && !iHasDoneRightTurn)
		iHasDoneRightTurn = true;
	else if(!lIsReflected && iHasDoneRightTurn)//turned back to not reflected state
		return true;
	
	return false;
}

void CControlsTutorialTurns::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	BasicAircraftNeededTutorialSaveOnDisk(aOutputStream);
  aOutputStream.WriteInt32(iHasDoneRightTurn); //used from the outside to know what kind of event to create
}

void CControlsTutorialTurns::LoadFromDisk(CFileReadStream &aReadStream)
{
	BasicAircraftNeededTutorialLoadFromDisk(aReadStream);
	iHasDoneRightTurn = aReadStream.ReadInt32();
}

